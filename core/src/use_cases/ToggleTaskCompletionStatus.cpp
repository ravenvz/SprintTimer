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

#include "core/use_cases/ToggleTaskCompletionStatus.h"

using dw::DateTime;

namespace sprint_timer::use_cases {

ToggleTaskCompletionStatus::ToggleTaskCompletionStatus(
    ITaskStorageWriter& taskStorageWriter, const entities::Task& task)
    : writer{taskStorageWriter}
    , uuid{task.uuid()}
    , oldTimeStamp{task.lastModified()}
{
}

void ToggleTaskCompletionStatus::execute()
{
    writer.toggleTaskCompletionStatus(uuid, DateTime::currentDateTimeLocal());
}

void ToggleTaskCompletionStatus::undo()
{
    writer.toggleTaskCompletionStatus(uuid, oldTimeStamp);
}

std::string ToggleTaskCompletionStatus::describe() const
{
    std::stringstream ss;
    ss << "Toggle task completion for " << uuid;
    return ss.str();
}

} // namespace sprint_timer::use_cases
