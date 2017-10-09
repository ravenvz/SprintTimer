/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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

#include "core/use_cases/RegisterNewSprint.h"

namespace core::use_cases {

RegisterNewSprint::RegisterNewSprint(ISprintStorageWriter& writer,
                                     Sprint newSprint)
    : writer{writer}
    , sprint{std::move(newSprint)}
{
}

void RegisterNewSprint::execute() { writer.save(sprint); }

void RegisterNewSprint::undo() { writer.remove(sprint); }

std::string RegisterNewSprint::describe() const
{
    std::stringstream ss;
    ss << "Register new sprint '" << sprint << "'";
    return ss.str();
}

} // namespace core::use_cases
