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
#ifndef REQUESTPROGRESSHANDLER_H_UNLHXZPA
#define REQUESTPROGRESSHANDLER_H_UNLHXZPA

#include "api/com_query/QueryHandler.h"
#include "api/requests/RequestProgressQuery.h"
#include "api/requests/RequestSprintDistributionQuery.h"
#include "api/requests/WorkScheduleQuery.h"
#include "core/ProgressOverPeriod.h"

namespace sprint_timer {

class BackRequestStrategy;
class ProgressComputeStrategy;

} // namespace sprint_timer

namespace sprint_timer::api {

class DateTimeProvider;

class RequestProgressHandler : public asp::QueryHandler<RequestProgressQuery> {
public:
    RequestProgressHandler(
        const DateTimeProvider& dateTimeProvider,
        const BackRequestStrategy& backRequestStrategy,
        const ProgressComputeStrategy& progressComputeStrategy,
        asp::QueryHandler<RequestSprintDistributionQuery>&
            requestDistributionHandler,
        asp::QueryHandler<WorkScheduleQuery>& requestWorkScheduleHandler);

    ProgressOverPeriod handle(const RequestProgressQuery& query) override;

private:
    const DateTimeProvider& dateTimeProvider;
    asp::QueryHandler<RequestSprintDistributionQuery>&
        requestDistributionHandler;
    asp::QueryHandler<WorkScheduleQuery>& requestWorkScheduleHandler;
    const BackRequestStrategy& backRequestStrategy;
    const ProgressComputeStrategy& progressComputeStrategy;
};

} // namespace sprint_timer::api

#endif /* end of include guard: REQUESTPROGRESSHANDLER_H_UNLHXZPA */
