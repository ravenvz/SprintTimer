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
#include "core/Sprint.h"
#include "gtest/gtest.h"
#include <chrono>

using namespace sprint_timer;

class TestSprintFixture : public ::testing::Test {
public:
};

TEST_F(TestSprintFixture, detects_no_conflict_when_sprints_are_apart_in_time)
{
    using namespace dw;
    using namespace std::chrono_literals;

    const Sprint left{DateTimeRange{
        DateTime{Date{Year{2021}, Month{11}, Day{26}}} + 18h + 43min + 38s,
        DateTime{Date{Year{2021}, Month{11}, Day{26}}} + 19h + 8min + 38s}};
    const Sprint right{DateTimeRange{
        DateTime{Date{Year{2021}, Month{11}, Day{26}}} + 19h + 43min + 38s,
        DateTime{Date{Year{2021}, Month{11}, Day{26}}} + 20h + 8min + 38s}};

    ASSERT_FALSE(intersectingInTime(left, right));
    ASSERT_FALSE(intersectingInTime(right, left));
}

TEST_F(
    TestSprintFixture,
    consecutive_sprints_are_not_considered_conflicting_if_they_start_or_end_at_same_minute)
{
    using namespace dw;
    using namespace std::chrono_literals;

    ASSERT_FALSE(intersectingInTime(
        Sprint{DateTimeRange{
            DateTime{Date{Year{2021}, Month{11}, Day{26}}} + 19h + 8min + 1s,
            DateTime{Date{Year{2021}, Month{11}, Day{26}}} + 20h + 33min + 1s}},
        Sprint{DateTimeRange{DateTime{Date{Year{2021}, Month{11}, Day{26}}} +
                                 18h + 43min + 59s,
                             DateTime{Date{Year{2021}, Month{11}, Day{26}}} +
                                 19h + 8min + 59s}}));
    ASSERT_FALSE(intersectingInTime(
        Sprint{DateTimeRange{
            DateTime{Date{Year{2021}, Month{5}, Day{2}}} + 23h + 45min + 28s,
            DateTime{Date{Year{2021}, Month{5}, Day{3}}} + 10min + 28s}},
        Sprint{
            DateTimeRange{
                DateTime{Date{Year{2021}, Month{5}, Day{3}}} + 10min + 2s,
                DateTime{Date{Year{2021}, Month{5}, Day{3}}} + 35min + 2s},
        }));
}

TEST_F(TestSprintFixture,
       detects_conflict_when_one_sprint_intersects_with_others_time_range)
{
    using namespace dw;
    using namespace std::chrono_literals;

    const Sprint left{
        DateTimeRange{
            DateTime{Date{Year{2021}, Month{11}, Day{26}}} + 18h + 43min + 28s,
            DateTime{Date{Year{2021}, Month{11}, Day{26}}} + 19h + 8min + 38s},
    };
    const Sprint right{
        DateTimeRange{
            DateTime{Date{Year{2021}, Month{11}, Day{26}}} + 18h + 52min + 18s,
            DateTime{Date{Year{2021}, Month{11}, Day{26}}} + 19h + 17min + 18s},
    };

    ASSERT_TRUE(intersectingInTime(left, right));
    ASSERT_TRUE(intersectingInTime(right, left));
}

