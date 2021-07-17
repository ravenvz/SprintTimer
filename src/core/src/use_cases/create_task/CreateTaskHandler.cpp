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
#include "core/use_cases/create_task/CreateTaskHandler.h"
#include "core/actions/CreateTask.h"

namespace sprint_timer::use_cases {

using entities::Tag;
using entities::Task;

CreateTaskHandler::CreateTaskHandler(TaskStorageWriter& writer_,
                                     ActionInvoker& actionInvoker_,
                                     UUIDGenerator& uuidGenerator_,
                                     DateTimeProvider& dateTimeProvider_)
    : writer{writer_}
    , actionInvoker{actionInvoker_}
    , uuidGenerator{uuidGenerator_}
    , dateTimeProvider{dateTimeProvider_}
{
}

void CreateTaskHandler::handle(CreateTaskCommand&& command)
{
    std::list<Tag> tags;
    std::transform(cbegin(command.tags),
                   cend(command.tags),
                   std::back_inserter(tags),
                   [](const auto& elem) { return Tag{elem}; });
    actionInvoker.execute(std::make_unique<actions::CreateTask>(
        writer,
        Task{command.name,
             command.estimatedCost,
             0,
             uuidGenerator.generateUUID(),
             tags,
             false,
             dateTimeProvider.dateTimeLocalNow()}));
}

} // namespace sprint_timer::use_cases

