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
#include "qt_gui/widgets/PlannerViews.h"
#include "core/TreeType.h"
#include "cpp_utils/algorithms/optional_ext.h"
#include <QHeaderView>
#include <QLayout>
#include <QVBoxLayout>
#include <ranges>
#include <stack>

namespace {

constexpr int numColumns{6};
constexpr int margin{10};

} // namespace
namespace sprint_timer::ui::qt_gui {

PlannerViews::PlannerViews(QAbstractItemModel& plannerViewsModel_,
                           QWidget* parent_)
    : QWidget{parent_}
    , plannerViewsModel{plannerViewsModel_}
    , treeView{std::make_unique<QTreeView>().release()}
{
    treeView->setModel(&plannerViewsModel);
    auto lt = std::make_unique<QVBoxLayout>();
    treeView->setHeaderHidden(true);
    // treeView->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    lt->addWidget(treeView);
    setLayout(lt.release());
    connect(
        treeView, &QAbstractItemView::clicked, [&](const QModelIndex& index) {
            alg::inspect(presenter(), [&](auto* presenter) {
                presenter->viewClicked(index.data().toString().toStdString());
            });
        });
}

auto PlannerViews::displayViews(
    const ui::contracts::PlannerViewsContract::ViewsTree& viewsTree) -> void
{
    plannerViewsModel.removeRows(0, plannerViewsModel.rowCount());

    std::stack<std::pair<QModelIndex,
                         TreeType<contracts::PlannerViewsContract::
                                      PlannerViewItem>::const_iterator>>
        frontier;

    for (auto child :
         std::views::reverse(viewsTree.children_iterators(viewsTree.cend()))) {
        frontier.push({QModelIndex{}, child});
    }

    while (not frontier.empty()) {
        auto [parent, it] = frontier.top();
        const auto targetRow = plannerViewsModel.rowCount(parent);
        plannerViewsModel.insertRow(targetRow, parent);

        const std::string name = it->name;
        QVariant var;
        var.setValue(QString::fromStdString(name));
        plannerViewsModel.setData(
            plannerViewsModel.index(targetRow, 0, parent), var, Qt::EditRole);

        frontier.pop();

        for (auto child :
             std::views::reverse(viewsTree.children_iterators(it))) {
            frontier.push(
                {plannerViewsModel.index(targetRow, 0, parent), child});
        }
    }

    treeView->expandAll();
    for (int i = 0; i < numColumns; ++i) {
        treeView->resizeColumnToContents(i);
        treeView->setColumnWidth(i, treeView->columnWidth(i) + margin);
    }
}

} // namespace sprint_timer::ui::qt_gui
