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
#include "core/use_cases/register_sprint/RegisterSprintBulkHandler.h"
#include "core/actions/RegisterSprintBulk.h"
#include "core/entities/Sprint.h"
#include <algorithm>

namespace sprint_timer::use_cases {

RegisterSprintBulkHandler::RegisterSprintBulkHandler(
    SprintStorageWriter& writer_,
    ActionInvoker& actionInvoker_,
    UUIDGenerator& uuidGenerator_)
    : writer{writer_}
    , actionInvoker{actionInvoker_}
    , uuidGenerator{uuidGenerator_}
{
}

void RegisterSprintBulkHandler::handle(RegisterSprintBulkCommand&& command)
{
    std::vector<entities::Sprint> sprints;
    sprints.reserve(command.intervals.size());
    std::transform(cbegin(command.intervals),
                   cend(command.intervals),
                   std::back_inserter(sprints),
                   [&](const auto& interval) {
                       return entities::Sprint{"",
                                               interval,
                                               std::list<entities::Tag>{},
                                               uuidGenerator.generateUUID(),
                                               command.taskUuid};
                   });
    actionInvoker.execute(
        std::make_unique<actions::RegisterSprintBulk>(writer, sprints));
}

} // namespace sprint_timer::use_cases
