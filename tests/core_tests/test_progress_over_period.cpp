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
#include "gtest/gtest.h"
#include <core/ProgressOverPeriod.h>

using namespace sprint_timer;

class ProgressOverPeriodFixture : public ::testing::Test {
public:
};

TEST_F(ProgressOverPeriodFixture, handles_empty_actual_progress)
{
    const ProgressOverPeriod progress{std::vector<GoalProgress>{
        {GoalProgress::Estimated{39}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{52}, GoalProgress::Actual{0}}}};

    EXPECT_EQ(0, progress.actual());
    EXPECT_EQ(351, progress.estimated());
    EXPECT_NEAR(0, *progress.percentage(), 0.1);
    EXPECT_EQ(351, progress.difference());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(0, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(6, progress.size());
}

TEST_F(ProgressOverPeriodFixture, handles_underwork)
{
    const ProgressOverPeriod progress{std::vector<GoalProgress>{
        {GoalProgress::Estimated{39}, GoalProgress::Actual{15}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{13}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{14}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{0}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{10}},
        {GoalProgress::Estimated{52}, GoalProgress::Actual{12}}}};

    EXPECT_EQ(64, progress.actual());
    EXPECT_EQ(351, progress.estimated());
    EXPECT_NEAR(18.2, *progress.percentage(), 0.1);
    EXPECT_EQ(287, progress.difference());
    EXPECT_FALSE(progress.isOverwork());
    EXPECT_NEAR(10.6, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(6, progress.size());
}

TEST_F(ProgressOverPeriodFixture, handles_overwork)
{
    const ProgressOverPeriod progress{std::vector<GoalProgress>{
        {GoalProgress::Estimated{39}, GoalProgress::Actual{40}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{79}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{67}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{63}},
        {GoalProgress::Estimated{65}, GoalProgress::Actual{64}},
        {GoalProgress::Estimated{52}, GoalProgress::Actual{89}}}};

    EXPECT_EQ(402, progress.actual());
    EXPECT_EQ(351, progress.estimated());
    EXPECT_NEAR(114.5, *progress.percentage(), 0.1);
    EXPECT_EQ(-51, progress.difference());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_NEAR(67, *progress.averagePerGroupPeriod(), 0.1);
    EXPECT_EQ(6, progress.size());
}

TEST_F(ProgressOverPeriodFixture, handles_empty_estimated)
{
    const ProgressOverPeriod progress{std::vector<GoalProgress>{
        {GoalProgress::Estimated{0}, GoalProgress::Actual{30}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{65}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{65}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{50}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{64}},
        {GoalProgress::Estimated{0}, GoalProgress::Actual{60}}}};

    EXPECT_EQ(334, progress.actual());
    EXPECT_EQ(0, progress.estimated());
    EXPECT_FALSE(progress.percentage());
    EXPECT_EQ(-334, progress.difference());
    EXPECT_TRUE(progress.isOverwork());
    EXPECT_FALSE(progress.averagePerGroupPeriod());
    EXPECT_EQ(6, progress.size());
}

