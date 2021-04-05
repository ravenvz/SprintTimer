/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "core/actions/RegisterSprintBulk.h"

namespace sprint_timer::actions {

using entities::Sprint;

RegisterSprintBulk::RegisterSprintBulk(SprintStorageWriter& writer_,
                                       std::vector<Sprint> sprints_)
    : writer{writer_}
    , sprints{std::move(sprints_)}
{
}

void RegisterSprintBulk::execute() { writer.save(sprints); }

void RegisterSprintBulk::undo() { writer.remove(sprints); }

std::string RegisterSprintBulk::describe() const
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

} // namespace sprint_timer::actions

