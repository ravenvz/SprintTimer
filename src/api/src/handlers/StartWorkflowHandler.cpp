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
#include "api/handlers/StartWorkflowHandler.h"
#include "api/DateTimeProvider.h"
#include "api/SprintStorageReader.h"
#include "core/SprintTimer.h"

namespace sprint_timer::api {

StartWorkflowHandler::StartWorkflowHandler(
    DateTimeProvider& dateTimeProvider_,
    SprintTimer& sprintTimer_,
    asp::QueryHandler<RequestProgressQuery>& requestProgressHandler_)
    : dateTimeProvider{dateTimeProvider_}
    , sprintTimer{sprintTimer_}
    , requestProgressHandler{requestProgressHandler_}
{
}

auto StartWorkflowHandler::handle(const StartTimerCommand& /*command*/) -> void
{
    const auto sprintsFinishedToday =
        requestProgressHandler.handle(RequestProgressQuery{}).actual();
    sprintTimer.setNumFinishedSprints(sprintsFinishedToday);
    sprintTimer.process(
        SprintTimer::StartEvent{dateTimeProvider.dateTimeLocalNow()});
}

} // namespace sprint_timer::api
