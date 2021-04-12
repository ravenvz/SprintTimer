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
#include "core/ComputeByMonthStrategy.h"
#include "core/GoalProgress.h"
#include "gtest/gtest.h"
#include "core/GoalProgress.h"
#include "core/ProgressOverPeriod.h"

using sprint_timer::GoalProgress;
using sprint_timer::ProgressOverPeriod;
using sprint_timer::WeekSchedule;
using sprint_timer::WorkSchedule;

using namespace dw;

class ComputeProgressByMonthFixture : public ::testing::Test {
public:
    ComputeProgressByMonthFixture()
    {
        weekSchedule.setTargetGoal(dw::Weekday::Monday, 10);
        weekSchedule.setTargetGoal(dw::Weekday::Tuesday, 10);
        weekSchedule.setTargetGoal(dw::Weekday::Wednesday, 10);
        weekSchedule.setTargetGoal(dw::Weekday::Thursday, 10);
        weekSchedule.setTargetGoal(dw::Weekday::Friday, 10);
        workSchedule.addWeekSchedule(period.start(), weekSchedule);
    }

    WeekSchedule weekSchedule;
    WorkSchedule workSchedule;

    const DateRange period{Date{Year{2018}, Month{4}, Day{10}},
                           Date{Year{2018}, Month{8}, Day{25}}};
    sprint_timer::ComputeByMonthStrategy computeByMonthStrategy;
};

TEST_F(ComputeProgressByMonthFixture, handles_empty_distribution)
{
    const std::vector<int> distribution;
    const std::vector<GoalProgress> expected{
        {GoalProgress::Estimated{150}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{230}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{210}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{220}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{180}, GoalProgress::Actual{0}}};

    const auto actual = computeByMonthStrategy.computeProgress(
        period, distribution, workSchedule);

    EXPECT_EQ(expected, actual);
}

TEST_F(ComputeProgressByMonthFixture, handles_generic_distribution)
{
    const std::vector<int> distribution{180, 200, 150, 170, 150};
    const std::vector<GoalProgress> expected{
        {GoalProgress::Estimated{150}, GoalProgress::Actual{180}},
        {GoalProgress::Estimated{230}, GoalProgress::Actual{200}},
        {GoalProgress::Estimated{210}, GoalProgress::Actual{150}},
        {GoalProgress::Estimated{220}, GoalProgress::Actual{170}},
        {GoalProgress::Estimated{180}, GoalProgress::Actual{150}}};

    const auto actual = computeByMonthStrategy.computeProgress(
        period, distribution, workSchedule);

    EXPECT_EQ(expected, actual);
}

TEST_F(ComputeProgressByMonthFixture, work_during_vacation)
{
    WeekSchedule vacationSchedule;
    vacationSchedule.setTargetGoal(dw::Weekday::Monday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Tuesday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Wednesday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Thursday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Friday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Saturday, 0);
    vacationSchedule.setTargetGoal(dw::Weekday::Sunday, 0);
    WorkSchedule vacationTracker;
    vacationTracker.addWeekSchedule(period.start(), vacationSchedule);
    const std::vector<int> distribution{180, 250, 230, 200, 190};
    const std::vector<GoalProgress> expected{
        {GoalProgress::Estimated{0}, GoalProgress::Actual{180}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{250}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{230}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{200}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{190}}};

    const auto actual = computeByMonthStrategy.computeProgress(
        period, distribution, vacationTracker);

    EXPECT_EQ(expected, actual);
}

TEST_F(ComputeProgressByMonthFixture,
       handles_corner_case_when_starting_from_first_day_of_month)
{
    const DateRange a_period{Date{Year{2018}, Month{6}, Day{1}},
                             Date{Year{2018}, Month{7}, Day{16}}};
    const std::vector<int> distribution{200, 75};
    const std::vector<GoalProgress> expected{
        {GoalProgress::Estimated{210}, GoalProgress::Actual{200}},
        {GoalProgress::Estimated{110}, GoalProgress::Actual{75}}};

    const auto actual = computeByMonthStrategy.computeProgress(
        a_period, distribution, workSchedule);

    EXPECT_EQ(expected, actual);
}

TEST_F(ComputeProgressByMonthFixture,
       handles_corner_case_when_ending_on_the_last_day_of_month)
{
    const DateRange a_period{Date{Year{2018}, Month{6}, Day{1}},
                             Date{Year{2018}, Month{7}, Day{31}}};
    const std::vector<int> distribution{200, 75};
    const std::vector<GoalProgress> expected{
        {GoalProgress::Estimated{210}, GoalProgress::Actual{200}},
        {GoalProgress::Estimated{220}, GoalProgress::Actual{75}}};

    const auto actual = computeByMonthStrategy.computeProgress(
        a_period, distribution, workSchedule);

    EXPECT_EQ(expected, actual);
}
