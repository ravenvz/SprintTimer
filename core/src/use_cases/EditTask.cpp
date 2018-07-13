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

#include "core/use_cases/EditTask.h"

using dw::DateTime;

namespace sprint_timer::use_cases {

using namespace entities;

EditTask::EditTask(ITaskStorageWriter& writer,
                   Task originalTask,
                   Task editedTask)
    : writer{writer}
    , originalTask_{std::move(originalTask)}
    , editedTask_{editedTask.name(),
                  editedTask.estimatedCost(),
                  originalTask_.actualCost(),
                  originalTask_.uuid(),
                  editedTask.tags(),
                  originalTask_.isCompleted(),
                  DateTime::currentDateTimeLocal()}
{
}

void EditTask::execute() { writer.edit(originalTask_, editedTask_); }

void EditTask::undo() { writer.edit(editedTask_, originalTask_); }

std::string EditTask::describe() const
{
    std::stringstream ss;
    ss << "Edit task '" << originalTask_ << " -> " << editedTask_ << "'";
    return ss.str();
}

} // namespace sprint_timer::use_cases
