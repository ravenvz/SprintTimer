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

#include "core/actions/DeleteTask.h"

namespace sprint_timer::actions {

DeleteTask::DeleteTask(TaskStorageWriter& taskStorageWriter_,
                       entities::Task taskToRemove_)
    : taskWriter{taskStorageWriter_}
    , task{std::move(taskToRemove_)}
{
}

void DeleteTask::execute()
{
    if (task.actualCost() == 0) {
        taskWriter.remove(task.uuid());
        return;
    }
    taskWriter.remove(task.uuid());
}

void DeleteTask::undo() { taskWriter.save(task); }

std::string DeleteTask::describe() const
{
    std::stringstream ss;
    ss << "Delete task '" << task << "'";
    return ss.str();
}

} // namespace sprint_timer::actions
