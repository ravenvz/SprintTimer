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

#include "core/use_cases/DeleteTask.h"

namespace sprint_timer::use_cases {

DeleteTask::DeleteTask(ITaskStorageWriter& taskStorageWriter,
                       ISprintStorageReader& sprintStorageReader,
                       ISprintStorageWriter& sprintStorageWriter,
                       entities::Task taskToRemove)
    : taskWriter{taskStorageWriter}
    , sprintReader{sprintStorageReader}
    , sprintWriter{sprintStorageWriter}
    , task{std::move(taskToRemove)}
{
}

void DeleteTask::execute()
{
    if (task.actualCost() == 0) {
        taskWriter.remove(task);
        return;
    }
    sprintReader.sprintsForTask(
        task.uuid(), [this](const std::vector<entities::Sprint>& sprints) {
            taskSprints = sprints;
            taskWriter.remove(task);
        });
}

void DeleteTask::undo()
{
    taskWriter.save(task);
    for (const auto& sprint : taskSprints) {
        sprintWriter.save(sprint);
    }
}

std::string DeleteTask::describe() const
{
    std::stringstream ss;
    ss << "Delete task '" << task << "'";
    return ss.str();
}

} // namespace sprint_timer::use_cases
