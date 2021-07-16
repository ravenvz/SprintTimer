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
#ifndef TESTQUERYHANDLERCOMPOSER_H_UAVBEGPJ
#define TESTQUERYHANDLERCOMPOSER_H_UAVBEGPJ

#include "core/OperationalRangeReader.h"
#include "core/SprintDistributionReader.h"
#include "core/SprintStorage.h"
#include "core/TaskStorage.h"
#include "core/use_cases/request_op_range/OperationalRangeHandler.h"
#include "core/use_cases/request_schedule/WorkScheduleHandler.h"
#include "core/use_cases/request_sprint_distribution/RequestSprintDistributionHandler.h"
#include "core/use_cases/request_sprints/RequestSprintsHandler.h"
#include "core/use_cases/request_sprints/SprintsForTaskHandler.h"
#include "core/use_cases/request_tags/AllTagsHandler.h"
#include "core/use_cases/request_tasks/ActiveTasksHandler.h"
#include "core/use_cases/request_tasks/FinishedTasksHandler.h"
#include "use_cases_tests/QueryHandlerComposer.h"

namespace sprint_timer::compose {

struct TestQueryHandlerComposer : public QueryHandlerComposer {
    TestQueryHandlerComposer(
        TaskStorage& taskStorage_,
        SprintStorage& sprintStorage_,
        OperationalRangeReader& operationRangeReader_,
        WorkScheduleReader& workScheduleReader_,
        SprintDistributionReader& dailyDistReader_,
        SprintDistributionReader& mondayFirstWeeklyDistReader_,
        SprintDistributionReader& sundayFirstWeeklyDistReader_,
        SprintDistributionReader& monthlyDistReader_)
        : activeTasks{taskStorage_}
        , allTags{taskStorage_}
        , requestSprints{sprintStorage_}
        , finishedTasks{taskStorage_}
        , sprintsForTask{sprintStorage_}
        , operationalRange{operationRangeReader_}
        , dailyDistribution{dailyDistReader_}
        , mondayFirstWeeklyDistribution{mondayFirstWeeklyDistReader_}
        , sundayFirstWeeklyDistribution{sundayFirstWeeklyDistReader_}
        , monthlyDistribution{monthlyDistReader_}
        , workSchedule{workScheduleReader_}
    {
    }

    QueryHandler<use_cases::ActiveTasksQuery>& activeTasksHandler() override
    {
        return activeTasks;
    }

    QueryHandler<use_cases::AllTagsQuery>& allTagsHandler() override
    {
        return allTags;
    }

    QueryHandler<use_cases::RequestSprintsQuery>&
    requestSprintsHandler() override
    {
        return requestSprints;
    }

    QueryHandler<use_cases::FinishedTasksQuery>& finishedTasksHandler() override
    {
        return finishedTasks;
    }

    QueryHandler<use_cases::SprintsForTaskQuery>&
    sprintsForTaskHandler() override
    {
        return sprintsForTask;
    }

    QueryHandler<use_cases::OperationalRangeQuery>&
    operationalRangeHandler() override
    {
        return operationalRange;
    }

    QueryHandler<use_cases::RequestSprintDistributionQuery>&
    dailyDistHandler() override
    {
        return dailyDistribution;
    }

    QueryHandler<use_cases::RequestSprintDistributionQuery>&
    weeklyDistHandler(dw::Weekday firstDayOfWeek) override
    {
        if (firstDayOfWeek == dw::Weekday::Monday) {
            return mondayFirstWeeklyDistribution;
        }
        return sundayFirstWeeklyDistribution;
    }

    QueryHandler<use_cases::RequestSprintDistributionQuery>&
    monthlyDistHandler() override
    {
        return monthlyDistribution;
    }

    QueryHandler<use_cases::WorkScheduleQuery>& workScheduleHandler() override
    {
        return workSchedule;
    }

private:
    use_cases::ActiveTasksHandler activeTasks;
    use_cases::AllTagsHandler allTags;
    use_cases::RequestSprintsHandler requestSprints;
    use_cases::FinishedTasksHandler finishedTasks;
    use_cases::SprintsForTaskHandler sprintsForTask;
    use_cases::OperationalRangeHandler operationalRange;
    use_cases::RequestSprintDistributionHandler dailyDistribution;
    use_cases::RequestSprintDistributionHandler mondayFirstWeeklyDistribution;
    use_cases::RequestSprintDistributionHandler sundayFirstWeeklyDistribution;
    use_cases::RequestSprintDistributionHandler monthlyDistribution;
    use_cases::WorkScheduleHandler workSchedule;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: TESTQUERYHANDLERCOMPOSER_H_UAVBEGPJ */
