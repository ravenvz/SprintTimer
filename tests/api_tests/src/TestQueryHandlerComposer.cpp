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
#include "api_tests/TestQueryHandlerComposer.h"

namespace sprint_timer::compose {

TestQueryHandlerComposer::TestQueryHandlerComposer(
    TaskStorage& taskStorage_,
    SprintStorage& sprintStorage_,
    OperationalRangeReader& operationRangeReader_,
    WorkScheduleReader& workScheduleReader_,
    SprintDistributionReader& dailyDistReader_,
    SprintDistributionReader& mondayFirstWeeklyDistReader_,
    SprintDistributionReader& sundayFirstWeeklyDistReader_,
    SprintDistributionReader& monthlyDistReader_,
    const patterns::Converter<api::TaskDTO, Task>& taskMapper_,
    const patterns::Converter<std::string, Tag>& tagMapper_,
    // const Converter<dw::DateTimeRange, Sprint>& sprintMapper_,
    const patterns::Converter<api::SprintDTO, SprintRecord>& sprintMapper_,
    const patterns::Converter<api::TaskTreeDTO, TaskTree>& taskTreeMapper_)
    : activeTasks{taskStorage_, taskMapper_}
    , allTags{taskStorage_}
    , requestSprints{sprintStorage_, sprintMapper_}
    , finishedTasks{taskStorage_, taskMapper_}
    , sprintsForTask{taskStorage_, tagMapper_}
    , operationalRange{operationRangeReader_}
    , dailyDistribution{dailyDistReader_}
    , mondayFirstWeeklyDistribution{mondayFirstWeeklyDistReader_}
    , sundayFirstWeeklyDistribution{sundayFirstWeeklyDistReader_}
    , monthlyDistribution{monthlyDistReader_}
    , workSchedule{workScheduleReader_}
    , sprintStatistics{sprintStorage_}
    , workdaysStatistics{sprintStatistics}
    , readTaskTree{taskStorage_, taskTreeMapper_}
{
}

asp::QueryHandler<api::ActiveTasksQuery>&
TestQueryHandlerComposer::activeTasksHandler()
{
    return activeTasks;
}

asp::QueryHandler<api::AllTagsQuery>& TestQueryHandlerComposer::allTagsHandler()
{
    return allTags;
}

asp::QueryHandler<api::RequestSprintsQuery>&
TestQueryHandlerComposer::requestSprintsHandler()
{
    return requestSprints;
}

asp::QueryHandler<api::FinishedTasksQuery>&
TestQueryHandlerComposer::finishedTasksHandler()
{
    return finishedTasks;
}

asp::QueryHandler<api::SprintsForTaskQuery>&
TestQueryHandlerComposer::sprintsForTaskHandler()
{
    return sprintsForTask;
}

asp::QueryHandler<api::OperationalRangeQuery>&
TestQueryHandlerComposer::operationalRangeHandler()
{
    return operationalRange;
}

asp::QueryHandler<api::RequestSprintDistributionQuery>&
TestQueryHandlerComposer::dailyDistHandler()
{
    return dailyDistribution;
}

asp::QueryHandler<api::RequestSprintDistributionQuery>&
TestQueryHandlerComposer::weeklyDistHandler(dw::Weekday firstDayOfWeek)
{
    if (firstDayOfWeek == dw::Weekday::Monday) {
        return mondayFirstWeeklyDistribution;
    }
    return sundayFirstWeeklyDistribution;
}

asp::QueryHandler<api::RequestSprintDistributionQuery>&
TestQueryHandlerComposer::monthlyDistHandler()
{
    return monthlyDistribution;
}

asp::QueryHandler<api::WorkScheduleQuery>&
TestQueryHandlerComposer::workScheduleHandler()
{
    return workSchedule;
}

asp::QueryHandler<api::SprintStatisticsQuery>&
TestQueryHandlerComposer::sprintStatisticsHandler()
{
    return sprintStatistics;
}

asp::QueryHandler<api::WorkdayStatisticsQuery>&
TestQueryHandlerComposer::workdayStatisticsHandler()
{
    return workdaysStatistics;
}

asp::QueryHandler<api::WorktimeStatisticsQuery>&
TestQueryHandlerComposer::worktimeStatisticsHandler()
{
    return worktimeStatistics;
}

asp::QueryHandler<api::DailyStatisticsQuery>&
TestQueryHandlerComposer::dailyStatisticsHandler()
{
    return dailyStatistics;
}

asp::QueryHandler<api::TopTagFrequenciesQuery>&
TestQueryHandlerComposer::topTagFrequenciesHandler()
{
    return topTagFrequencies;
}

asp::QueryHandler<api::ReadTaskTreeQuery>&
TestQueryHandlerComposer::readTaskTreeHandler()
{
    return readTaskTree;
}

} // namespace sprint_timer::compose

