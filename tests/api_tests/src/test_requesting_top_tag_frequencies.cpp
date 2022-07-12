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

class RequestingTopTagFrequenciesFixture : public ::testing::Test {
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
    asp::QueryHandler<TopTagFrequenciesQuery>& topTagFrequenciesHandler{
        queryHandlerComposer.topTagFrequenciesHandler()};
};

void expectTopTagFrequenciesDTOEquals(const TagFrequencyDTO& expected,
                                      const TagFrequencyDTO& actual)
{
    using namespace ::testing;
    EXPECT_THAT(actual.frequency, DoubleNear(expected.frequency, 0.01));
    EXPECT_EQ(expected.tag, actual.tag);
}

void expectTagFrequenciesEquals(const std::vector<TagFrequencyDTO>& expected,
                                const std::vector<TagFrequencyDTO>& actual)
{
    ASSERT_EQ(expected.size(), actual.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        expectTopTagFrequenciesDTOEquals(expected[i], actual[i]);
    }
}

TEST_F(RequestingTopTagFrequenciesFixture, returns_top_tag_frequencies)
{
    using namespace dw;
    const auto dateTimeRange = fixtures::givenStatisticsDataFixtureLoaded(
        createTaskHandler, registerSprintsHandler);
    const auto initialDateTime = dateTimeRange.start();
    const auto enclosingDateRange = dw::DateRange{
        initialDateTime.date(), (initialDateTime + Days{10}).date()};
    const std::vector<TagFrequencyDTO> expected{TagFrequencyDTO{0.303, "Tag1"},
                                                TagFrequencyDTO{0.242, "Tag2"},
                                                TagFrequencyDTO{0.212, "Tag4"},
                                                TagFrequencyDTO{0.243, ""}};

    // No data in given range.
    EXPECT_EQ(std::nullopt,
              topTagFrequenciesHandler.handle(TopTagFrequenciesQuery{
                  4,
                  dw::DateRange{(initialDateTime - Days{50}).date(),
                                (initialDateTime - Days{30}).date()}}));
    const auto actual = topTagFrequenciesHandler.handle(
        TopTagFrequenciesQuery{4, enclosingDateRange});

    ASSERT_TRUE(actual);
    expectTagFrequenciesEquals(expected, *actual);
}
