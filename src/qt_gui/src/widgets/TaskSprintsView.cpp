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
#include "qt_gui/widgets/TaskSprintsView.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include "ui_sprints_for_task_view.h"
#include "core/utils/StringUtils.h"

namespace {

using sprint_timer::entities::Sprint;
using sprint_timer::use_cases::SprintDTO;
using sprint_timer::ui::qt_gui::HistoryModel;

HistoryModel::HistoryData
transformToHistoryData(const std::vector<SprintDTO>& sprints);

QString sprintToString(const SprintDTO&);

std::string prefixTags(const std::vector<std::string>& tags);

} // namespace

namespace sprint_timer::ui::qt_gui {

TaskSprintsView::TaskSprintsView(QStyledItemDelegate& delegate_,
                                 QWidget* parent_)
    : StandaloneDisplayableWidget{parent_}
    , ui{std::make_unique<Ui::TaskSprintsView>()}
{
    ui->setupUi(this);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setModel(&historyModel);
    ui->treeView->setItemDelegate(&delegate_);
}

TaskSprintsView::~TaskSprintsView() = default;

void TaskSprintsView::displaySprints(const std::vector<SprintDTO>& sprints)
{
    historyModel.fill(transformToHistoryData(sprints));
    ui->treeView->expandAll();
}

} // namespace sprint_timer::ui::qt_gui

namespace {

HistoryModel::HistoryData
transformToHistoryData(const std::vector<SprintDTO>& sprints)
{
    using sprint_timer::ui::qt_gui::utils::toQDate;
    HistoryModel::HistoryData taskSprintsHistory;
    taskSprintsHistory.reserve(sprints.size());
    std::transform(cbegin(sprints),
                   cend(sprints),
                   std::back_inserter(taskSprintsHistory),
                   [](const auto& elem) {
                       return std::make_pair(toQDate(elem.timeRange.start()),
                                             sprintToString(elem));
                   });
    return taskSprintsHistory;
}

QString sprintToString(const SprintDTO& sprint)
{
    const auto& timeRange = sprint.timeRange;

    return QString("%1 - %2 %3 %4")
        .arg(QString::fromStdString(dw::to_string(timeRange.start(), "hh:mm")))
        .arg(QString::fromStdString(dw::to_string(timeRange.finish(), "hh:mm")))
        .arg(QString::fromStdString(prefixTags(sprint.tags)))
        .arg(QString::fromStdString(sprint.taskName));
}

std::string prefixTags(const std::vector<std::string>& tags)
{
    return sprint_timer::utils::transformJoin(tags, " ", [](const auto& el) {
        std::string res{"#"};
        res += el;
        return res;
    });
}

} // namespace
