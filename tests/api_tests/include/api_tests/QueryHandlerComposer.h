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
#ifndef QUERYHANDLERCOMPOSER_H_TUJS0ZHX
#define QUERYHANDLERCOMPOSER_H_TUJS0ZHX

#include "api/com_query/QueryHandler.h"
#include "api/requests/ActiveTasksQuery.h"
#include "api/requests/AllTagsQuery.h"
#include "api/requests/DailyStatisticsQuery.h"
#include "api/requests/FinishedTasksQuery.h"
#include "api/requests/OperationalRangeQuery.h"
#include "api/requests/ReadTaskTreeQuery.h"
#include "api/requests/RequestProgressQuery.h"
#include "api/requests/RequestSprintDistributionQuery.h"
#include "api/requests/RequestSprintsQuery.h"
#include "api/requests/SprintStatisticsQuery.h"
#include "api/requests/SprintsForTaskQuery.h"
#include "api/requests/TopTagFrequenciesQuery.h"
#include "api/requests/WorkScheduleQuery.h"
#include "api/requests/WorkdayStatisticsQuery.h"
#include "api/requests/WorktimeStatisticsQuery.h"

namespace sprint_timer::compose {

struct QueryHandlerComposer {
    virtual ~QueryHandlerComposer() = default;

    virtual auto requestDailyProgressHandler()
        -> asp::QueryHandler<api::RequestProgressQuery>& = 0;

    virtual auto
    activeTasksHandler() -> asp::QueryHandler<api::ActiveTasksQuery>& = 0;

    virtual auto allTagsHandler() -> asp::QueryHandler<api::AllTagsQuery>& = 0;

    virtual auto
    requestSprintsHandler() -> asp::QueryHandler<api::RequestSprintsQuery>& = 0;

    virtual auto
    finishedTasksHandler() -> asp::QueryHandler<api::FinishedTasksQuery>& = 0;

    virtual auto
    sprintsForTaskHandler() -> asp::QueryHandler<api::SprintsForTaskQuery>& = 0;

    virtual auto operationalRangeHandler()
        -> asp::QueryHandler<api::OperationalRangeQuery>& = 0;

    virtual auto dailyDistHandler()
        -> asp::QueryHandler<api::RequestSprintDistributionQuery>& = 0;

    virtual auto weeklyDistHandler(dw::Weekday firstDayOfWeek)
        -> asp::QueryHandler<api::RequestSprintDistributionQuery>& = 0;

    virtual auto monthlyDistHandler()
        -> asp::QueryHandler<api::RequestSprintDistributionQuery>& = 0;

    virtual auto
    workScheduleHandler() -> asp::QueryHandler<api::WorkScheduleQuery>& = 0;

    virtual auto sprintStatisticsHandler()
        -> asp::QueryHandler<api::SprintStatisticsQuery>& = 0;

    virtual auto workdayStatisticsHandler()
        -> asp::QueryHandler<api::WorkdayStatisticsQuery>& = 0;

    virtual auto worktimeStatisticsHandler()
        -> asp::QueryHandler<api::WorktimeStatisticsQuery>& = 0;

    virtual auto dailyStatisticsHandler()
        -> asp::QueryHandler<api::DailyStatisticsQuery>& = 0;

    virtual auto topTagFrequenciesHandler()
        -> asp::QueryHandler<api::TopTagFrequenciesQuery>& = 0;

    virtual auto
    readTaskTreeHandler() -> asp::QueryHandler<api::ReadTaskTreeQuery>& = 0;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: QUERYHANDLERCOMPOSER_H_TUJS0ZHX */
