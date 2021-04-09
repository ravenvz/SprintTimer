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
#include "mocks/DateRangeChangeListenerMock.h"
#include "mocks/QueryHandlerMock.h"
#include "mocks/StatisticsColleagueMock.h"
#include "mocks/StatisticsMediatorMock.h"
#include <core/QueryHandler.h>
#include <core/use_cases/request_op_range/OperationalRangeQuery.h>
#include <qt_gui/presentation/DateRangeSelectorPresenter.h>
#include <qt_gui/presentation/StatisticsMediator.h>

using namespace ::testing;
using namespace dw;

using sprint_timer::use_cases::OperationalRangeQuery;

class DateRangeSelectorViewMock
    : public sprint_timer::ui::contracts::DateRangeSelectorContract::View {
public:
    using sprint_timer::ui::contracts::DateRangeSelectorContract::View::View;

    MOCK_METHOD(void,
                updateOperationalRange,
                (const std::vector<int>&),
                (override));

    MOCK_METHOD(void, setFirstDayOfWeek, (dw::Weekday), (override));
};

class DateRangeSelectorPresenterFixture : public ::testing::Test {
public:
    NiceMock<mocks::QueryHandlerMock<OperationalRangeQuery, dw::DateRange>>
        requestOperationalRangeHandlerMock;
    NiceMock<mocks::DateRangeChangeListenerMock> dateRangeChangeListenerMock;
    DateRange someDateRange{Date{Year{1995}, Month{2}, Day{7}},
                            Date{Year{2003}, Month{8}, Day{3}}};
    NiceMock<DateRangeSelectorViewMock> view;
    sprint_timer::ui::DateRangeSelectorPresenter sut{
        requestOperationalRangeHandlerMock,
        dateRangeChangeListenerMock,
        dw::Weekday::Monday};
};

TEST_F(DateRangeSelectorPresenterFixture,
       initializes_view_when_view_is_attached)
{
    ON_CALL(requestOperationalRangeHandlerMock, handle(_))
        .WillByDefault(Return(someDateRange));
    const std::vector<int> expected{
        1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003};

    EXPECT_CALL(view, updateOperationalRange(expected));
    EXPECT_CALL(view, setFirstDayOfWeek(dw::Weekday::Monday));

    sut.attachView(view);
}

TEST_F(DateRangeSelectorPresenterFixture, updates_operational_range)
{
    ON_CALL(requestOperationalRangeHandlerMock, handle(_))
        .WillByDefault(Return(someDateRange));
    const std::vector<int> expected{
        1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003};
    sut.attachView(view);

    EXPECT_CALL(view, updateOperationalRange(expected));

    sut.updateView();
}

TEST_F(DateRangeSelectorPresenterFixture,
       notifies_mediator_when_selected_date_range_is_changed)
{
    EXPECT_CALL(dateRangeChangeListenerMock, onRangeChanged(someDateRange));

    sut.onSelectedRangeChanged(someDateRange);
}

TEST_F(DateRangeSelectorPresenterFixture, sets_first_day_of_week_for_view)
{
    ON_CALL(requestOperationalRangeHandlerMock, handle(_))
        .WillByDefault(Return(someDateRange));
    sut.attachView(view);

    EXPECT_CALL(view, setFirstDayOfWeek(dw::Weekday::Monday));

    sut.updateView();
}
