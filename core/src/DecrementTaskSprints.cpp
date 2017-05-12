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

#include "core/use_cases/DecrementTaskSprints.h"

namespace UseCases {

DecrementTaskSprints::DecrementTaskSprints(
    ITaskStorageWriter& taskStorageWriter, const std::string& taskUuid)
    : writer{taskStorageWriter}
    , taskUuid{taskUuid}
{
}

void DecrementTaskSprints::executeAction()
{
    writer.decrementSprints(taskUuid);
}

void DecrementTaskSprints::undoAction() { writer.incrementSprints(taskUuid); }

std::string DecrementTaskSprints::inspect() const
{
    std::stringstream ss;
    ss << "Decrement sprints for " << taskUuid;
    return ss.str();
}


} /* UseCases */
