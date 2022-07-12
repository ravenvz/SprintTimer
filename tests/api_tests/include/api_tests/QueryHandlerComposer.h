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

    virtual asp::QueryHandler<api::ActiveTasksQuery>& activeTasksHandler() = 0;

    virtual asp::QueryHandler<api::AllTagsQuery>& allTagsHandler() = 0;

    virtual asp::QueryHandler<api::RequestSprintsQuery>&
    requestSprintsHandler() = 0;

    virtual asp::QueryHandler<api::FinishedTasksQuery>&
    finishedTasksHandler() = 0;

    virtual asp::QueryHandler<api::SprintsForTaskQuery>&
    sprintsForTaskHandler() = 0;

    virtual asp::QueryHandler<api::OperationalRangeQuery>&
    operationalRangeHandler() = 0;

    virtual asp::QueryHandler<api::RequestSprintDistributionQuery>&
    dailyDistHandler() = 0;

    virtual asp::QueryHandler<api::RequestSprintDistributionQuery>&
    weeklyDistHandler(dw::Weekday firstDayOfWeek) = 0;

    virtual asp::QueryHandler<api::RequestSprintDistributionQuery>&
    monthlyDistHandler() = 0;

    virtual asp::QueryHandler<api::WorkScheduleQuery>&
    workScheduleHandler() = 0;

    virtual asp::QueryHandler<api::SprintStatisticsQuery>&
    sprintStatisticsHandler() = 0;

    virtual asp::QueryHandler<api::WorkdayStatisticsQuery>&
    workdayStatisticsHandler() = 0;

    virtual asp::QueryHandler<api::WorktimeStatisticsQuery>&
    worktimeStatisticsHandler() = 0;

    virtual asp::QueryHandler<api::DailyStatisticsQuery>&
    dailyStatisticsHandler() = 0;

    virtual asp::QueryHandler<api::TopTagFrequenciesQuery>&
    topTagFrequenciesHandler() = 0;

    virtual asp::QueryHandler<api::ReadTaskTreeQuery>&
    readTaskTreeHandler() = 0;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: QUERYHANDLERCOMPOSER_H_TUJS0ZHX */
