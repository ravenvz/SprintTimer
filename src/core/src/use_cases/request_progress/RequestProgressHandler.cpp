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
#include "core/use_cases/request_progress/RequestProgressHandler.h"

#include <iostream>

namespace sprint_timer::use_cases {

RequestProgressHandler::RequestProgressHandler(
    const BackRequestStrategy& backRequestStrategy_,
    const ProgressComputeStrategy& progressComputeStrategy_,
    QueryHandler<RequestSprintDistributionQuery>& requestDistributionHandler_,
    QueryHandler<WorkScheduleQuery>& requestWorkScheduleHandler_)
    : requestDistributionHandler{requestDistributionHandler_}
    , requestWorkScheduleHandler{requestWorkScheduleHandler_}
    , backRequestStrategy{backRequestStrategy_}
    , progressComputeStrategy{progressComputeStrategy_}
{
}

ProgressOverPeriod
RequestProgressHandler::handle(RequestProgressQuery&& /*query*/)
{
    const auto distribution = requestDistributionHandler.handle(
        use_cases::RequestSprintDistributionQuery{
            backRequestStrategy.dateRange()});
    const auto workSchedule =
        requestWorkScheduleHandler.handle(use_cases::WorkScheduleQuery{});
    return ProgressOverPeriod{progressComputeStrategy.computeProgress(
        backRequestStrategy.dateRange(), distribution, workSchedule)};
}

} // namespace sprint_timer::use_cases
