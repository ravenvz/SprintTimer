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
using namespace std::chrono_literals;

class RequestingWorktimeStatisticsFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    compose::CommandHandlerComposer& commandHandlerComposer{
        initializer.commandHandlerComposer()};
    compose::QueryHandlerComposer& queryHandlerComposer{
        initializer.queryHandlerComposer()};
    asp::CommandHandler<RegisterSprintBulkCommand>& registerSprintsHandler{
        initializer.commandHandlerComposer().registerSprintBulkHandler()};
    asp::CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandHandlerComposer.createTaskHandler()};
    asp::QueryHandler<SprintStatisticsQuery>& sprintStatisticsHandler{
        queryHandlerComposer.sprintStatisticsHandler()};
    asp::QueryHandler<WorktimeStatisticsQuery>& worktimeStatisticsHandler{
        queryHandlerComposer.worktimeStatisticsHandler()};
};

/*  Creates sprints with following distribution
 * in date range {15.01.2022, 15.01.2022 + 10 days}.
 *
 * Function returns this datetime range for convinience.
 *
 *  3   0   2   5   2   3   1   3   1   2   2
 * Sat Sun Mon Tue Wed Thu Fri Sat Sun Mon Tue
 *
 * Represent a day partition
 *
 * Day has 6 4-hour parts:
 *      Midnight  22:00 - 2:00   0
 *      Night      2:00 - 6:00   0
 *      Morning    6:00 - 10:00  0
 *      Noon      10:00 - 14:00  0
 *      Afternoon 14:00 - 18:00  0
 *      Evening   18:00 - 22:00  0
 */
void expectWorktimeStatisticsEquals(const WorktimeStatisticsDTO& expected,
                                    const WorktimeStatisticsDTO& actual)
{
    using namespace testing;
    EXPECT_EQ(expected.bestWorktime, actual.bestWorktime);
    EXPECT_THAT(actual.timeRanges,
                UnorderedElementsAreArray(expected.timeRanges));
}

TEST_F(RequestingWorktimeStatisticsFixture, returnsWorktimeStatistics)
{
    using namespace dw;
    const auto dateTimeRange = fixtures::givenStatisticsDataFixtureLoaded(
        createTaskHandler, registerSprintsHandler);
    const auto initialDateTime = dateTimeRange.start();
    const auto enclosingDateRange = dw::DateRange{
        initialDateTime.date(), (initialDateTime + Days{10}).date()};

    // No data in this date range
    EXPECT_EQ(std::nullopt,
              worktimeStatisticsHandler.handle(WorktimeStatisticsQuery{
                  4,
                  std::nullopt,
                  dw::DateRange{(initialDateTime - Days{50}).date(),
                                (initialDateTime - Days{30}).date()}}));
    // No tag filtering
    expectWorktimeStatisticsEquals(
        WorktimeStatisticsDTO{{createRange(initialDateTime + Days{3} + 10h),
                               createRange(initialDateTime + Days{3} + 2h),
                               createRange(initialDateTime + Days{4} + 9h),
                               createRange(initialDateTime + Days{7} + 13h),
                               createRange(initialDateTime + Days{4} + 18h),
                               createRange(initialDateTime + Days{10} + 5h),
                               createRange(initialDateTime + 12h),
                               createRange(initialDateTime + 20h),
                               createRange(initialDateTime + Days{2} + 10h),
                               createRange(initialDateTime + Days{2} + 17h),
                               createRange(initialDateTime + Days{9} + 21h),
                               createRange(initialDateTime + 1h),
                               createRange(initialDateTime + Days{3} + 7h),
                               createRange(initialDateTime + Days{6} + 5h),
                               createRange(initialDateTime + Days{7} + 7h),
                               createRange(initialDateTime + Days{7} + 20h),
                               createRange(initialDateTime + Days{10} + 6h),
                               createRange(initialDateTime + Days{3} + 23h),
                               createRange(initialDateTime + Days{5} + 7h),
                               createRange(initialDateTime + Days{8} + 14h),
                               createRange(initialDateTime + Days{9} + 16h),
                               createRange(initialDateTime + Days{3} + 12h),
                               createRange(initialDateTime + Days{5} + 4h),
                               createRange(initialDateTime + Days{5} + 19h)},
                              DayPart::Morning},
        *worktimeStatisticsHandler.handle(
            WorktimeStatisticsQuery{4, std::nullopt, enclosingDateRange}));
    // Filtered by 0-th top tag
    expectWorktimeStatisticsEquals(
        WorktimeStatisticsDTO{{createRange(initialDateTime + Days{3} + 10h),
                               createRange(initialDateTime + Days{3} + 2h),
                               createRange(initialDateTime + Days{4} + 9h),
                               createRange(initialDateTime + Days{7} + 13h),
                               createRange(initialDateTime + 1h),
                               createRange(initialDateTime + Days{3} + 7h),
                               createRange(initialDateTime + Days{6} + 5h),
                               createRange(initialDateTime + Days{7} + 7h),
                               createRange(initialDateTime + Days{7} + 20h),
                               createRange(initialDateTime + Days{10} + 6h)},
                              DayPart::Morning},
        *worktimeStatisticsHandler.handle(
            WorktimeStatisticsQuery{4, 0, enclosingDateRange}));
    // Filtered by n-th top tag (compressed others)
    expectWorktimeStatisticsEquals(
        WorktimeStatisticsDTO{{createRange(initialDateTime + 12h),
                               createRange(initialDateTime + 20h),
                               createRange(initialDateTime + Days{2} + 10h),
                               createRange(initialDateTime + Days{2} + 17h),
                               createRange(initialDateTime + Days{9} + 21h),
                               createRange(initialDateTime + Days{3} + 12h),
                               createRange(initialDateTime + Days{5} + 4h),
                               createRange(initialDateTime + Days{5} + 19h)},
                              DayPart::Evening},
        *worktimeStatisticsHandler.handle(
            WorktimeStatisticsQuery{4, 3, enclosingDateRange}));
}
