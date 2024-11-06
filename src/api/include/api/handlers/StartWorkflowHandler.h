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
#ifndef STARTWORKFLOWHANDLER_H_AN0HBIXK
#define STARTWORKFLOWHANDLER_H_AN0HBIXK

#include "api/com_query/CommandHandler.h"
#include "api/com_query/QueryHandler.h"
#include "api/requests/RequestProgressQuery.h"
#include "api/requests/StartTimerCommand.h"

namespace sprint_timer {

class SprintStorageReader;
class SprintTimer;

} // namespace sprint_timer

namespace sprint_timer::api {

class DateTimeProvider;

class StartWorkflowHandler : public asp::CommandHandler<StartTimerCommand> {
public:
    StartWorkflowHandler(
        DateTimeProvider& dateTimeProvider_,
        SprintTimer& sprintTimer_,
        asp::QueryHandler<RequestProgressQuery>& requestProgressHandler_);

    auto handle(const StartTimerCommand& command) -> void override;

private:
    DateTimeProvider& dateTimeProvider;
    SprintTimer& sprintTimer;
    asp::QueryHandler<RequestProgressQuery>& requestProgressHandler;
};

} // namespace sprint_timer::api

#endif /* end of include guard: STARTWORKFLOWHANDLER_H_AN0HBIXK */

