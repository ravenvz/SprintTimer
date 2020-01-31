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
#include "gtest/gtest.h"
#include <core/ComputeByWeekStrategy.h>
#include <core/GoalProgress.h>

using sprint_timer::GoalProgress;
using sprint_timer::WeekSchedule;
using sprint_timer::WorkSchedule;

using namespace dw;

class ComputeProgressByWeekStrategyFixture : public ::testing::Test {
public:
    ComputeProgressByWeekStrategyFixture()
    {
        weekSchedule.setTargetGoal(dw::Weekday::Monday, 13);
        weekSchedule.setTargetGoal(dw::Weekday::Tuesday, 13);
        weekSchedule.setTargetGoal(dw::Weekday::Wednesday, 13);
        weekSchedule.setTargetGoal(dw::Weekday::Thursday, 13);
        weekSchedule.setTargetGoal(dw::Weekday::Friday, 13);
        workSchedule.addWeekSchedule(period.start() - Years{1}, weekSchedule);
    }

    WeekSchedule weekSchedule;
    WorkSchedule workSchedule;

    const DateRange period{Date{Year{2019}, Month{1}, Day{9}},
                           Date{Year{2019}, Month{2}, Day{14}}};
    const sprint_timer::ComputeByWeekStrategy mondayFirstStrategy{
        dw::Weekday::Monday};
};

TEST_F(ComputeProgressByWeekStrategyFixture, handles_empty_distribution)
{
    const std::vector<int> distribution;
    const std::vector<GoalProgress> expected{
        {GoalProgress::Estimated{39}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{52}, GoalProgress::Actual{0}}};

    const auto actual =
        mondayFirstStrategy.computeProgress(period, distribution, workSchedule);

    EXPECT_EQ(expected, actual);
}

TEST_F(ComputeProgressByWeekStrategyFixture, handles_generic_distribution)
{
    const std::vector<int> distribution{15, 13, 14, 0, 10, 12};
    const std::vector<GoalProgress> expected{
        {GoalProgress::Estimated{39}, GoalProgress::Actual{15}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{13}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{14}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{10}},
        {GoalProgress::Estimated{52}, GoalProgress::Actual{12}}};

    const auto actual =
        mondayFirstStrategy.computeProgress(period, distribution, workSchedule);

    EXPECT_EQ(expected, actual);
}

TEST_F(ComputeProgressByWeekStrategyFixture, work_during_vacation)
{
    const Date start{Year{2019}, Month{1}, Day{9}};
    const Date finish{Year{2019}, Month{2}, Day{14}};
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
    const std::vector<int> distribution{30, 65, 65, 50, 64, 60};
    const std::vector<GoalProgress> expected{
        {GoalProgress::Estimated{0}, GoalProgress::Actual{30}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{65}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{65}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{50}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{64}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{60}}};

    const auto actual = mondayFirstStrategy.computeProgress(
        period, distribution, vacationTracker);

    EXPECT_EQ(expected, actual);
}

TEST_F(ComputeProgressByWeekStrategyFixture,
       handles_corner_case_when_starting_from_grouping_day)
{
    const std::vector<int> distribution{30, 65};
    const std::vector<GoalProgress> expected{
        {GoalProgress::Estimated{65}, GoalProgress::Actual{30}},
        {GoalProgress::Estimated{13}, GoalProgress::Actual{65}}};
    const DateRange a_period{Date{Year{2019}, Month{1}, Day{13}},
                             Date{Year{2019}, Month{1}, Day{21}}};

    const sprint_timer::ComputeByWeekStrategy strategy{dw::Weekday::Sunday};
    const auto actual =
        strategy.computeProgress(a_period, distribution, workSchedule);

    EXPECT_EQ(expected, actual);
}

TEST_F(ComputeProgressByWeekStrategyFixture,
       handles_corner_case_when_ending_by_grouping_day)
{
    const std::vector<int> distribution{30, 65};
    const std::vector<GoalProgress> expected{
        {GoalProgress::Estimated{52}, GoalProgress::Actual{30}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{65}}};
    const DateRange a_period{Date{Year{2019}, Month{1}, Day{8}},
                             Date{Year{2019}, Month{1}, Day{19}}};

    const sprint_timer::ComputeByWeekStrategy strategy{dw::Weekday::Sunday};
    const auto actual =
        strategy.computeProgress(a_period, distribution, workSchedule);

    EXPECT_EQ(expected, actual);
}
