/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

// TODO remove when Gtest drops std::tr1
// Workaround for C++17 as std::tr1 no longer available and Gtest uses it
#define GTEST_LANG_CXX11 1

#include "core/utils/WeekdaySelection.h"
#include "gtest/gtest.h"

using dw::DateTime;

TEST(WeekdaySelection, test_adds_selection)
{
    WeekdaySelection selection;
    selection.selectDay(DateTime::Weekday::Monday);
    selection.selectDay(DateTime::Weekday::Sunday);

    EXPECT_EQ(65, selection.selectionMask());
    EXPECT_TRUE(selection.isSelected(DateTime::Weekday::Monday));
    EXPECT_TRUE(selection.isSelected(DateTime::Weekday::Sunday));
}

TEST(WeekdaySelection, test_unselects_days)
{
    WeekdaySelection selection{65};

    selection.unselectDay(DateTime::Weekday::Monday);
    selection.unselectDay(DateTime::Weekday::Sunday);

    EXPECT_FALSE(selection.isSelected(DateTime::Weekday::Monday));
    EXPECT_FALSE(selection.isSelected(DateTime::Weekday::Sunday));
}

TEST(WeekdaySelection, test_returns_total_number_selected)
{
    EXPECT_EQ(0, WeekdaySelection{0}.numSelected());
    EXPECT_EQ(7, WeekdaySelection{127}.numSelected());
    EXPECT_EQ(5, WeekdaySelection{93}.numSelected());
}
