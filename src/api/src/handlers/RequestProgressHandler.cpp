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
#include "api/handlers/RequestProgressHandler.h"
#include "api/DateTimeProvider.h"
#include "core/BackRequestStrategy.h"

namespace sprint_timer::api {

RequestProgressHandler::RequestProgressHandler(
    const DateTimeProvider& dateTimeProvider_,
    const BackRequestStrategy& backRequestStrategy_,
    const ProgressComputeStrategy& progressComputeStrategy_,
    asp::QueryHandler<RequestSprintDistributionQuery>&
        requestDistributionHandler_,
    asp::QueryHandler<WorkScheduleQuery>& requestWorkScheduleHandler_)
    : dateTimeProvider{dateTimeProvider_}
    , requestDistributionHandler{requestDistributionHandler_}
    , requestWorkScheduleHandler{requestWorkScheduleHandler_}
    , backRequestStrategy{backRequestStrategy_}
    , progressComputeStrategy{progressComputeStrategy_}
{
}

RequestProgressQuery::Result
RequestProgressHandler::handle(const RequestProgressQuery& /*query*/)
{
    const auto period =
        backRequestStrategy.dateRange(dateTimeProvider.dateLocalNow());
    const auto distribution = requestDistributionHandler.handle(
        RequestSprintDistributionQuery{period});
    const auto workSchedule =
        requestWorkScheduleHandler.handle(WorkScheduleQuery{});
    return ProgressOverPeriod{progressComputeStrategy.computeProgress(
        period, distribution, workSchedule)};
}

} // namespace sprint_timer::api
