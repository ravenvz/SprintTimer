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

namespace core::use_cases {

DeleteTask::DeleteTask(ITaskStorageWriter& taskStorageWriter,
                       Task taskToRemove)
    : writer{taskStorageWriter}
    , task{std::move(taskToRemove)}
{
}

void DeleteTask::execute() { writer.remove(task); }

void DeleteTask::undo() { writer.save(task); }

std::string DeleteTask::describe() const
{
    std::stringstream ss;
    ss << "Delete task '" << task << "'";
    return ss.str();
}

} // namespace core::use_cases
