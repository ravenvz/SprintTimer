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

constexpr size_t nameCol{0};
constexpr size_t progressCol{1};
constexpr size_t tagsCol{2};
constexpr size_t dueCol{3};
constexpr size_t reminderCol{4};
constexpr size_t auxCol{5};

constexpr int numColumns{5};
constexpr int nameRightMargin{10};
} // namespace

namespace sprint_timer::ui::qt_gui {

PlannerWindow::PlannerWindow(QAbstractItemModel& plannerModel_,
                             QAbstractItemDelegate& itemDelegate_,
                             Displayable& addTaskDialog_,
                             Displayable& editTaskDialog_,
                             QWidget* parent_)
    : StandaloneDisplayableWidget{parent_}
    , plannerModel{plannerModel_}
    , outlineView{std::make_unique<ReordableTreeView>().release()}
    , addTaskDialog{addTaskDialog_}
    , editTaskDialog{editTaskDialog_}
{
    auto lt = std::make_unique<QHBoxLayout>();

    auto pbPlanner = std::make_unique<QPushButton>("Planning");
    auto pbGoals = std::make_unique<QPushButton>("Goals");
    auto pbProjects = std::make_unique<QPushButton>("Projects");
    auto pbReview = std::make_unique<QPushButton>("Review");
    auto buttonLayout = std::make_unique<QVBoxLayout>();

    auto buttonWidget = std::make_unique<QWidget>();
    buttonLayout->addWidget(pbPlanner.release());
    buttonLayout->addWidget(pbGoals.release());
    buttonLayout->addWidget(pbProjects.release());
    buttonLayout->addWidget(pbReview.release());
    buttonWidget->setLayout(buttonLayout.release());
    lt->addWidget(buttonWidget.release());
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

    auto modelIndexToUuid =
        [](const QModelIndex& index) -> std::optional<std::string> {
        return index.isValid()
                   ? index.data(CustomRoles::IdRole).value<std::string>()
                   : std::optional<std::string>{};
    };
    connect(&plannerModel,
            &QAbstractItemModel::rowsMoved,
            [&](const QModelIndex& sourceParent,
                int sourceStart,
                int sourceEnd,
                const QModelIndex& destinationParent,
                int destinationRow) {
                utils::inspect(presenter(), [&](auto* presenter) {
                    presenter->moveNodes(modelIndexToUuid(sourceParent),
                                         sourceStart,
                                         sourceEnd - sourceStart + 1,
                                         modelIndexToUuid(destinationParent),
                                         destinationRow);
                });
            });
}

// auto setData(QAbstractItemModel& model,
//              const contracts::PlannerContract::PlannerItem& payload,
//              const QModelIndex& parent) -> void
// {
//     auto convertItem = [](const auto& itemDescriptor) {
//         QVariant var;
//         var.setValue(std::tuple<QString, QColor, QColor>{
//             QString::fromStdString(itemDescriptor.description),
//             QColor{
//                 QString::fromStdString(std::string{itemDescriptor.foreground})},
//             QColor{QString::fromStdString(
//                 std::string{itemDescriptor.background})}});
//         return var;
//     };
//
//     const auto targetRow = model.rowCount(parent);
//     model.insertRow(targetRow, parent);
//
//     model.setData(model.index(targetRow, 0, parent),
//     convertItem(payload.name)); model.setData(model.index(targetRow, 1,
//     parent),
//                   convertItem(payload.progress));
//     model.setData(model.index(targetRow, 2, parent),
//     convertItem(payload.tags)); model.setData(model.index(targetRow, 3,
//     parent),
//                   convertItem(payload.dueDate));
//     model.setData(model.index(targetRow, 4, parent),
//                   convertItem(payload.reminder));
//     QVariant var;
//     var.setValue(std::tuple(payload.uuid, payload.finished, payload.type));
//     model.setData(model.index(targetRow, 5, parent), var);
// }

auto PlannerWindow::displayPlanner(
    const contracts::PlannerContract::PlannerTree& taskTree) -> void
{
    plannerModel.removeRows(0, plannerModel.rowCount());

    std::stack<std::pair<QModelIndex, std::string>> frontier;
    for (const auto& child : std::views::reverse(taskTree.children())) {
        frontier.push({QModelIndex{}, child});
    }

    while (not frontier.empty()) {
        const auto parent = frontier.top().first;
        const auto uuid = std::move(frontier.top().second);
        const auto& payload = taskTree.payload(uuid).value().get();
        const auto targetRow = plannerModel.rowCount(parent);
        plannerModel.insertRow(targetRow, parent);

        // setData(plannerModel, payload, parent);

        frontier.pop();

        auto convertItem = [](const auto& itemDescriptor) {
            QVariant var;
            var.setValue(std::tuple<QString, QColor, QColor>{
                QString::fromStdString(itemDescriptor.description),
                QColor{QString::fromStdString(
                    std::string{itemDescriptor.foreground})},
                QColor{QString::fromStdString(
                    std::string{itemDescriptor.background})}});
            return var;
        };

        plannerModel.setData(plannerModel.index(targetRow, nameCol, parent),
                             convertItem(payload.name));
        plannerModel.setData(plannerModel.index(targetRow, progressCol, parent),
                             convertItem(payload.progress));
        plannerModel.setData(plannerModel.index(targetRow, tagsCol, parent),
                             convertItem(payload.tags));
        plannerModel.setData(plannerModel.index(targetRow, dueCol, parent),
                             convertItem(payload.dueDate));
        plannerModel.setData(plannerModel.index(targetRow, reminderCol, parent),
                             convertItem(payload.reminder));
        QVariant var;
        var.setValue(std::tuple(payload.uuid, payload.finished, payload.type));
        plannerModel.setData(plannerModel.index(targetRow, auxCol, parent),
                             var);

        for (const auto& child : std::views::reverse(taskTree.children(uuid))) {
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

    auto* addSubtaskAction = contextMenu.addAction("Add Subtask");
    contextMenu.addSeparator();
    auto* addSiblingAction = contextMenu.addAction("Add Sibling Task");
    contextMenu.addSeparator();
    auto* editAction = contextMenu.addAction("Edit");
    contextMenu.addSeparator();
    auto* deleteTaskAction = contextMenu.addAction("Delete");

    QAction* selectedEntry = contextMenu.exec(globalPos);

    if (selectedEntry == nullptr) {
        return;
    }

    if (selectedEntry == addSubtaskAction) {
        utils::inspect(presenter(), [this](auto* presenter) {
            presenter->changeTaskAdditionContext(selectedTaskUuid(), true);
        });
        addTaskDialog.display();
    }

    if (selectedEntry == addSiblingAction) {
        utils::inspect(presenter(), [this](auto* presenter) {
            presenter->changeTaskAdditionContext(selectedTaskUuid(), false);
            addTaskDialog.display();
        });
    }

    if (selectedEntry == editAction) {
        utils::inspect(presenter(), [this](auto* presenter) {
            presenter->changeTaskEditionContext(selectedTaskUuid());
        });
        editTaskDialog.display();
    }

    if (selectedEntry == deleteTaskAction) {
        utils::inspect(presenter(), [this](auto* presenter) {
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
