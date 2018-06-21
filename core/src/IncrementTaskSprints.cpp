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

#include "core/use_cases/IncrementTaskSprints.h"

namespace sprint_timer::use_cases {

IncrementTaskSprints::IncrementTaskSprints(
    ITaskStorageWriter& taskStorageWriter, std::string taskUuid)
    : writer{taskStorageWriter}
    , taskUuid_{std::move(taskUuid)}
{
}

void IncrementTaskSprints::execute() { writer.incrementSprints(taskUuid_); }

void IncrementTaskSprints::undo() { writer.decrementSprints(taskUuid_); }

std::string IncrementTaskSprints::describe() const
{
    std::stringstream ss;
    ss << "Increment finished sprints for " << taskUuid_;
    return ss.str();
}

} // namespace sprint_timer::use_cases
