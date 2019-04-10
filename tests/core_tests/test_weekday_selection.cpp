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

#include "core/utils/WeekdaySelection.h"
#include "gtest/gtest.h"

using dw::DateTime;
using sprint_timer::utils::WeekdaySelection;

TEST(WeekdaySelection, adds_selection)
{
    WeekdaySelection selection;
    selection.selectDay(dw::Weekday::Monday);
    selection.selectDay(dw::Weekday::Sunday);

    EXPECT_EQ(65, selection.selectionMask());

    EXPECT_TRUE(selection.isSelected(dw::Weekday::Monday));
    EXPECT_TRUE(selection.isSelected(dw::Weekday::Sunday));

    EXPECT_FALSE(selection.isSelected(dw::Weekday::Tuesday));
    EXPECT_FALSE(selection.isSelected(dw::Weekday::Wednesday));
    EXPECT_FALSE(selection.isSelected(dw::Weekday::Thursday));
    EXPECT_FALSE(selection.isSelected(dw::Weekday::Friday));
    EXPECT_FALSE(selection.isSelected(dw::Weekday::Saturday));
}

TEST(WeekdaySelection, unselects_days)
{
    WeekdaySelection selection{65};

    selection.unselectDay(dw::Weekday::Monday);
    selection.unselectDay(dw::Weekday::Sunday);

    EXPECT_FALSE(selection.isSelected(dw::Weekday::Monday));
    EXPECT_FALSE(selection.isSelected(dw::Weekday::Sunday));
    EXPECT_FALSE(selection.isSelected(dw::Weekday::Tuesday));
    EXPECT_FALSE(selection.isSelected(dw::Weekday::Wednesday));
    EXPECT_FALSE(selection.isSelected(dw::Weekday::Thursday));
    EXPECT_FALSE(selection.isSelected(dw::Weekday::Friday));
    EXPECT_FALSE(selection.isSelected(dw::Weekday::Saturday));
}

TEST(WeekdaySelection, returns_total_number_selected)
{
    EXPECT_EQ(0, WeekdaySelection{0}.numSelected());
    EXPECT_EQ(7, WeekdaySelection{127}.numSelected());
    EXPECT_EQ(5, WeekdaySelection{93}.numSelected());
}

TEST(WeekdaySelection, returns_number_of_workdays_in_given_timespan)
{
    using namespace dw;
    WeekdaySelection selection{31}; // Monday - Friday selected
    const DateTimeRange timeSpan{DateTime{Date{Year{2019}, Month{1}, Day{26}}},
                                 DateTime{Date{Year{2019}, Month{2}, Day{7}}}};

    EXPECT_EQ(9, numWorkdays(timeSpan, selection));
}

TEST(WeekdaySelection, returns_workday_outline)
{
    using namespace dw;
    WeekdaySelection selection{31}; // Monday - Friday selected
    const DateTimeRange timeSpan{DateTime{Date{Year{2019}, Month{1}, Day{26}}},
                                 DateTime{Date{Year{2019}, Month{2}, Day{7}}}};
    const std::vector<int> expected{0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1};

    EXPECT_EQ(expected, workday_outline(timeSpan, selection));
}

