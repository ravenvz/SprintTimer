/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include "qt_gui/widgets/TaskSprintsView.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include "ui_sprints_for_task_view.h"

namespace {

using sprint_timer::entities::Sprint;
using sprint_timer::ui::qt_gui::HistoryModel;

HistoryModel::HistoryData
transformToHistoryData(const std::vector<Sprint>& sprints);

QString sprintToString(const Sprint&);

} // namespace


namespace sprint_timer::ui::qt_gui {

TaskSprintsView::TaskSprintsView(HistoryModel& model,
                                 QStyledItemDelegate& delegate,
                                 QWidget* parent)
    : QWidget{parent}
    , ui{std::make_unique<Ui::TaskSprintsView>()}
    , model{model}
{
    ui->setupUi(this);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setModel(&model);
    ui->treeView->setItemDelegate(&delegate);
}

TaskSprintsView::~TaskSprintsView() = default;

void TaskSprintsView::setData(const std::vector<entities::Sprint>& sprints)
{
    model.fill(transformToHistoryData(sprints));
    ui->treeView->expandAll();
}

} // namespace sprint_timer::ui::qt_gui


namespace {

HistoryModel::HistoryData
transformToHistoryData(const std::vector<Sprint>& sprints)
{
    using sprint_timer::ui::qt_gui::DateTimeConverter;
    HistoryModel::HistoryData taskSprintsHistory;
    taskSprintsHistory.reserve(sprints.size());
    std::transform(cbegin(sprints),
                   cend(sprints),
                   std::back_inserter(taskSprintsHistory),
                   [](const auto& sprint) {
                       return std::make_pair(
                           DateTimeConverter::qDate(sprint.startTime()),
                           sprintToString(sprint));
                   });
    return taskSprintsHistory;
}

QString sprintToString(const Sprint& sprint)
{
    return QString("%1 - %2 %3 %4")
        .arg(QString::fromStdString(dw::to_string(sprint.startTime(), "hh:mm")))
        .arg(
            QString::fromStdString(dw::to_string(sprint.finishTime(), "hh:mm")))
        .arg(QString::fromStdString(prefixTags(sprint.tags())))
        .arg(QString::fromStdString(sprint.name()));
}

} // namespace
