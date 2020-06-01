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
#include "mocks/DateRangeChangeListenerMock.h"
#include "mocks/QueryHandlerMock.h"
#include "mocks/StatisticsColleagueMock.h"
#include "mocks/StatisticsMediatorMock.h"
#include <core/QueryHandler.h>
#include <core/use_cases/request_op_range/OperationalRangeQuery.h>
#include <qt_gui/presentation/DateRangeSelectorPresenter.h>
#include <qt_gui/presentation/StatisticsMediator.h>

using namespace ::testing;

using sprint_timer::use_cases::OperationalRangeQuery;

class DateRangeSelectorViewMock
    : public sprint_timer::ui::contracts::DateRangeSelectorContract::View {
public:
    MOCK_METHOD(void,
                updateOperationalRange,
                (const std::vector<int>&),
                (override));

    MOCK_METHOD(void, setFirstDayOfWeek, (dw::Weekday), (override));
};

class DateRangeSelectorPresenterFixture : public ::testing::Test {
public:
    NiceMock<DateRangeSelectorViewMock> viewMock;
    NiceMock<mocks::QueryHandlerMock<OperationalRangeQuery, dw::DateRange>>
        requestOperationalRangeHandlerMock;
    NiceMock<mocks::DateRangeChangeListenerMock> dateRangeChangeListenerMock;
    dw::DateRange someDateRange{dw::current_date(), dw::current_date()};
    sprint_timer::ui::DateRangeSelectorPresenter presenter{
        requestOperationalRangeHandlerMock,
        dateRangeChangeListenerMock,
        dw::Weekday::Monday};
};

TEST_F(DateRangeSelectorPresenterFixture, updates_operational_range)
{
    ON_CALL(requestOperationalRangeHandlerMock, handle(_))
        .WillByDefault(Return(someDateRange));

    EXPECT_CALL(viewMock, updateOperationalRange(_));

    presenter.attachView(viewMock);
}

TEST_F(DateRangeSelectorPresenterFixture,
       notifies_mediator_when_selected_date_range_is_changed)
{
    EXPECT_CALL(dateRangeChangeListenerMock, onRangeChanged(someDateRange));

    presenter.onSelectedRangeChanged(someDateRange);
}

TEST_F(DateRangeSelectorPresenterFixture, sets_first_day_of_week_for_view)
{
    ON_CALL(requestOperationalRangeHandlerMock, handle(_))
        .WillByDefault(Return(someDateRange));

    EXPECT_CALL(viewMock, setFirstDayOfWeek(dw::Weekday::Monday));

    presenter.attachView(viewMock);
}
