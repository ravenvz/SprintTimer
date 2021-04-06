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
#include "core/ComputeByDayStrategy.h"
#include "core/GoalProgress.h"
#include "gtest/gtest.h"
#include <core/GoalProgress.h>
#include <core/ProgressOverPeriod.h>

using sprint_timer::GoalProgress;
using sprint_timer::ProgressOverPeriod;
using sprint_timer::WeekSchedule;
using sprint_timer::WorkSchedule;

using namespace dw;

class ComputeProgressByDayFixture : public ::testing::Test {
public:
    ComputeProgressByDayFixture()
    {
        weekSchedule.setTargetGoal(dw::Weekday::Monday, 13);
        weekSchedule.setTargetGoal(dw::Weekday::Tuesday, 13);
        weekSchedule.setTargetGoal(dw::Weekday::Wednesday, 13);
        weekSchedule.setTargetGoal(dw::Weekday::Thursday, 13);
        weekSchedule.setTargetGoal(dw::Weekday::Friday, 13);
        workSchedule.addWeekSchedule(period.start(), weekSchedule);
    }

    WeekSchedule weekSchedule;
    WorkSchedule workSchedule;

    const DateRange period{Date{Year{2019}, Month{1}, Day{30}},
                           Date{Year{2019}, Month{2}, Day{4}}};
    sprint_timer::ComputeByDayStrategy computeByDayStrategy;
};

TEST_F(ComputeProgressByDayFixture, handles_empty_distribution)
{
    const DateRange period{Date{Year{2019}, Month{1}, Day{30}},
                           Date{Year{2019}, Month{2}, Day{4}}};
    const std::vector<int> distribution;
    const std::vector<GoalProgress> expected{
        {GoalProgress::Estimated{13}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{13}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{13}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{13}, GoalProgress::Actual{0}}};

    const auto actual = computeByDayStrategy.computeProgress(
        period, distribution, workSchedule);

    EXPECT_EQ(expected, actual);
}

TEST_F(ComputeProgressByDayFixture, handles_generic_distribution)
{
    const DateRange period{Date{Year{2019}, Month{1}, Day{30}},
                           Date{Year{2019}, Month{2}, Day{4}}};
    const std::vector<int> distribution{13, 12, 11, 0, 0, 10};
    const std::vector<GoalProgress> expected{
        {GoalProgress::Estimated{13}, GoalProgress::Actual{13}},
        {GoalProgress::Estimated{13}, GoalProgress::Actual{12}},
        {GoalProgress::Estimated{13}, GoalProgress::Actual{11}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{13}, GoalProgress::Actual{10}}};

    const auto actual = computeByDayStrategy.computeProgress(
        period, distribution, workSchedule);

    EXPECT_EQ(expected, actual);
}

TEST_F(ComputeProgressByDayFixture, work_during_vacation)
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
    WorkSchedule vacationTracker;
    vacationTracker.addWeekSchedule(start, vacationSchedule);
    const std::vector<int> distribution{15, 13, 14, 0, 10, 12};
    const std::vector<GoalProgress> expected{
        {GoalProgress::Estimated{0}, GoalProgress::Actual{15}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{13}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{14}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{10}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{12}}};

    const auto actual = computeByDayStrategy.computeProgress(
        period, distribution, vacationTracker);

    EXPECT_EQ(expected, actual);
}
