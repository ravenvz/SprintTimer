/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#include "core/use_cases/EditTask.h"

namespace UseCases {
EditTask::EditTask(ITaskStorageWriter& writer,
                   const Task& task,
                   const Task& editedTask)
    : writer{writer}
    , task{task}
    , editedTask{editedTask.name(),
                 editedTask.estimatedCost(),
                 task.actualCost(),
                 task.uuid(),
                 std::move(editedTask.tags()),
                 task.isCompleted(),
                 DateTime::currentDateTimeLocal()}
{
}

void EditTask::executeAction() { writer.edit(task, editedTask); }

void EditTask::undoAction() { writer.edit(editedTask, task); }

std::string EditTask::inspect() const
{
    std::stringstream ss;
    ss << "Edit task '" << task << " -> " << editedTask << "'";
    return ss.str();
}
} /* UseCases */
