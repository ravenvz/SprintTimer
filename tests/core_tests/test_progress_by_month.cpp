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
#include "core/GoalProgress.h"
#include "core/GroupByMonth.h"
#include "gtest/gtest.h"
#include <core/GoalProgress.h>
#include <core/ProgressOverPeriod.h>

using sprint_timer::GoalProgress;
using sprint_timer::ProgressOverPeriod;
using sprint_timer::WeekSchedule;
using sprint_timer::WorkdayTracker;

using namespace dw;

/* NOTE
 * These tests focus on testing ProgressOverPeriod features together with
 * GroupByDay strategy. They do not test odd combinations of WeekSchedules or
 * addition of exceptional days trusting that these things are tested in
 * WorkdayTracker tests.
 */

class ProgressByMonthFixture : public ::testing::Test {
public:
    ProgressByMonthFixture()
    {
        schedule.setTargetGoal(dw::Weekday::Monday, 10);
        schedule.setTargetGoal(dw::Weekday::Tuesday, 10);
        schedule.setTargetGoal(dw::Weekday::Wednesday, 10);
        schedule.setTargetGoal(dw::Weekday::Thursday, 10);
        schedule.setTargetGoal(dw::Weekday::Friday, 10);
        workdayTracker.addWeekSchedule(period.start(), schedule);
    }

    WeekSchedule schedule;
    WorkdayTracker workdayTracker;

    const DateRange period{Date{Year{2018}, Month{4}, Day{10}},
                           Date{Year{2018}, Month{8}, Day{25}}};
    sprint_timer::GroupByMonth groupByMonthStrategy;
};

TEST_F(ProgressByMonthFixture, handles_empty_actual_progress)
{
    const std::vector<int> actualProgress;
    const std::vector<GoalProgress> expected{
        {150, 0}, {230, 0}, {210, 0}, {220, 0}, {180, 0}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdayTracker, groupByMonthStrategy};

    EXPECT_EQ(0, progress.actual());
    EXPECT_EQ(990, progress.estimated());
    EXPECT_NEAR(0, *progress.percentage(), 0.1);
    EXPECT_EQ(990, progress.difference());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(0, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(5, progress.size());

    for (size_t i = 0; i < expected.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(ProgressByMonthFixture, underwork)
{
    const std::vector<int> actualProgress{180, 200, 150, 170, 150};
    const std::vector<GoalProgress> expected{
        {150, 180}, {230, 200}, {210, 150}, {220, 170}, {180, 150}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdayTracker, groupByMonthStrategy};

    EXPECT_EQ(850, progress.actual());
    EXPECT_EQ(990, progress.estimated());
    EXPECT_NEAR(85.8, *progress.percentage(), 0.1);
    EXPECT_EQ(140, progress.difference());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(170, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(5, progress.size());

    for (size_t i = 0; i < actualProgress.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(ProgressByMonthFixture, overwork)
{
    const std::vector<int> actualProgress{180, 250, 230, 200, 190};
    const std::vector<GoalProgress> expected{
        {150, 180}, {230, 250}, {210, 230}, {220, 200}, {180, 190}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdayTracker, groupByMonthStrategy};

    EXPECT_EQ(1050, progress.actual());
    EXPECT_EQ(990, progress.estimated());
    EXPECT_NEAR(106, *progress.percentage(), 0.1);
    EXPECT_EQ(-60, progress.difference());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_NEAR(210, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(5, progress.size());

    for (size_t i = 0; i < actualProgress.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(ProgressByMonthFixture, work_during_vacation)
{
    WeekSchedule vacationSchedule;
    vacationSchedule.setTargetGoal(dw::Weekday::Monday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Tuesday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Wednesday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Thursday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Friday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Saturday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Sunday, 0);
    WorkdayTracker vacationTracker;
    vacationTracker.addWeekSchedule(period.start(), vacationSchedule);
    const std::vector<int> actualProgress{180, 250, 230, 200, 190};
    const std::vector<GoalProgress> expected{
        {0, 180}, {0, 250}, {0, 230}, {0, 200}, {0, 190}};

    const ProgressOverPeriod progress{
        period, actualProgress, vacationTracker, groupByMonthStrategy};

    EXPECT_EQ(1050, progress.actual());
    EXPECT_EQ(0, progress.estimated());
    EXPECT_FALSE(progress.percentage());
    EXPECT_EQ(-1050, progress.difference());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_FALSE(progress.averagePerGroupPeriod());
    EXPECT_EQ(5, progress.size());

    for (size_t i = 0; i < actualProgress.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(ProgressByMonthFixture,
       handles_corner_case_when_starting_from_first_day_of_month)
{
    const DateRange a_period{Date{Year{2018}, Month{6}, Day{1}},
                             Date{Year{2018}, Month{7}, Day{16}}};
    const std::vector<int> actualProgress{200, 75};
    const std::vector<GoalProgress> expected{{210, 200}, {110, 75}};

    const ProgressOverPeriod progress{
        a_period, actualProgress, workdayTracker, groupByMonthStrategy};

    EXPECT_EQ(275, progress.actual());
    EXPECT_EQ(320, progress.estimated());
    EXPECT_NEAR(85.9, *progress.percentage(), 0.1);
    EXPECT_EQ(45, progress.difference());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(137.5, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(2, progress.size());

    for (size_t i = 0; i < actualProgress.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(ProgressByMonthFixture,
       handles_corner_case_when_ending_on_the_last_day_of_month)
{
    const DateRange a_period{Date{Year{2018}, Month{6}, Day{1}},
                             Date{Year{2018}, Month{7}, Day{31}}};
    const std::vector<int> actualProgress{200, 75};
    const std::vector<GoalProgress> expected{{210, 200}, {220, 75}};

    const ProgressOverPeriod progress{
        a_period, actualProgress, workdayTracker, groupByMonthStrategy};

    EXPECT_EQ(275, progress.actual());
    EXPECT_EQ(430, progress.estimated());
    EXPECT_NEAR(63.9, *progress.percentage(), 0.1);
    EXPECT_EQ(155, progress.difference());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(137.5, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(2, progress.size());

    for (size_t i = 0; i < actualProgress.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}
