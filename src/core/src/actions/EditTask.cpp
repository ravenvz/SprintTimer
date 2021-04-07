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

#include "core/actions/EditTask.h"

using dw::DateTime;

namespace sprint_timer::actions {

using namespace entities;

EditTask::EditTask(TaskStorageWriter& writer_,
                   Task originalTask_,
                   const Task& editedTask_)
    : writer{writer_}
    , editedTask{editedTask_.name(),
                 editedTask_.estimatedCost(),
                 originalTask_.actualCost(),
                 originalTask_.uuid(),
                 editedTask_.tags(),
                 originalTask_.isCompleted(),
                 dw::current_date_time_local()}
    , originalTask{std::move(originalTask_)}
{
}

void EditTask::execute() { writer.edit(originalTask, editedTask); }

void EditTask::undo() { writer.edit(editedTask, originalTask); }

std::string EditTask::describe() const
{
    std::stringstream ss;
    ss << "Edit task '" << originalTask << " -> " << editedTask << "'";
    return ss.str();
}

} // namespace sprint_timer::actions
