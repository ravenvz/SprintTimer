/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "mocks/QueryHandlerMock.h"
#include "mocks/StatisticsColleagueMock.h"
#include "gmock/gmock.h"
#include <core/SprintBuilder.h>
#include <qt_gui/presentation/StatisticsMediatorImpl.h>

using namespace sprint_timer::ui;
using sprint_timer::entities::Sprint;

namespace sprint_timer::use_cases {

bool operator==(const RequestSprintsQuery& lhs, const RequestSprintsQuery& rhs)
{
    return lhs.dateRange == rhs.dateRange;
}

} // namespace sprint_timer::use_cases

class StatisticsMediatorFixture : public ::testing::Test {
public:
    ::testing::NiceMock<
        mocks::QueryHandlerMock<sprint_timer::use_cases::RequestSprintsQuery,
                                std::vector<sprint_timer::entities::Sprint>>>
        requestSprintsHandlerMock;
    const size_t numTopTags{12};
    StatisticsMediatorImpl mediator{requestSprintsHandlerMock, numTopTags};
    ::testing::NiceMock<mocks::ColleagueMock> colleagueMockFirst;
    ::testing::NiceMock<mocks::ColleagueMock> colleagueMockSecond;
    const dw::DateRange someDateRange{dw::current_date(), dw::current_date()};
};

TEST_F(StatisticsMediatorFixture,
       notifies_colleagues_when_date_range_is_changed)
{
    mediator.addColleague(&colleagueMockFirst);
    mediator.addColleague(&colleagueMockSecond);

    EXPECT_CALL(colleagueMockFirst, onSharedDataChanged()).Times(1);
    EXPECT_CALL(colleagueMockSecond, onSharedDataChanged()).Times(1);

    mediator.onRangeChanged(someDateRange);
}

TEST_F(StatisticsMediatorFixture,
       notifies_colleaugues_except_caller_when_tag_is_changed_TODO_REMOVE)
{
    mocks::ColleagueMock driverColleague;
    mediator.addColleague(&colleagueMockFirst);
    mediator.addColleague(&colleagueMockSecond);
    mediator.addColleague(&driverColleague);

    EXPECT_CALL(colleagueMockFirst, onSharedDataChanged()).Times(1);
    EXPECT_CALL(colleagueMockSecond, onSharedDataChanged()).Times(1);
    EXPECT_CALL(driverColleague, onSharedDataChanged()).Times(0);

    mediator.filterByTag(&driverColleague, 3);
}

TEST_F(StatisticsMediatorFixture,
       notifies_colleaugues_except_caller_when_tag_is_changed)
{
    mocks::ColleagueMock driverColleague;
    mediator.addColleague(&colleagueMockFirst);
    mediator.addColleague(&colleagueMockSecond);
    mediator.addColleague(&driverColleague);

    EXPECT_CALL(colleagueMockFirst, onSharedDataChanged()).Times(1);
    EXPECT_CALL(colleagueMockSecond, onSharedDataChanged()).Times(1);
    EXPECT_CALL(driverColleague, onSharedDataChanged()).Times(0);

    mediator.filterByTag(&driverColleague, 3);
}

TEST_F(StatisticsMediatorFixture, requeries_handler_when_date_range_is_changed)
{
    using sprint_timer::use_cases::RequestSprintsQuery;
    mocks::ColleagueMock driverColleague;
    mediator.addColleague(&driverColleague);
    const dw::DateRange newDateRange{
        dw::add_offset(someDateRange, dw::Days{3})};

    EXPECT_CALL(requestSprintsHandlerMock,
                handle(RequestSprintsQuery{newDateRange}));

    mediator.onRangeChanged(newDateRange);
}

TEST_F(StatisticsMediatorFixture, filters_sprints_by_tag)
{
    using sprint_timer::entities::Tag;
    mocks::ColleagueMock driverColleague;
    mediator.addColleague(&driverColleague);
    sprint_timer::SprintBuilder builder;
    const dw::DateTimeRange someTimeSpan{dw::current_date_time(),
                                         dw::current_date_time()};
    builder.withTaskUuid("123").withTimeSpan(someTimeSpan);

    std::vector<Sprint> sprints{
        builder.withExplicitTags({Tag{"Tag1"}}).build(),
        builder.withExplicitTags({Tag{"Tag2"}}).build(),
        builder.withExplicitTags({Tag{"Tag2"}}).build(),
        builder.withExplicitTags({Tag{"Tag3"}}).build(),
    };
    const std::vector<Sprint> sprints_with_tag_2{sprints[1], sprints[2]};
    ON_CALL(requestSprintsHandlerMock, handle(::testing::_))
        .WillByDefault(::testing::Return(sprints));

    mediator.onRangeChanged(someDateRange);
    mediator.filterByTag(&driverColleague, 0);

    EXPECT_THAT(mediator.sprints(),
                ::testing::UnorderedElementsAreArray(sprints_with_tag_2));

    mediator.filterByTag(&driverColleague, std::nullopt);

    EXPECT_THAT(mediator.sprints(),
                ::testing::UnorderedElementsAreArray(sprints));
}

TEST_F(StatisticsMediatorFixture, resets_selected_tag_when_changing_date_range)
{
    mocks::ColleagueMock driverColleague;
    mediator.addColleague(&driverColleague);
    const std::optional<size_t> someTagIndex{4};
    mediator.filterByTag(&driverColleague, someTagIndex);

    EXPECT_EQ(someTagIndex, mediator.selectedTagNumber());

    mediator.onRangeChanged(someDateRange);

    EXPECT_EQ(std::nullopt, mediator.selectedTagNumber());
}
