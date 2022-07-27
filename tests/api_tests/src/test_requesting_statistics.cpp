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
using namespace sprint_timer;
using namespace dw;
using namespace std::chrono_literals;

void SprintStatisticsDTOEquals(const SprintStatisticsDTO& expected,
                               const SprintStatisticsDTO& actual)
{
    using namespace testing;
    EXPECT_EQ(expected.numTopTags, actual.numTopTags);
    EXPECT_THAT(actual.allSprints,
                UnorderedElementsAreArray(expected.allSprints));
    // EXPECT_EQ(expected.allSprints, actual.allSprints);
    ASSERT_EQ(expected.data.size(), actual.data.size());

    for (size_t i = 0; i < expected.data.size(); ++i) {
        EXPECT_EQ(expected.data[i].tag, actual.data[i].tag);
        EXPECT_THAT(actual.data[i].frequency,
                    DoubleNear(expected.data[i].frequency, 0.001));
        EXPECT_THAT(actual.data[i].sprints,
                    UnorderedElementsAreArray(actual.data[i].sprints));
    }
}

class RequestingSprintStatisticsFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    compose::CommandHandlerComposer& commandHandlerComposer{
        initializer.commandHandlerComposer()};
    compose::QueryHandlerComposer& queryHandlerComposer{
        initializer.queryHandlerComposer()};
    asp::QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryHandlerComposer.activeTasksHandler()};
    asp::CommandHandler<RegisterSprintBulkCommand>& registerSprintsHandler{
        commandHandlerComposer.registerSprintBulkHandler()};
    asp::CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandHandlerComposer.createTaskHandler()};
    asp::QueryHandler<SprintsForTaskQuery>& sprintsForTaskHandler{
        queryHandlerComposer.sprintsForTaskHandler()};
    asp::CommandHandler<DeleteSprintCommand>& deleteSprintHandler{
        commandHandlerComposer.deleteSprintHandler()};
    asp::QueryHandler<SprintStatisticsQuery>& sprintStatisticsHandler{
        queryHandlerComposer.sprintStatisticsHandler()};
};

TEST_F(RequestingSprintStatisticsFixture, returns_statistics)
{
    const auto dateTimeRange = fixtures::givenStatisticsDataFixtureLoaded(
        createTaskHandler, registerSprintsHandler);
    const auto initialDateTime = dateTimeRange.start();

    std::vector<TaggedSprintsDTO> taggedSprints{
        {"Tag1",
         0.303,
         {createRange(initialDateTime + 1h),
          createRange(initialDateTime + Days{3} + 2h),
          createRange(initialDateTime + Days{3} + 7h),
          createRange(initialDateTime + Days{3} + 10h),
          createRange(initialDateTime + Days{4} + 9h),
          createRange(initialDateTime + Days{6} + 5h),
          createRange(initialDateTime + Days{7} + 7h),
          createRange(initialDateTime + Days{7} + 13h),
          createRange(initialDateTime + Days{7} + 20h),
          createRange(initialDateTime + Days{10} + 6h)}},
        {"Tag2",
         0.242,
         {createRange(initialDateTime + 1h),
          createRange(initialDateTime + Days{3} + 7h),
          createRange(initialDateTime + Days{4} + 18h),
          createRange(initialDateTime + Days{6} + 5h),
          createRange(initialDateTime + Days{7} + 7h),
          createRange(initialDateTime + Days{7} + 20h),
          createRange(initialDateTime + Days{10} + 5h),
          createRange(initialDateTime + Days{10} + 6h)}},
        {"Tag4",
         0.212,
         {createRange(initialDateTime + 1h),
          createRange(initialDateTime + Days{3} + 7h),
          createRange(initialDateTime + Days{6} + 5h),
          createRange(initialDateTime + Days{7} + 7h),
          createRange(initialDateTime + Days{7} + 20h),
          createRange(initialDateTime + Days{10} + 6h)}},
        {"",
         0.243,
         {createRange(initialDateTime + 12h),
          createRange(initialDateTime + 20h),
          createRange(initialDateTime + Days{2} + 10h),
          createRange(initialDateTime + Days{2} + 17h),
          createRange(initialDateTime + Days{9} + 21h),
          createRange(initialDateTime + Days{3} + 12h),
          createRange(initialDateTime + Days{5} + 4h),
          createRange(initialDateTime + Days{5} + 19h)}}};

    const size_t numTopTags{4};

    std::vector<dw::DateTimeRange> allSprints{
        createRange(initialDateTime + 1h),
        createRange(initialDateTime + 12h),
        createRange(initialDateTime + 20h),
        createRange(initialDateTime + Days{2} + 10h),
        createRange(initialDateTime + Days{2} + 17h),
        createRange(initialDateTime + Days{3} + 10h),
        createRange(initialDateTime + Days{3} + 12h),
        createRange(initialDateTime + Days{3} + 23h),
        createRange(initialDateTime + Days{3} + 2h),
        createRange(initialDateTime + Days{3} + 7h),
        createRange(initialDateTime + Days{4} + 9h),
        createRange(initialDateTime + Days{4} + 18h),
        createRange(initialDateTime + Days{5} + 4h),
        createRange(initialDateTime + Days{5} + 7h),
        createRange(initialDateTime + Days{5} + 19h),
        createRange(initialDateTime + Days{6} + 5h),
        createRange(initialDateTime + Days{7} + 7h),
        createRange(initialDateTime + Days{7} + 13h),
        createRange(initialDateTime + Days{7} + 20h),
        createRange(initialDateTime + Days{8} + 14h),
        createRange(initialDateTime + Days{9} + 16h),
        createRange(initialDateTime + Days{9} + 21h),
        createRange(initialDateTime + Days{10} + 5h),
        createRange(initialDateTime + Days{10} + 6h)};
    SprintStatisticsDTO expected{
        numTopTags, std::move(allSprints), std::move(taggedSprints)};

    ASSERT_FALSE(
        sprintStatisticsHandler.handle(api::SprintStatisticsQuery{
            numTopTags,
            dw::DateRange{(initialDateTime - Days{30}).date(),
                          (initialDateTime - Days{15}).date()}}));
    SprintStatisticsDTOEquals(
        expected,
        *sprintStatisticsHandler.handle(api::SprintStatisticsQuery{
            numTopTags,
            dw::DateRange{initialDateTime.date(),
                          (initialDateTime + Days{10}).date()}}));
}
