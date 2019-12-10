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
#include "mocks/ConfigMock.h"
#include "gtest/gtest.h"
#include <core/GoalProgress.h>
#include <core/GroupByWeek.h>
#include <core/ProgressOverPeriod.h>

using sprint_timer::GoalProgress;
using sprint_timer::ProgressOverPeriod;
using sprint_timer::WeekSchedule;
using sprint_timer::WorkdayTracker;
using ::testing::Return;

using namespace dw;

/* NOTE
 * These tests focus on testing ProgressOverPeriod features together with
 * GroupByMonth strategy. They do not test odd combinations of WeekSchedules or
 * addition of exceptional days trusting that these things are tested in
 * WorkdayTracker tests.
 */

class ProgressByWeekFixture : public ::testing::Test {
public:
    ProgressByWeekFixture()
    {
        schedule.setTargetGoal(dw::Weekday::Monday, 13);
        schedule.setTargetGoal(dw::Weekday::Tuesday, 13);
        schedule.setTargetGoal(dw::Weekday::Wednesday, 13);
        schedule.setTargetGoal(dw::Weekday::Thursday, 13);
        schedule.setTargetGoal(dw::Weekday::Friday, 13);
        workdayTracker.addWeekSchedule(period.start() - Years{1}, schedule);
    }

    WeekSchedule schedule;
    WorkdayTracker workdayTracker;
    ConfigMock configMock;

    const DateRange period{Date{Year{2019}, Month{1}, Day{9}},
                           Date{Year{2019}, Month{2}, Day{14}}};
    sprint_timer::GroupByWeek groupByWeekStrategy{configMock};
};

TEST_F(ProgressByWeekFixture, handles_empty_actual_progress)
{
    const std::vector<int> actualProgress;
    const std::vector<GoalProgress> expected{
        {39, 0}, {65, 0}, {65, 0}, {65, 0}, {65, 0}, {52, 0}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdayTracker, groupByWeekStrategy};

    EXPECT_EQ(0, progress.actual());
    EXPECT_EQ(351, progress.estimated());
    EXPECT_NEAR(0, *progress.percentage(), 0.1);
    EXPECT_EQ(351, progress.difference());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(0, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(6, progress.size());

    for (size_t i = 0; i < expected.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(ProgressByWeekFixture, underwork)
{
    const std::vector<int> actualProgress{15, 13, 14, 0, 10, 12};
    const std::vector<GoalProgress> expected{
        {39, 15}, {65, 13}, {65, 14}, {65, 0}, {65, 10}, {52, 12}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdayTracker, groupByWeekStrategy};

    EXPECT_EQ(64, progress.actual());
    EXPECT_EQ(351, progress.estimated());
    EXPECT_NEAR(18.2, *progress.percentage(), 0.1);
    EXPECT_EQ(287, progress.difference());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(10.6, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(6, progress.size());

    for (size_t i = 0; i < actualProgress.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(ProgressByWeekFixture, overwork)
{
    const std::vector<int> actualProgress{40, 79, 67, 63, 64, 89};
    const std::vector<GoalProgress> expected{
        {39, 40}, {65, 79}, {65, 67}, {65, 63}, {65, 64}, {52, 89}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdayTracker, groupByWeekStrategy};

    EXPECT_EQ(402, progress.actual());
    EXPECT_EQ(351, progress.estimated());
    EXPECT_NEAR(114.5, *progress.percentage(), 0.1);
    EXPECT_EQ(-51, progress.difference());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_NEAR(67, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(6, progress.size());

    for (size_t i = 0; i < actualProgress.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(ProgressByWeekFixture, work_during_vacation)
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
    WorkdayTracker vacationTracker;
    vacationTracker.addWeekSchedule(start, vacationSchedule);
    const std::vector<int> actualProgress{30, 65, 65, 50, 64, 60};
    const std::vector<GoalProgress> expected{
        {0, 30}, {0, 65}, {0, 65}, {0, 50}, {0, 64}, {0, 60}};

    const ProgressOverPeriod progress{
        period, actualProgress, vacationTracker, groupByWeekStrategy};

    EXPECT_EQ(334, progress.actual());
    EXPECT_EQ(0, progress.estimated());
    EXPECT_FALSE(progress.percentage());
    EXPECT_EQ(-334, progress.difference());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_FALSE(progress.averagePerGroupPeriod());
    EXPECT_EQ(6, progress.size());

    for (size_t i = 0; i < actualProgress.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(ProgressByWeekFixture,
       handles_corner_case_when_starting_from_grouping_day)
{
    const std::vector<int> actualProgress{30, 65};
    const std::vector<GoalProgress> expected{{65, 30}, {13, 65}};
    const DateRange a_period{Date{Year{2019}, Month{1}, Day{13}},
                             Date{Year{2019}, Month{1}, Day{21}}};
    const sprint_timer::GroupByWeek strategy{configMock};
    ON_CALL(configMock, firstDayOfWeek())
        .WillByDefault(Return(dw::Weekday::Sunday));

    const ProgressOverPeriod progress{
        a_period, actualProgress, workdayTracker, strategy};

    EXPECT_EQ(95, progress.actual());
    EXPECT_EQ(78, progress.estimated());
    EXPECT_NEAR(121.7, *progress.percentage(), 0.1);
    EXPECT_EQ(-17, progress.difference());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_NEAR(47.5, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(2, progress.size());

    for (size_t i = 0; i < actualProgress.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(ProgressByWeekFixture, handles_corner_case_when_ending_by_grouping_day)
{
    const std::vector<int> actualProgress{30, 65};
    const std::vector<GoalProgress> expected{{52, 30}, {65, 65}};
    const DateRange a_period{Date{Year{2019}, Month{1}, Day{8}},
                             Date{Year{2019}, Month{1}, Day{19}}};
    const sprint_timer::GroupByWeek strategy{configMock};
    ON_CALL(configMock, firstDayOfWeek())
        .WillByDefault(Return(dw::Weekday::Sunday));

    const ProgressOverPeriod progress{
        a_period, actualProgress, workdayTracker, strategy};

    EXPECT_EQ(95, progress.actual());
    EXPECT_EQ(117, progress.estimated());
    EXPECT_NEAR(81.2, *progress.percentage(), 0.1);
    EXPECT_EQ(22, progress.difference());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(47.5, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(2, progress.size());

    for (size_t i = 0; i < actualProgress.size(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}
