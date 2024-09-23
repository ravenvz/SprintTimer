/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "qt_gui/widgets/PlannerWindow.h"
#include "cpp_utils/algorithms/optional_ext.h"
#include "cpp_utils/algorithms/string_ext.h"
#include "qt_gui/models/CustomRoles.h"
#include "qt_gui/models/PlannerModel.h"
#include "qt_gui/widgets/ReordableTreeView.h"
#include <QAbstractItemModel>
#include <QDialog>
#include <QHeaderView>
#include <QMenu>
#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>
#include <unordered_set>

namespace {

constexpr int numColumns{6};
constexpr int nameRightMargin{10};

auto transformTags(const QString& raw_tags) -> std::vector<std::string>;

auto transformProgress(const QString& raw_progress) -> int;

auto uuidFromIndex =
    [](const QModelIndex& index) -> std::optional<std::string> {
    return index.isValid()
               ? std::optional<std::string>{index
                                                .data(sprint_timer::ui::qt_gui::
                                                          CustomRoles::IdRole)
                                                .value<std::string>()}
               : std::optional<std::string>{};
};

} // namespace

namespace sprint_timer::ui::qt_gui {

PlannerWindow::PlannerWindow(QAbstractItemModel& plannerModel_,
                             QAbstractItemDelegate& itemDelegate_,
                             std::unique_ptr<QWidget> plannerViews_,
                             Displayable& addTaskDialog_,
                             Displayable& editTaskDialog_,
                             QWidget* parent_)
    : StandaloneDisplayableWidget{parent_}
    , plannerModel{plannerModel_}
    , outlineView{std::make_unique<ReordableTreeView>().release()}
    // , plannerViews{plannerViews_.release}
    , addTaskDialog{addTaskDialog_}
    , editTaskDialog{editTaskDialog_}
{
    auto lt = std::make_unique<QHBoxLayout>();

    auto pbPlanner = std::make_unique<QPushButton>("Planning");
    auto pbGoals = std::make_unique<QPushButton>("Goals");
    auto pbProjects = std::make_unique<QPushButton>("Projects");
    auto pbReview = std::make_unique<QPushButton>("Review");
    auto buttonLayout = std::make_unique<QVBoxLayout>();

    // auto buttonWidget = std::make_unique<QWidget>();
    // buttonLayout->addWidget(pbPlanner.release());
    // buttonLayout->addWidget(pbGoals.release());
    // buttonLayout->addWidget(pbProjects.release());
    // buttonLayout->addWidget(pbReview.release());
    // buttonWidget->setLayout(buttonLayout.release());
    lt->addWidget(plannerViews_.release());
    lt->addWidget(outlineView);

    setLayout(lt.release());

    setMinimumWidth(1280);
    setMinimumHeight(1024);

    outlineView->setModel(&plannerModel);
    outlineView->setItemDelegate(&itemDelegate_);

    // NOTE that resizing depends on SizeHint; in this case it is provided by
    // QStyledItemDelegate implementation (PlannerItemDelegate)
    outlineView->header()->setStretchLastSection(false);
    outlineView->header()->resizeSections(QHeaderView::ResizeToContents);
    outlineView->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    outlineView->setColumnHidden(5, true);
    outlineView->setColumnHidden(6, true);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,
            &ReordableTreeView::customContextMenuRequested,
            this,
            &PlannerWindow::showContextMenu);

    connect(&plannerModel,
            &QAbstractItemModel::rowsMoved,
            [&](const QModelIndex& sourceParent,
                int sourceStart,
                int sourceEnd,
                const QModelIndex& destinationParent,
                int destinationRow) {
                alg::inspect(presenter(), [&](auto* presenter) {
                    presenter->moveNodes(uuidFromIndex(sourceParent),
                                         sourceStart,
                                         sourceEnd - sourceStart + 1,
                                         uuidFromIndex(destinationParent),
                                         destinationRow);
                });
            });
    connect(&plannerModel,
            &QAbstractItemModel::dataChanged,
            [&](const QModelIndex& topLeft,
                const QModelIndex& bottomRight,
                const QList<int>& roles) {
                if (roles.contains(Qt::EditRole)) {
                    handleEdit(topLeft);
                }
                if (roles.contains(Qt::CheckStateRole)) {
                    handleToggle(topLeft);
                }
            });
}

auto PlannerWindow::handleEdit(const QModelIndex& index) const -> void
{
    QVariant var = index.data(Qt::EditRole);
    auto payload =
        var.value<std::tuple<std::string, QString, QString, QString>>();
    ;
    auto [uuid, raw_name, raw_progress, raw_tags] = payload;
    alg::inspect(presenter(), [&](auto* presenter) {
        presenter->quickEditTask(std::move(uuid),
                                 raw_name.toStdString(),
                                 transformTags(raw_tags),
                                 transformProgress(raw_progress));
    });
}

auto PlannerWindow::handleToggle(const QModelIndex& index) const -> void
{
    alg::inspect(presenter(), [&](auto* presenter) {
        alg::inspect(uuidFromIndex(index),
                     [&](const auto& uuid) { presenter->toggleTask(uuid); });
    });
}

auto PlannerWindow::displayPlanner(
    const contracts::PlannerContract::PlannerTree& taskTree) -> void
{
    using contracts::PlannerContract::PlannerTree;
    using namespace std::views;

    plannerModel.removeRows(0, plannerModel.rowCount());

    auto name_tree = taskTree.transform([](const auto& payload) {
        return std::string{"Name: "} + payload.name.description;
    });

    std::stack<std::pair<QModelIndex, PlannerTree::const_iterator>> frontier;

    for (auto child : reverse(taskTree.children_iterators(taskTree.cend()))) {
        frontier.push({QModelIndex{}, child});
    }

    while (not frontier.empty()) {
        const auto [parent, it] = frontier.top();
        const auto targetRow = plannerModel.rowCount(parent);
        plannerModel.insertRow(targetRow, parent);

        QVariant var;
        var.setValue(*it);
        plannerModel.setData(plannerModel.index(targetRow, 0, parent),
                             var,
                             CustomRoles::ItemRole);

        frontier.pop();

        for (auto child : reverse(taskTree.children_iterators(it))) {
            frontier.push({plannerModel.index(targetRow, 0, parent), child});
        }
    }

    outlineView->expandAll();
    for (int i = 0; i < numColumns; ++i) {
        outlineView->resizeColumnToContents(i);
        outlineView->setColumnWidth(
            i, outlineView->columnWidth(i) + nameRightMargin);
    }
}

auto PlannerWindow::showContextMenu(const QPoint& pos) const -> void
{
    auto globalPos = mapToGlobal(pos);
    QMenu contextMenu;

    const bool treeIsEmpty{outlineView->model()->rowCount() == 0};

    auto* addSubtaskAction = contextMenu.addAction("Add Subtask");
    contextMenu.addSeparator();

    auto* addSiblingAction = contextMenu.addAction("Add Sibling Task");
    addSiblingAction->setEnabled(not treeIsEmpty);
    contextMenu.addSeparator();

    auto* editAction = contextMenu.addAction("Edit");
    contextMenu.addSeparator();

    auto* deleteTaskAction = contextMenu.addAction("Delete");

    QAction* selectedEntry = contextMenu.exec(globalPos);

    if (selectedEntry == nullptr) {
        return;
    }

    if (selectedEntry == addSubtaskAction) {
        alg::inspect(presenter(), [this, treeIsEmpty](auto* presenter) {
            presenter->changeTaskAdditionContext(
                treeIsEmpty ? std::optional<std::string>{} : selectedTaskUuid(),
                true);
        });
        addTaskDialog.display();
    }

    if (selectedEntry == addSiblingAction) {
        alg::inspect(presenter(), [this](auto* presenter) {
            presenter->changeTaskAdditionContext(selectedTaskUuid(), false);
            addTaskDialog.display();
        });
    }

    if (selectedEntry == editAction) {
        alg::inspect(presenter(), [this](auto* presenter) {
            presenter->changeTaskEditionContext(selectedTaskUuid());
        });
        editTaskDialog.display();
    }

    if (selectedEntry == deleteTaskAction) {
        alg::inspect(presenter(), [this](auto* presenter) {
            presenter->deleteTask(selectedTaskUuid());
        });
    }
}

auto PlannerWindow::selectedTaskUuid() const -> std::string
{
    return outlineView->currentIndex()
        .data(CustomRoles::IdRole)
        .value<std::string>();
}

} // namespace sprint_timer::ui::qt_gui

namespace {

auto transformTags(const QString& raw_tags) -> std::vector<std::string>
{
    const std::string s_tags = raw_tags.toStdString();
    std::vector<std::string> result;
    alg::parseWords(cbegin(s_tags), cend(s_tags), std::back_inserter(result));
    return result;
    ;
}

auto transformProgress(const QString& raw_progress) -> int
{
    qDebug() << "Raw progress: " << raw_progress;
    return raw_progress.split("/").back().toInt();
}

} // namespace

