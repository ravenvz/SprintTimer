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

#include "api/handlers/ActiveTasksHandler.h"
#include "api/handlers/AllTagsHandler.h"
#include "api/handlers/DailyStatisticsHandler.h"
#include "api/handlers/FinishedTasksHandler.h"
#include "api/handlers/OperationalRangeHandler.h"
#include "api/handlers/ReadTaskTreeHandler.h"
#include "api/handlers/RequestSprintDistributionHandler.h"
#include "api/handlers/RequestSprintsHandler.h"
#include "api/handlers/SprintStatisticsHandler.h"
#include "api/handlers/SprintsForTaskHandler.h"
#include "api/handlers/TopTagFrequenciesHandler.h"
#include "api/handlers/WorkScheduleHandler.h"
#include "api/handlers/WorkdayStatisticsHandler.h"
#include "api/handlers/WorktimeStatisticsHandler.h"
#include "api_tests/QueryHandlerComposer.h"
#include "api/OperationalRangeReader.h"
#include "api/SprintDistributionReader.h"
#include "api/SprintStorage.h"
#include "api/TaskStorage.h"
#include "core/TaskTreeMetadataReader.h"

namespace sprint_timer::compose {

struct TestQueryHandlerComposer final : public QueryHandlerComposer {
    TestQueryHandlerComposer(
        TaskStorage& taskStorage_,
        SprintStorage& sprintStorage_,
        OperationalRangeReader& operationRangeReader_,
        WorkScheduleReader& workScheduleReader_,
        SprintDistributionReader& dailyDistReader_,
        SprintDistributionReader& mondayFirstWeeklyDistReader_,
        SprintDistributionReader& sundayFirstWeeklyDistReader_,
        SprintDistributionReader& monthlyDistReader_,
        TaskTreeMetadataReader& taskTreeMetadataStorage_);

    asp::QueryHandler<api::ActiveTasksQuery>& activeTasksHandler() override;

    asp::QueryHandler<api::AllTagsQuery>& allTagsHandler() override;

    asp::QueryHandler<api::RequestSprintsQuery>&
    requestSprintsHandler() override;

    asp::QueryHandler<api::FinishedTasksQuery>& finishedTasksHandler() override;

    asp::QueryHandler<api::SprintsForTaskQuery>&
    sprintsForTaskHandler() override;

    asp::QueryHandler<api::OperationalRangeQuery>&
    operationalRangeHandler() override;

    asp::QueryHandler<api::RequestSprintDistributionQuery>&
    dailyDistHandler() override;

    asp::QueryHandler<api::RequestSprintDistributionQuery>&
    weeklyDistHandler(dw::Weekday firstDayOfWeek) override;

    asp::QueryHandler<api::RequestSprintDistributionQuery>&
    monthlyDistHandler() override;

    asp::QueryHandler<api::WorkScheduleQuery>& workScheduleHandler() override;

    asp::QueryHandler<api::SprintStatisticsQuery>&
    sprintStatisticsHandler() override;

    asp::QueryHandler<api::WorkdayStatisticsQuery>&
    workdayStatisticsHandler() override;

    asp::QueryHandler<api::WorktimeStatisticsQuery>&
    worktimeStatisticsHandler() override;

    asp::QueryHandler<api::DailyStatisticsQuery>&
    dailyStatisticsHandler() override;

    asp::QueryHandler<api::TopTagFrequenciesQuery>&
    topTagFrequenciesHandler() override;

    asp::QueryHandler<api::ReadTaskTreeQuery>& readTaskTreeHandler() override;

private:
    api::ActiveTasksHandler activeTasks;
    api::AllTagsHandler allTags;
    api::RequestSprintsHandler requestSprints;
    api::FinishedTasksHandler finishedTasks;
    api::SprintsForTaskHandler sprintsForTask;
    api::OperationalRangeHandler operationalRange;
    api::RequestSprintDistributionHandler dailyDistribution;
    api::RequestSprintDistributionHandler mondayFirstWeeklyDistribution;
    api::RequestSprintDistributionHandler sundayFirstWeeklyDistribution;
    api::RequestSprintDistributionHandler monthlyDistribution;
    api::WorkScheduleHandler workSchedule;
    api::SprintStatisticsHandler sprintStatistics;
    api::WorkdayStatisticsHandler workdaysStatistics{sprintStatistics};
    api::WorktimeStatisticsHandler worktimeStatistics{sprintStatistics};
    api::DailyStatisticsHandler dailyStatistics{workSchedule, sprintStatistics};
    api::TopTagFrequenciesHandler topTagFrequencies{sprintStatistics};
    api::ReadTaskTreeHandler readTaskTree;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: TESTQUERYHANDLERCOMPOSER_H_UAVBEGPJ */
