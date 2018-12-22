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
#include "core/use_cases/RegisterNewSprintBulk.h"

namespace sprint_timer::use_cases {

using entities::Sprint;

RegisterNewSprintBulk::RegisterNewSprintBulk(ISprintStorageWriter& writer_,
                                             std::vector<Sprint> sprints_)
    : writer{writer_}
    , sprints{std::move(sprints_)}
{
}

void RegisterNewSprintBulk::execute() { writer.save(sprints); }

void RegisterNewSprintBulk::undo() { writer.remove(sprints); }

std::string RegisterNewSprintBulk::describe() const
{
    std::stringstream ss;
    ss << "Register new sprint bulk:\n";
    std::copy(sprints.cbegin(),
              sprints.cend(),
              std::ostream_iterator<Sprint>(ss, "\n"));
    std::string res{ss.str()};
    res.pop_back();
    return ss.str();
}

} // namespace sprint_timer::use_cases

