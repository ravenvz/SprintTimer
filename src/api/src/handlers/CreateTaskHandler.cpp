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
#include "api/handlers/CreateTaskHandler.h"
#include "api/actions/CreateTask.h"
#include "api/dtos/TagMapper.h"

namespace sprint_timer::api {

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

void CreateTaskHandler::handle(const CreateTaskCommand& command)
{
    std::vector<Tag> tags;
    std::ranges::copy(dtoAdapter(command.tags), std::back_inserter(tags));
    actionInvoker.execute(std::make_unique<actions::CreateTask>(
        writer,
        Task{command.name,
             command.estimatedCost,
             {},
             uuidGenerator.generateUUID(),
             tags,
             false,
             dateTimeProvider.dateTimeLocalNow()}));
}

} // namespace sprint_timer::api

