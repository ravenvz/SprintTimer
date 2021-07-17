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

#include "core/QueryHandler.h"
#include "core/use_cases/request_op_range/OperationalRangeQuery.h"
#include "core/use_cases/request_schedule/WorkScheduleQuery.h"
#include "core/use_cases/request_sprint_distribution/RequestSprintDistributionQuery.h"
#include "core/use_cases/request_sprints/RequestSprintsQuery.h"
#include "core/use_cases/request_sprints/SprintsForTaskQuery.h"
#include "core/use_cases/request_tags/AllTagsQuery.h"
#include "core/use_cases/request_tasks/ActiveTasksQuery.h"
#include "core/use_cases/request_tasks/FinishedTasksQuery.h"

namespace sprint_timer::compose {

struct QueryHandlerComposer {
    virtual ~QueryHandlerComposer() = default;

    virtual QueryHandler<use_cases::ActiveTasksQuery>& activeTasksHandler() = 0;

    virtual QueryHandler<use_cases::AllTagsQuery>& allTagsHandler() = 0;

    virtual QueryHandler<use_cases::RequestSprintsQuery>&
    requestSprintsHandler() = 0;

    virtual QueryHandler<use_cases::FinishedTasksQuery>&
    finishedTasksHandler() = 0;

    virtual QueryHandler<use_cases::SprintsForTaskQuery>&
    sprintsForTaskHandler() = 0;

    virtual QueryHandler<use_cases::OperationalRangeQuery>&
    operationalRangeHandler() = 0;

    virtual QueryHandler<use_cases::RequestSprintDistributionQuery>&
    dailyDistHandler() = 0;

    virtual QueryHandler<use_cases::RequestSprintDistributionQuery>&
    weeklyDistHandler(dw::Weekday firstDayOfWeek) = 0;

    virtual QueryHandler<use_cases::RequestSprintDistributionQuery>&
    monthlyDistHandler() = 0;

    virtual QueryHandler<use_cases::WorkScheduleQuery>&
    workScheduleHandler() = 0;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: QUERYHANDLERCOMPOSER_H_TUJS0ZHX */
