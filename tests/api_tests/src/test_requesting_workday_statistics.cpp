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

class RequestingWorkdayStatisticsFixture : public ::testing::Test {
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
    asp::QueryHandler<WorkdayStatisticsQuery>& workdayStatisticsHandler{
        queryHandlerComposer.workdayStatisticsHandler()};
};

/*  3   0   2   5   2   3   1   3   1   2   2
 * Sat Sun Mon Tue Wed Thu Fri Sat Sun Mon Tue

 * 2.0 3.5 2.0 3.0 1.0 3.0 0.5
 * Mon Tue Wed Thu Fri Sat Sun
 */

void ExpectWorkdayStatisticsEquals(const WorkdayStatisticsDTO& expected,
                                   const WorkdayStatisticsDTO& actual)
{
    using namespace testing;
    EXPECT_EQ(expected.bestWorkday, actual.bestWorkday);
    EXPECT_EQ(expected.percentageAboveAverage, actual.percentageAboveAverage);
    for (size_t i = 0; i < 7; ++i) {
        EXPECT_THAT(actual.sprintDistribution[i],
                    DoubleNear(expected.sprintDistribution[i], 0.01));
    }
}

TEST_F(RequestingWorkdayStatisticsFixture, returns_workday_statistics)
{
    const auto dateTimeRange = fixtures::givenStatisticsDataFixtureLoaded(
        createTaskHandler, registerSprintsHandler);
    const auto initialDateTime = dateTimeRange.start();
    const auto enclosingDateRange = dw::DateRange{
        initialDateTime.date(), (initialDateTime + Days{10}).date()};

    // Out of date range (no data)
    EXPECT_EQ(std::nullopt,
              workdayStatisticsHandler.handle(WorkdayStatisticsQuery{
                  4,
                  std::nullopt,
                  dw::DateRange{(initialDateTime - Days{30}).date(),
                                (initialDateTime - Days{15}).date()}}));
    // Unfiltered by tag
    ExpectWorkdayStatisticsEquals(
        WorkdayStatisticsDTO{
            {2.0, 3.5, 2.0, 3.0, 1.0, 3.0, 0.5}, dw::Weekday::Tuesday, 63},
        *workdayStatisticsHandler.handle(
            WorkdayStatisticsQuery{4, std::nullopt, enclosingDateRange}));
    // 1-st tag from top
    ExpectWorkdayStatisticsEquals(
        WorkdayStatisticsDTO{
            {0.0, 2.0, 1.0, 0.0, 1.0, 2.0, 0.0}, dw::Weekday::Tuesday, 133},
        *workdayStatisticsHandler.handle(
            WorkdayStatisticsQuery{4, 0, enclosingDateRange}));
    // 2-nd tag from top
    ExpectWorkdayStatisticsEquals(
        WorkdayStatisticsDTO{
            {0.0, 1.5, 1.0, 0.0, 1.0, 1.5, 0.0}, dw::Weekday::Tuesday, 110},
        *workdayStatisticsHandler.handle(
            WorkdayStatisticsQuery{4, 1, enclosingDateRange}));
    // 3-rd tag from top
    ExpectWorkdayStatisticsEquals(
        WorkdayStatisticsDTO{
            {0.5, 1.0, 0.0, 3.0, 0.0, 0.0, 0.5}, dw::Weekday::Thursday, 319},
        *workdayStatisticsHandler.handle(
            WorkdayStatisticsQuery{4, 2, enclosingDateRange}));
    // 4-th tag from top (combined other tags)
    ExpectWorkdayStatisticsEquals(
        WorkdayStatisticsDTO{
            {1.5, 0.5, 0.0, 2.0, 0.0, 1.0, 0.0}, dw::Weekday::Thursday, 179},
        *workdayStatisticsHandler.handle(
            WorkdayStatisticsQuery{4, 3, enclosingDateRange}));
    // Tag greater than numTopTags returns empty dataset
    EXPECT_EQ(std::nullopt,
              workdayStatisticsHandler.handle(
                  WorkdayStatisticsQuery{4, 4, enclosingDateRange}));
}
