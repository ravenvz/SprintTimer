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

using sprint_timer::TagTop;
using sprint_timer::entities::Sprint;
using ::testing::_;
using ::testing::ByMove;
using ::testing::NiceMock;
using ::testing::Return;

namespace sprint_timer::ui {

bool operator==(const StatisticsContext& lhs, const StatisticsContext& rhs)
{
    return lhs.sprintIntervals() == rhs.sprintIntervals() &&
           lhs.tagFrequencies() == rhs.tagFrequencies() &&
           lhs.currentRange() == rhs.currentRange() &&
           lhs.selectedTag() == rhs.selectedTag();
}

} // namespace sprint_timer::ui

class StatisticsSharedDataFetcherFixture : public ::testing::Test {
public:
    NiceMock<
        mocks::QueryHandlerMock<sprint_timer::use_cases::SprintStatisticsQuery>>
        sprintStatisticsHandler;
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
        sprintStatisticsHandler, statisticsMediator, context, numTopTags};

    EXPECT_CALL(sprintStatisticsHandler, handle(_)).Times(0);

    sut.fetchData();
}

TEST_F(StatisticsSharedDataFetcherFixture, updates_context_when_range_is_set)
{
    using sprint_timer::use_cases::SprintStatisticsDTO;
    sprint_timer::ui::StatisticsContext context;
    sprint_timer::ui::StatisticsSharedDataFetcher sut{
        sprintStatisticsHandler, statisticsMediator, context, numTopTags};
    SprintStatisticsDTO sprintStatistics;
    mocks::given_handler_returns(sprintStatisticsHandler, sprintStatistics);
    statisticsMediator.onRangeChanged(someDateRange);
    const sprint_timer::ui::StatisticsContext expected{SprintStatisticsDTO{},
                                                       someDateRange};

    sut.fetchData();
    sut.updateView();

    EXPECT_EQ(expected, context);
}

TEST_F(StatisticsSharedDataFetcherFixture, notifies_mediator_when_data_is_ready)
{
    using sprint_timer::use_cases::SprintStatisticsDTO;
    statisticsMediator.addColleague(&fakeColleague);
    sprint_timer::ui::StatisticsContext context{SprintStatisticsDTO{},
                                                someDateRange};
    sprint_timer::ui::StatisticsSharedDataFetcher sut{
        sprintStatisticsHandler, statisticsMediator, context, numTopTags};
    mocks::given_handler_returns(sprintStatisticsHandler,
                                 SprintStatisticsDTO{});

    EXPECT_CALL(fakeColleague, onSharedDataChanged());

    sut.fetchData();
    sut.updateView();
}
