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

#include "core/actions/DeleteTask.h"

namespace sprint_timer::actions {

DeleteTask::DeleteTask(TaskStorageWriter& taskStorageWriter_,
                       SprintStorage& sprintStorage_,
                       entities::Task taskToRemove_)
    : taskWriter{taskStorageWriter_}
    , sprintStorage{sprintStorage_}
    , task{std::move(taskToRemove_)}
{
}

void DeleteTask::execute()
{
    if (task.actualCost() == 0) {
        taskWriter.remove(task.uuid());
        return;
    }
    taskSprints = sprintStorage.findByTaskUuid(task.uuid());
    taskWriter.remove(task.uuid());
}

void DeleteTask::undo()
{
    // This prevents doubling actual cost in case when deletion
    // of Task that has sprints will be undone - otherwise actual cost
    // will be restored and when sprints are added afterwards, actual cost
    // effectively doubles.
    if (task.actualCost() != 0) {
        entities::Task nullifiedCostTask = task;
        nullifiedCostTask.setActualCost(0);
        taskWriter.save(nullifiedCostTask);
    }
    else {
        taskWriter.save(task);
    }

    sprintStorage.save(taskSprints);
}

std::string DeleteTask::describe() const
{
    std::stringstream ss;
    ss << "Delete task '" << task << "'";
    return ss.str();
}

} // namespace sprint_timer::actions
