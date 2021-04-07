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

#include "core/actions/RegisterSprint.h"

namespace sprint_timer::actions {

RegisterSprint::RegisterSprint(SprintStorageWriter& writer_,
                               entities::Sprint newSprint_)
    : writer{writer_}
    , sprint{std::move(newSprint_)}
{
}

void RegisterSprint::execute() { writer.save(sprint); }

void RegisterSprint::undo() { writer.remove(sprint); }

std::string RegisterSprint::describe() const
{
    std::stringstream ss;
    ss << "Register new sprint '" << sprint << "'";
    return ss.str();
}

} // namespace sprint_timer::actions
