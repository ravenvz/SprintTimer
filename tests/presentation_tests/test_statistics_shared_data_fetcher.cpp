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
#include "core/ThreadPoolQueryHandler.h"
#include "mocks/QueryHandlerMock.h"
#include "mocks/StatisticsColleagueMock.h"
#include "qt_gui/presentation/StatisticsSharedDataFetcher.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::ByMove;
using ::testing::NiceMock;
using ::testing::Return;

namespace sprint_timer::ui {

bool operator==(const StatisticsContext& lhs, const StatisticsContext& rhs)
{
    return lhs.sprints() == rhs.sprints() &&
           lhs.tagFrequencies() == rhs.tagFrequencies() &&
           lhs.currentRange() == rhs.currentRange() &&
           lhs.selectedTag() == rhs.selectedTag();
}

} // namespace sprint_timer::ui

class StatisticsSharedDataFetcherFixture : public ::testing::Test {
public:
    NiceMock<mocks::AsyncQueryHandlerMock<
        sprint_timer::use_cases::RequestSprintsQuery>>
        requestSprintsHandler;
    NiceMock<mocks::ColleagueMock> fakeColleague;
    sprint_timer::ui::StatisticsMediator statisticsMediator;
    dw::DateRange someDateRange{dw::current_date(), dw::current_date()};
    size_t numTopTags{5};
};

TEST_F(StatisticsSharedDataFetcherFixture,
       does_not_fetch_data_when_range_is_not_set)
{
    sprint_timer::ui::StatisticsContext context;
    sprint_timer::ui::StatisticsSharedDataFetcher sut{
        requestSprintsHandler, statisticsMediator, context, numTopTags};

    EXPECT_CALL(requestSprintsHandler, handleAwaitImpl(_)).Times(0);

    sut.fetchData();
}

TEST_F(StatisticsSharedDataFetcherFixture, updates_context_when_range_is_set)
{
    using sprint_timer::use_cases::SprintDTO;
    sprint_timer::ui::StatisticsContext context;
    sprint_timer::ui::StatisticsSharedDataFetcher sut{
        requestSprintsHandler, statisticsMediator, context, numTopTags};
    std::vector<SprintDTO> sprints;
    std::promise<std::vector<SprintDTO>> p;
    p.set_value(sprints);
    ON_CALL(requestSprintsHandler, handleAwaitImpl(_))
        .WillByDefault(Return(ByMove(p.get_future())));
    statisticsMediator.onRangeChanged(someDateRange);
    const sprint_timer::ui::StatisticsContext expected{
        sprints, someDateRange, numTopTags};

    sut.fetchData();
    sut.updateView();

    EXPECT_EQ(expected, context);
}

TEST_F(StatisticsSharedDataFetcherFixture, notifies_mediator_when_data_is_ready)
{
    using sprint_timer::use_cases::SprintDTO;
    statisticsMediator.addColleague(&fakeColleague);
    std::vector<SprintDTO> sprints;
    sprint_timer::ui::StatisticsContext context{
        sprints, someDateRange, numTopTags};
    sprint_timer::ui::StatisticsSharedDataFetcher sut{
        requestSprintsHandler, statisticsMediator, context, numTopTags};
    std::promise<std::vector<SprintDTO>> p;
    p.set_value(sprints);
    ON_CALL(requestSprintsHandler, handleAwaitImpl(_))
        .WillByDefault(Return(ByMove(p.get_future())));

    EXPECT_CALL(fakeColleague, onSharedDataChanged());

    sut.fetchData();
    sut.updateView();
}

// TEST_F(StatisticsSharedDataFetcherFixture, notifies_mediator_when_) { }
