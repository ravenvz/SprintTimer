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
#include "api_tests/QtStorageInitializer.h"
#include "api_tests/fixtures/StatisticsDataFixture.h"
#include "api_tests/utils/utils.h"
#include "gtest/gtest.h"

using namespace sprint_timer;
using namespace sprint_timer::api;
using namespace sprint_timer::entities;
using namespace dw;
using namespace std::chrono_literals;

class RequestingDailyStatisticsFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    compose::CommandHandlerComposer& commandHandlerComposer{
        initializer.commandHandlerComposer()};
    compose::QueryHandlerComposer& queryHandlerComposer{
        initializer.queryHandlerComposer()};
    asp::CommandHandler<RegisterSprintBulkCommand>& registerSprintsHandler{
        commandHandlerComposer.registerSprintBulkHandler()};
    asp::CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandHandlerComposer.createTaskHandler()};
    asp::QueryHandler<SprintStatisticsQuery>& sprintStatisticsHandler{
        queryHandlerComposer.sprintStatisticsHandler()};
    asp::QueryHandler<DailyStatisticsQuery>& dailyStatisticsHandler{
        queryHandlerComposer.dailyStatisticsHandler()};
    asp::CommandHandler<ChangeWorkScheduleCommand>& changeWorkScheduleHandler{
        commandHandlerComposer.changeWorkScheduleHandler()};
};

/*  Creates sprints with following distribution
 * in date range {15.01.2022, 15.01.2022 + 10 days}.
 *
 * Function returns this datetime range for convinience.
 *
 *  1   0   0   3   1   0   1   3   0   0   1
 * Sat Sun Mon Tue Wed Thu Fri Sat Sun Mon Tue
 *
 */

void expectDailyStatisticsEquals(const DailyStatisticsDTO& expected,
                                 const DailyStatisticsDTO& actual)
{
    using namespace testing;
    EXPECT_THAT(actual.averageSprintsPerWorkday,
                DoubleNear(actual.averageSprintsPerWorkday, 0.01));
    EXPECT_THAT(actual.expectedSprintsPerWorkday,
                DoubleNear(actual.expectedSprintsPerWorkday, 0.01));
    EXPECT_EQ(expected.distribution, actual.distribution);
}

TEST_F(RequestingDailyStatisticsFixture, returns_daily_statistics)
{
    const auto dateTimeRange = fixtures::givenStatisticsDataFixtureLoaded(
        createTaskHandler, registerSprintsHandler);
    const auto initialDateTime = dateTimeRange.start();
    const auto enclosingDateRange = dw::DateRange{
        initialDateTime.date(), (initialDateTime + Days{10}).date()};

    WorkSchedule oldSchedule;
    WorkSchedule workSchedule;
    workSchedule.addWeekSchedule(initialDateTime.date() - Days{100},
                                 WeekSchedule{{12, 12, 12, 12, 12, 0, 0}});
    changeWorkScheduleHandler.handle(ChangeWorkScheduleCommand{
        .oldSchedule = oldSchedule, .newSchedule = workSchedule});
    const double expectedSprintsPerWorkday{84.0};

    // No data in this date range
    expectDailyStatisticsEquals(
        DailyStatisticsDTO{0.0,
                           expectedSprintsPerWorkday,
                           0,
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        dailyStatisticsHandler.handle(DailyStatisticsQuery{
            4,
            std::nullopt,
            dw::DateRange{(initialDateTime - Days{11}).date(),
                          (initialDateTime - Days{1}).date()}}));
    // No filtering by tag
    expectDailyStatisticsEquals(
        DailyStatisticsDTO{3.43,
                           expectedSprintsPerWorkday,
                           24,
                           {3, 0, 2, 5, 2, 3, 1, 3, 1, 2, 2}},
        dailyStatisticsHandler.handle(
            DailyStatisticsQuery{4, std::nullopt, enclosingDateRange}));
    // Filter by 0-th top tag
    expectDailyStatisticsEquals(
        DailyStatisticsDTO{1.28,
                           expectedSprintsPerWorkday,
                           10,
                           {1, 0, 0, 3, 1, 0, 1, 3, 0, 0, 1}},
        dailyStatisticsHandler.handle(
            DailyStatisticsQuery{4, 0, enclosingDateRange}));
    // Filter by other combined tags
    expectDailyStatisticsEquals(
        DailyStatisticsDTO{1.14,
                           expectedSprintsPerWorkday,
                           8,
                           {2, 0, 2, 1, 0, 2, 0, 0, 0, 1, 0}},
        dailyStatisticsHandler.handle(
            DailyStatisticsQuery{4, 3, enclosingDateRange}));
}
