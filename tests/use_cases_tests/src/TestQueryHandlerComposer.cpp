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
#include "use_cases_tests/TestQueryHandlerComposer.h"

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
    TaskTreeMetadataReader& taskTreeMetadataStorage_)
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
    , taskBacklog{taskStorage_}
    , sprintStatistics{sprintStorage_}
    , workdaysStatistics{sprintStatistics}
    , readTaskTree{taskStorage_, taskTreeMetadataStorage_}
{
}

QueryHandler<use_cases::ActiveTasksQuery>&
TestQueryHandlerComposer::activeTasksHandler()
{
    return activeTasks;
}

QueryHandler<use_cases::AllTagsQuery>&
TestQueryHandlerComposer::allTagsHandler()
{
    return allTags;
}

QueryHandler<use_cases::RequestSprintsQuery>&
TestQueryHandlerComposer::requestSprintsHandler()
{
    return requestSprints;
}

QueryHandler<use_cases::FinishedTasksQuery>&
TestQueryHandlerComposer::finishedTasksHandler()
{
    return finishedTasks;
}

QueryHandler<use_cases::SprintsForTaskQuery>&
TestQueryHandlerComposer::sprintsForTaskHandler()
{
    return sprintsForTask;
}

QueryHandler<use_cases::OperationalRangeQuery>&
TestQueryHandlerComposer::operationalRangeHandler()
{
    return operationalRange;
}

QueryHandler<use_cases::RequestSprintDistributionQuery>&
TestQueryHandlerComposer::dailyDistHandler()
{
    return dailyDistribution;
}

QueryHandler<use_cases::RequestSprintDistributionQuery>&
TestQueryHandlerComposer::weeklyDistHandler(dw::Weekday firstDayOfWeek)
{
    if (firstDayOfWeek == dw::Weekday::Monday) {
        return mondayFirstWeeklyDistribution;
    }
    return sundayFirstWeeklyDistribution;
}

QueryHandler<use_cases::RequestSprintDistributionQuery>&
TestQueryHandlerComposer::monthlyDistHandler()
{
    return monthlyDistribution;
}

QueryHandler<use_cases::WorkScheduleQuery>&
TestQueryHandlerComposer::workScheduleHandler()
{
    return workSchedule;
}

QueryHandler<use_cases::TaskBacklogQuery>&
TestQueryHandlerComposer::taskBacklogHandler()
{
    return taskBacklog;
}

QueryHandler<use_cases::SprintStatisticsQuery>&
TestQueryHandlerComposer::sprintStatisticsHandler()
{
    return sprintStatistics;
}

QueryHandler<use_cases::WorkdayStatisticsQuery>&
TestQueryHandlerComposer::workdayStatisticsHandler()
{
    return workdaysStatistics;
}

QueryHandler<use_cases::WorktimeStatisticsQuery>&
TestQueryHandlerComposer::worktimeStatisticsHandler()
{
    return worktimeStatistics;
}

QueryHandler<use_cases::DailyStatisticsQuery>&
TestQueryHandlerComposer::dailyStatisticsHandler()
{
    return dailyStatistics;
}

QueryHandler<use_cases::TopTagFrequenciesQuery>&
TestQueryHandlerComposer::topTagFrequenciesHandler()
{
    return topTagFrequencies;
}

QueryHandler<use_cases::ReadTaskTreeQuery>&
TestQueryHandlerComposer::readTaskTreeHandler()
{
    return readTaskTree;
}

} // namespace sprint_timer::compose

