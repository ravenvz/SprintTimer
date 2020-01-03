/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "qt_gui/SprintRegistrator.h"
#include "qt_gui/models/TaskModelRoles.h"
#include <core/use_cases/RegisterNewSprintBulk.h>

namespace {

std::vector<sprint_timer::entities::Sprint>
buildFromIntervals(const std::string& taskUuid,
                   const std::vector<dw::DateTimeRange>& intervals);

} // namespace

namespace sprint_timer::ui::qt_gui {

SprintRegistrator::SprintRegistrator(
    QAbstractItemModel& taskModel_,
    ISprintStorageWriter& sprintWriter_,
    CommandInvoker& commandInvoker_,
    IndexChangedReemitter& selectedTaskRowReemitter_,
    QObject* parent_)
    : QObject{parent_}
    , taskModel{taskModel_}
    , sprintWriter{sprintWriter_}
    , commandInvoker{commandInvoker_}
{
    connect(&selectedTaskRowReemitter_,
            &IndexChangedReemitter::currentRowChanged,
            this,
            &SprintRegistrator::onTaskSelected);
}

void SprintRegistrator::onSubmissionRequested(
    const std::vector<dw::DateTimeRange>& timeIntervals)
{
    if (candidateRow == -1)
        return;
    const QModelIndex index = taskModel.index(candidateRow, 0);
    if (!index.isValid())
        return;
    const auto taskUuid =
        index.data(static_cast<int>(TaskModelRoles::GetIdRole))
            .toString()
            .toStdString();
    const auto sprints = buildFromIntervals(taskUuid, timeIntervals);
    commandInvoker.executeCommand(
        std::make_unique<use_cases::RegisterNewSprintBulk>(sprintWriter,
                                                           sprints));
}

void SprintRegistrator::onTaskSelected(int taskRow) { candidateRow = taskRow; }

} // namespace sprint_timer::ui::qt_gui

namespace {

std::vector<sprint_timer::entities::Sprint>
buildFromIntervals(const std::string& taskUuid,
                   const std::vector<dw::DateTimeRange>& intervals)
{
    std::vector<sprint_timer::entities::Sprint> sprints;
    sprints.reserve(intervals.size());

    std::transform(
        intervals.cbegin(),
        intervals.cend(),
        std::back_inserter(sprints),
        [&taskUuid](const auto& interval) {
            return sprint_timer::entities::Sprint{taskUuid, interval};
        });

    return sprints;
};

} // namespace
