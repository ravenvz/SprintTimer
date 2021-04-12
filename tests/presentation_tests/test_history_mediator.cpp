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
#include "gmock/gmock.h"
#include "qt_gui/presentation/HistoryMediatorImpl.h"

using namespace sprint_timer;
using namespace ::testing;

class ColleagueMock : public ui::HistoryColleague {
public:
    MOCK_METHOD(void, onSharedDataChanged, (), (override));
};

class HistoryMediatorFixture : public ::testing::Test {
public:
    ui::HistoryMediatorImpl mediator;
};

TEST_F(HistoryMediatorFixture, notifies_colleagues_when_date_range_is_changed)
{
    ColleagueMock colleague_1;
    ColleagueMock colleague_2;
    const dw::DateRange someRange{dw::current_date(), dw::current_date()};
    mediator.addColleague(&colleague_1);
    mediator.addColleague(&colleague_2);

    EXPECT_CALL(colleague_1, onSharedDataChanged());
    EXPECT_CALL(colleague_2, onSharedDataChanged());

    mediator.onRangeChanged(someRange);

    EXPECT_EQ(std::optional<dw::DateRange>{someRange},
              mediator.currentDateRange());
}

TEST_F(HistoryMediatorFixture, notifies_colleagues_when_display_is_changed)
{
    ColleagueMock colleague_1;
    ColleagueMock colleague_2;
    ColleagueMock driverColleague;
    const auto display = ui::HistoryMediator::DisplayedHistory::TaskHistory;
    mediator.addColleague(&colleague_1);
    mediator.addColleague(&colleague_2);
    mediator.addColleague(&driverColleague);

    EXPECT_CALL(colleague_1, onSharedDataChanged());
    EXPECT_CALL(colleague_2, onSharedDataChanged());
    EXPECT_CALL(driverColleague, onSharedDataChanged());

    mediator.changeDisplayedHistory(&driverColleague, display);

    EXPECT_EQ(display, mediator.displayedHistory());
}

TEST_F(HistoryMediatorFixture, sprint_history_is_default_displayed_history)
{
    EXPECT_EQ(ui::HistoryMediator::DisplayedHistory::SprintHistory,
              mediator.displayedHistory());
}
