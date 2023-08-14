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
#include "core/TaskTimeframe.h"
#include "gtest/gtest.h"

using namespace std::chrono_literals;
using namespace sprint_timer;
using namespace dw;

class TaskTimeframeFixture : public ::testing::Test {
public:
};

TEST_F(TaskTimeframeFixture, inherits_start_and_due_date_if_is_not_set)
{
    constexpr dw::DateTime start{DateTime{Date{Year{2023}, Month{7}, Day{10}}} +
                                 4h + 30min};
    constexpr dw::DateTime dueTo{DateTime{Date{Year{2023}, Month{9}, Day{10}}}};
    constexpr TaskTimeframe sut{start - Days{20}};
    constexpr TaskTimeframe other{start, dueTo};
    constexpr TaskTimeframe expected{start, dueTo};

    constexpr auto actual = sut.inherit(other);

    static_assert(expected == actual);
}

TEST_F(TaskTimeframeFixture, does_not_inherit_date_when_due_date_is_set)
{
    constexpr dw::DateTime start{DateTime{Date{Year{2023}, Month{7}, Day{10}}} +
                                 4h + 30min};
    constexpr dw::DateTime dueTo{DateTime{Date{Year{2023}, Month{9}, Day{10}}}};
    constexpr TaskTimeframe sut{start - Days{20}};
    constexpr TaskTimeframe other{start, dueTo};
    constexpr TaskTimeframe expected{start, dueTo};

    constexpr auto actual = sut.inherit(other);

    static_assert(expected == actual);
}

TEST_F(
    TaskTimeframeFixture,
    sets_due_date_to_next_recurrence_when_recurrence_is_set_and_due_date_is_not)
{
    constexpr auto start =
        DateTime{Date{Year{2016}, Month{9}, Day{21}}} + 12h + 59min + 19s;
    const TaskTimeframe expected{
        start,
        DateTime{Date{Year{2016}, Month{10}, Day{10}}} + 21h + 20min,
        std::nullopt,
        Recurrence{"*-*-10 21:20:00"}};
    const TaskTimeframe sut{
        start, std::nullopt, std::nullopt, Recurrence{"*-*-10 21:20:00"}};

    EXPECT_EQ(expected, sut);
    // static_assert(expected == sut);
}
