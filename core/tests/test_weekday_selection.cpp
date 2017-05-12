/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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
#include <TestHarness.h>
#include <iostream>


TEST_GROUP(WeekdaySelection){

};

TEST(WeekdaySelection, test_adds_selection)
{
    WeekdaySelection selection;
    selection.selectDay(DateTime::Weekday::Monday);
    selection.selectDay(DateTime::Weekday::Sunday);

    CHECK_EQUAL(65, selection.selectionMask());
    CHECK(selection.isSelected(DateTime::Weekday::Monday));
    CHECK(selection.isSelected(DateTime::Weekday::Sunday));
}

TEST(WeekdaySelection, test_unselects_days)
{
    WeekdaySelection selection{65};

    selection.unselectDay(DateTime::Weekday::Monday);
    selection.unselectDay(DateTime::Weekday::Sunday);

    CHECK(!selection.isSelected(DateTime::Weekday::Monday));
    CHECK(!selection.isSelected(DateTime::Weekday::Sunday));
}
