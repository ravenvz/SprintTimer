/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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

#include "core/BackRequestStrategy.h"
#include "core/ProgressOverPeriod.h"
#include "core/QueryHandler.h"
#include "core/use_cases/request_progress/RequestProgressQuery.h"
#include "core/use_cases/request_schedule/WorkScheduleQuery.h"
#include "core/use_cases/request_sprint_distribution/RequestSprintDistributionQuery.h"

namespace sprint_timer::use_cases {

class RequestProgressHandler
    : public QueryHandler<RequestProgressQuery, ProgressOverPeriod> {
public:
    RequestProgressHandler(
        const BackRequestStrategy& backRequestStrategy,
        const ProgressComputeStrategy& progressComputeStrategy,
        QueryHandler<RequestSprintDistributionQuery, std::vector<int>>&
            requestDistributionHandler,
        QueryHandler<WorkScheduleQuery, WorkSchedule>&
            requestWorkScheduleHandler);

    ProgressOverPeriod handle(RequestProgressQuery&& query) override;

private:
    QueryHandler<RequestSprintDistributionQuery, std::vector<int>>&
        requestDistributionHandler;
    QueryHandler<WorkScheduleQuery, WorkSchedule>& requestWorkScheduleHandler;
    const BackRequestStrategy& backRequestStrategy;
    const ProgressComputeStrategy& progressComputeStrategy;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: REQUESTPROGRESSHANDLER_H_UNLHXZPA */
