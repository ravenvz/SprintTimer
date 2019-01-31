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

#include "core/GoalProgress.h"
#include "core/GroupByDay.h"
#include "core/GroupByMonth.h"
#include "core/GroupByWeek.h"
#include "gtest/gtest.h"
#include <core/ProgressProto.h>
#include <core/GoalProgress.h>

using sprint_timer::Distribution;
using sprint_timer::GoalProgress;
using sprint_timer::ProgressOverPeriod;


class GroupByDayProgressFixture : public ::testing::Test {
public:
    sprint_timer::utils::WeekdaySelection workdays{31}; // Mon - Fri
    const dw::TimeSpan period{dw::DateTime::fromYMD(2019, 1, 30),
                              dw::DateTime::fromYMD(2019, 2, 4)};
    const int workdayGoal{13};
    sprint_timer::GroupByDay groupByDayStrategy;
};

class GroupByWeekFixture : public ::testing::Test {
public:
    sprint_timer::utils::WeekdaySelection workdays{31}; // Mon - Fri
    const dw::TimeSpan period{dw::DateTime::fromYMD(2019, 1, 9),
                              dw::DateTime::fromYMD(2019, 2, 14)};
    const int workdayGoal{13};
    sprint_timer::GroupByWeek groupByWeekStrategy{
        dw::DateTime::Weekday::Monday};
};

class GroupByMonthFixture : public ::testing::Test {
public:
    sprint_timer::utils::WeekdaySelection workdays{31}; // Mon - Fri
    const dw::TimeSpan period{dw::DateTime::fromYMD(2018, 4, 10),
                              dw::DateTime::fromYMD(2018, 8, 25)};
    const int workdayGoal{10};
    ::sprint_timer::GroupByMonth groupByMonthStrategy;
};

TEST_F(GroupByDayProgressFixture, underwork_progress)
{
    const dw::TimeSpan period{dw::DateTime::fromYMD(2019, 1, 30),
                              dw::DateTime::fromYMD(2019, 2, 4)};
    const Distribution<int> actualProgress{{13, 12, 11, 0, 0, 10}};
    const std::vector<GoalProgress> expected{
        {13, 13}, {13, 12}, {13, 11}, {0, 0}, {0, 0}, {13, 10}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdays, groupByDayStrategy, workdayGoal};

    EXPECT_EQ(46, progress.actual());
    EXPECT_DOUBLE_EQ(11.5, *progress.averagePerGroupPeriod());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(88.46, *progress.percentage(), 0.1);
    EXPECT_EQ(6, progress.difference());
    EXPECT_EQ(52, progress.estimated());
    EXPECT_EQ(6, progress.size());

    for (size_t i = 0; i < actualProgress.getNumBins(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(GroupByDayProgressFixture, overwork_progress)
{
    const dw::TimeSpan period{dw::DateTime::fromYMD(2019, 1, 30),
                              dw::DateTime::fromYMD(2019, 2, 4)};
    const Distribution<int> actualProgress{{15, 13, 14, 0, 10, 12}};
    const std::vector<GoalProgress> expected{
        {13, 15}, {13, 13}, {13, 14}, {0, 0}, {0, 10}, {13, 12}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdays, groupByDayStrategy, workdayGoal};

    EXPECT_EQ(64, progress.actual());
    EXPECT_DOUBLE_EQ(16, *progress.averagePerGroupPeriod());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_EQ(-12, progress.difference());
    EXPECT_NEAR(123.08, *progress.percentage(), 0.1);
    EXPECT_EQ(52, progress.estimated());
    EXPECT_EQ(6, progress.size());

    for (size_t i = 0; i < actualProgress.getNumBins(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(GroupByDayProgressFixture, work_during_vacation)
{
    const dw::TimeSpan period{dw::DateTime::fromYMD(2019, 1, 30),
                              dw::DateTime::fromYMD(2019, 2, 4)};
    const sprint_timer::utils::WeekdaySelection noWorkdays; // All are rest days
    const Distribution<int> actualProgress{{15, 13, 14, 0, 10, 12}};
    const std::vector<GoalProgress> expected{
        {0, 15}, {0, 13}, {0, 14}, {0, 0}, {0, 10}, {0, 12}};

    const ProgressOverPeriod progress{
        period, actualProgress, noWorkdays, groupByDayStrategy, workdayGoal};

    EXPECT_EQ(64, progress.actual());
    EXPECT_FALSE(progress.averagePerGroupPeriod());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_FALSE(progress.percentage());
    EXPECT_EQ(-64, progress.difference());
    EXPECT_EQ(0, progress.estimated());
    EXPECT_EQ(6, progress.size());

    for (size_t i = 0; i < actualProgress.getNumBins(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(GroupByWeekFixture, underwork_progress)
{
    const Distribution<int> actualProgress{{15, 13, 14, 0, 10, 12}};
    const std::vector<GoalProgress> expected{
        {39, 15}, {65, 13}, {65, 14}, {65, 0}, {65, 10}, {52, 12}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdays, groupByWeekStrategy, workdayGoal};

    EXPECT_EQ(64, progress.actual());
    EXPECT_EQ(351, progress.estimated());
    EXPECT_NEAR(18.2, *progress.percentage(), 0.1);
    EXPECT_EQ(287, progress.difference());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(10.6, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(6, progress.size());

    for (size_t i = 0; i < actualProgress.getNumBins(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(GroupByWeekFixture, overwork_progress)
{
    const Distribution<int> actualProgress{{40, 79, 67, 63, 64, 89}};
    const std::vector<GoalProgress> expected{
        {39, 40}, {65, 79}, {65, 67}, {65, 63}, {65, 64}, {52, 89}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdays, groupByWeekStrategy, workdayGoal};

    EXPECT_EQ(402, progress.actual());
    EXPECT_EQ(351, progress.estimated());
    EXPECT_NEAR(114.5, *progress.percentage(), 0.1);
    EXPECT_EQ(-51, progress.difference());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_NEAR(67, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(6, progress.size());

    for (size_t i = 0; i < actualProgress.getNumBins(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(GroupByWeekFixture, work_during_vacation)
{
    const Distribution<int> actualProgress{{30, 65, 65, 50, 64, 60}};
    const std::vector<GoalProgress> expected{
        {0, 30}, {0, 65}, {0, 65}, {0, 50}, {0, 64}, {0, 60}};

    const sprint_timer::utils::WeekdaySelection noWorkdays; // All are rest days
    const ProgressOverPeriod progress{
        period, actualProgress, noWorkdays, groupByWeekStrategy, workdayGoal};

    EXPECT_EQ(334, progress.actual());
    EXPECT_EQ(0, progress.estimated());
    EXPECT_FALSE(progress.percentage());
    EXPECT_EQ(-334, progress.difference());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_FALSE(progress.averagePerGroupPeriod());
    EXPECT_EQ(6, progress.size());

    for (size_t i = 0; i < actualProgress.getNumBins(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(GroupByWeekFixture, handles_corner_case_when_starting_from_grouping_day)
{
    const Distribution<int> actualProgress{{30, 65}};
    const std::vector<GoalProgress> expected{{65, 30}, {13, 65}};
    const dw::TimeSpan a_period{dw::DateTime::fromYMD(2019, 1, 13),
                                dw::DateTime::fromYMD(2019, 1, 21)};
    const sprint_timer::GroupByWeek strategy{dw::DateTime::Weekday::Sunday};
    const ProgressOverPeriod progress{
        a_period, actualProgress, workdays, strategy, workdayGoal};

    EXPECT_EQ(95, progress.actual());
    EXPECT_EQ(78, progress.estimated());
    EXPECT_NEAR(121.7, *progress.percentage(), 0.1);
    EXPECT_EQ(-17, progress.difference());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_NEAR(47.5, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(2, progress.size());

    for (size_t i = 0; i < actualProgress.getNumBins(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(GroupByWeekFixture, handles_corner_case_when_ending_by_grouping_day)
{
    const Distribution<int> actualProgress{{30, 65}};
    const std::vector<GoalProgress> expected{{52, 30}, {65, 65}};
    const dw::TimeSpan a_period{dw::DateTime::fromYMD(2019, 1, 8),
                                dw::DateTime::fromYMD(2019, 1, 19)};
    const sprint_timer::GroupByWeek strategy{dw::DateTime::Weekday::Sunday};
    const ProgressOverPeriod progress{
        a_period, actualProgress, workdays, strategy, workdayGoal};

    EXPECT_EQ(95, progress.actual());
    EXPECT_EQ(117, progress.estimated());
    EXPECT_NEAR(81.2, *progress.percentage(), 0.1);
    EXPECT_EQ(22, progress.difference());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(47.5, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(2, progress.size());

    for (size_t i = 0; i < actualProgress.getNumBins(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(GroupByMonthFixture, underwork_progress)
{
    const Distribution<int> actualProgress{{180, 200, 150, 170, 150}};
    const std::vector<GoalProgress> expected{
        {150, 180}, {230, 200}, {210, 150}, {220, 170}, {180, 150}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdays, groupByMonthStrategy, workdayGoal};

    EXPECT_EQ(850, progress.actual());
    EXPECT_EQ(990, progress.estimated());
    EXPECT_NEAR(85.8, *progress.percentage(), 0.1);
    EXPECT_EQ(140, progress.difference());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(170, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(5, progress.size());

    for (size_t i = 0; i < actualProgress.getNumBins(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(GroupByMonthFixture, overwork_progress)
{
    const Distribution<int> actualProgress{{180, 250, 230, 200, 190}};
    const std::vector<GoalProgress> expected{
        {150, 180}, {230, 250}, {210, 230}, {220, 200}, {180, 190}};

    const ProgressOverPeriod progress{
        period, actualProgress, workdays, groupByMonthStrategy, workdayGoal};

    EXPECT_EQ(1050, progress.actual());
    EXPECT_EQ(990, progress.estimated());
    EXPECT_NEAR(106, *progress.percentage(), 0.1);
    EXPECT_EQ(-60, progress.difference());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_NEAR(210, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(5, progress.size());

    for (size_t i = 0; i < actualProgress.getNumBins(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(GroupByMonthFixture, work_during_vacation)
{
    const Distribution<int> actualProgress{{180, 250, 230, 200, 190}};
    const std::vector<GoalProgress> expected{
        {0, 180}, {0, 250}, {0, 230}, {0, 200}, {0, 190}};
    const sprint_timer::utils::WeekdaySelection noWorkdays; // All are rest days

    const ProgressOverPeriod progress{
        period, actualProgress, noWorkdays, groupByMonthStrategy, workdayGoal};

    EXPECT_EQ(1050, progress.actual());
    EXPECT_EQ(0, progress.estimated());
    EXPECT_FALSE(progress.percentage());
    EXPECT_EQ(-1050, progress.difference());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_FALSE(progress.averagePerGroupPeriod());
    EXPECT_EQ(5, progress.size());

    for (size_t i = 0; i < actualProgress.getNumBins(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(GroupByMonthFixture,
       handles_corner_case_when_starting_from_first_day_of_month)
{
    const dw::TimeSpan a_period{dw::DateTime::fromYMD(2018, 6, 1),
                                dw::DateTime::fromYMD(2018, 7, 16)};
    const Distribution<int> actualProgress{{200, 75}};
    const std::vector<GoalProgress> expected{{210, 200}, {110, 75}};

    const ProgressOverPeriod progress{
        a_period, actualProgress, workdays, groupByMonthStrategy, workdayGoal};

    EXPECT_EQ(275, progress.actual());
    EXPECT_EQ(320, progress.estimated());
    EXPECT_NEAR(85.9, *progress.percentage(), 0.1);
    EXPECT_EQ(45, progress.difference());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(137.5, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(2, progress.size());

    for (size_t i = 0; i < actualProgress.getNumBins(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

TEST_F(GroupByMonthFixture,
       handles_corner_case_when_ending_on_the_last_day_of_month)
{
    const dw::TimeSpan a_period{dw::DateTime::fromYMD(2018, 6, 1),
                                dw::DateTime::fromYMD(2018, 7, 31)};
    const Distribution<int> actualProgress{{200, 75}};
    const std::vector<GoalProgress> expected{{210, 200}, {220, 75}};

    const ProgressOverPeriod progress{
        a_period, actualProgress, workdays, groupByMonthStrategy, workdayGoal};

    EXPECT_EQ(275, progress.actual());
    EXPECT_EQ(430, progress.estimated());
    EXPECT_NEAR(63.9, *progress.percentage(), 0.1);
    EXPECT_EQ(155, progress.difference());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(137.5, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(2, progress.size());

    for (size_t i = 0; i < actualProgress.getNumBins(); ++i)
        EXPECT_EQ(expected[i], progress.getValue(i));
}

class ProgressFixture : public ::testing::Test {
public:
};

TEST_F(ProgressFixture, exactly_met_progress)
{
    ::sprint_timer::GoalProgress progress{10, 10};

    EXPECT_EQ(10, progress.actual());
    EXPECT_EQ(10, progress.estimated());
    EXPECT_EQ(100, progress.percentage());
}
