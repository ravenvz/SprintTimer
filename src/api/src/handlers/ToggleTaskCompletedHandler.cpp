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
#include "api/handlers/ToggleTaskCompletedHandler.h"
#include "api/actions/ToggleTaskCompleted.h"

namespace sprint_timer::api {

ToggleTaskCompletedHandler::ToggleTaskCompletedHandler(
    TaskStorage& taskStorage_,
    ActionInvoker& actionInvoker_,
    UUIDGenerator& uuidGenerator_,
    const DateTimeProvider& dateTimeProvider_)
    : taskStorage{taskStorage_}
    , actionInvoker{actionInvoker_}
    , uuidGenerator{uuidGenerator_}
    , dateTimeProvider{dateTimeProvider_}
{
}

void ToggleTaskCompletedHandler::handle(
    const ToggleTaskCompletedCommand& command)
{
    actionInvoker.execute(
        actions::ToggleTaskCompleted{taskStorage,
                                     command.taskUuid,
                                     command.lastModificationTimestamp,
                                     uuidGenerator,
                                     dateTimeProvider});
}

} // namespace sprint_timer::api
