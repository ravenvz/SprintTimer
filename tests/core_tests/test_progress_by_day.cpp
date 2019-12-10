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
#include "core/GroupByDay.h"
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

class ProgressByDayFixture : public ::testing::Test {
public:
    ProgressByDayFixture()
    {
        schedule.setTargetGoal(dw::Weekday::Monday, 13);
        schedule.setTargetGoal(dw::Weekday::Tuesday, 13);
        schedule.setTargetGoal(dw::Weekday::Wednesday, 13);
        schedule.setTargetGoal(dw::Weekday::Thursday, 13);
        schedule.setTargetGoal(dw::Weekday::Friday, 13);
        workdayTracker.addWeekSchedule(period.start(), schedule);
    }

    WeekSchedule schedule;
    WorkdayTracker workdayTracker;

    const DateRange period{Date{Year{2019}, Month{1}, Day{30}},
                           Date{Year{2019}, Month{2}, Day{4}}};
    sprint_timer::GroupByDay groupByDayStrategy;
};

TEST_F(ProgressByDayFixture, handles_empty_actual_progress)
{
    const DateRange period{Date{Year{2019}, Month{1}, Day{30}},
                           Date{Year{2019}, Month{2}, Day{4}}};
    const std::vector<int> actualProgress;
    const std::vector<GoalProgress> expected{
        {13, 0}, {13, 0}, {13, 0}, {0, 0}, {0, 0}, {13, 0}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdayTracker, groupByDayStrategy};

    EXPECT_EQ(0, progress.actual());
    EXPECT_DOUBLE_EQ(0, *progress.averagePerGroupPeriod());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(0, *progress.percentage(), 0.1);
    EXPECT_EQ(52, progress.difference());
    EXPECT_EQ(52, progress.estimated());
    EXPECT_EQ(6, progress.size());

    for (size_t i = 0; i < expected.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(ProgressByDayFixture, underwork)
{
    const DateRange period{Date{Year{2019}, Month{1}, Day{30}},
                           Date{Year{2019}, Month{2}, Day{4}}};
    const std::vector<int> actualProgress{13, 12, 11, 0, 0, 10};
    const std::vector<GoalProgress> expected{
        {13, 13}, {13, 12}, {13, 11}, {0, 0}, {0, 0}, {13, 10}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdayTracker, groupByDayStrategy};

    EXPECT_EQ(46, progress.actual());
    EXPECT_DOUBLE_EQ(11.5, *progress.averagePerGroupPeriod());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(88.46, *progress.percentage(), 0.1);
    EXPECT_EQ(6, progress.difference());
    EXPECT_EQ(52, progress.estimated());
    EXPECT_EQ(6, progress.size());

    for (size_t i = 0; i < actualProgress.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(ProgressByDayFixture, overwork)
{
    const DateRange period{Date{Year{2019}, Month{1}, Day{30}},
                           Date{Year{2019}, Month{2}, Day{4}}};
    const std::vector<int> actualProgress{15, 13, 14, 0, 10, 12};
    const std::vector<GoalProgress> expected{
        {13, 15}, {13, 13}, {13, 14}, {0, 0}, {0, 10}, {13, 12}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdayTracker, groupByDayStrategy};

    EXPECT_EQ(64, progress.actual());
    EXPECT_DOUBLE_EQ(16, *progress.averagePerGroupPeriod());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_EQ(-12, progress.difference());
    EXPECT_NEAR(123.08, *progress.percentage(), 0.1);
    EXPECT_EQ(52, progress.estimated());
    EXPECT_EQ(6, progress.size());

    for (size_t i = 0; i < actualProgress.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(ProgressByDayFixture, work_during_vacation)
{
    const Date start{Year{2019}, Month{1}, Day{30}};
    const Date finish{Year{2019}, Month{2}, Day{4}};
    const DateRange period{start, finish};
    WeekSchedule vacationSchedule;
    vacationSchedule.setTargetGoal(dw::Weekday::Monday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Tuesday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Wednesday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Thursday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Friday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Saturday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Sunday, 0);
    WorkdayTracker vacationTracker;
    vacationTracker.addWeekSchedule(start, vacationSchedule);
    const std::vector<int> actualProgress{15, 13, 14, 0, 10, 12};
    const std::vector<GoalProgress> expected{
        {0, 15}, {0, 13}, {0, 14}, {0, 0}, {0, 10}, {0, 12}};

    const ProgressOverPeriod progress{
        period, actualProgress, vacationTracker, groupByDayStrategy};

    EXPECT_EQ(64, progress.actual());
    EXPECT_FALSE(progress.averagePerGroupPeriod());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_FALSE(progress.percentage());
    EXPECT_EQ(-64, progress.difference());
    EXPECT_EQ(0, progress.estimated());
    EXPECT_EQ(6, progress.size());

    for (size_t i = 0; i < actualProgress.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}
