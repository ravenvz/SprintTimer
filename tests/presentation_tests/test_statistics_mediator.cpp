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
#include "mocks/StatisticsColleagueMock.h"
#include "qt_gui/presentation/StatisticsMediator.h"
#include "gmock/gmock.h"

using namespace sprint_timer::ui;

class StatisticsMediatorFixture : public ::testing::Test {
public:
    StatisticsMediator mediator;
    ::testing::NiceMock<mocks::ColleagueMock> colleagueMockFirst;
    ::testing::NiceMock<mocks::ColleagueMock> colleagueMockSecond;
    const dw::DateRange someDateRange{dw::current_date(), dw::current_date()};
};

TEST_F(StatisticsMediatorFixture,
       notifies_colleagues_when_date_range_is_changed)
{
    mediator.addColleague(&colleagueMockFirst);
    mediator.addColleague(&colleagueMockSecond);

    EXPECT_CALL(colleagueMockFirst, onDateRangeChanged(someDateRange));
    EXPECT_CALL(colleagueMockSecond, onDateRangeChanged(someDateRange));

    mediator.onRangeChanged(someDateRange);
}

TEST_F(StatisticsMediatorFixture,
       notifies_colleagues_when_tag_selection_is_changed)
{
    mocks::ColleagueMock driverColleague;
    mediator.addColleague(&driverColleague);
    mediator.addColleague(&colleagueMockFirst);
    const std::optional<size_t> someTagIndex{4};

    EXPECT_CALL(colleagueMockFirst, onTagSelected(someTagIndex));
    EXPECT_CALL(colleagueMockFirst, onSharedDataChanged());
    EXPECT_CALL(driverColleague, onTagSelected(someTagIndex)).Times(0);
    EXPECT_CALL(driverColleague, onSharedDataChanged()).Times(0);

    mediator.selectTag(&driverColleague, someTagIndex);
}
