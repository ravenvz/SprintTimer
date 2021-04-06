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

#include "core/actions/ToggleTaskCompleted.h"

using dw::DateTime;

namespace sprint_timer::actions {

ToggleTaskCompleted::ToggleTaskCompleted(TaskStorageWriter& taskStorageWriter,
                                         const entities::Task& task)
    : writer{taskStorageWriter}
    , uuid{task.uuid()}
    , oldTimeStamp{task.lastModified()}
{
}

void ToggleTaskCompleted::execute()
{
    writer.toggleCompleted(uuid, dw::current_date_time_local());
}

void ToggleTaskCompleted::undo() { writer.toggleCompleted(uuid, oldTimeStamp); }

std::string ToggleTaskCompleted::describe() const
{
    std::stringstream ss;
    ss << "Toggle task completion for " << uuid;
    return ss.str();
}

} // namespace sprint_timer::actions
