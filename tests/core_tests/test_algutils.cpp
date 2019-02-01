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

#include "core/utils/StringUtils.h"
#include "gtest/gtest.h"
#include <core/utils/Algutils.h>

using namespace sprint_timer::utils;

class AlgutilsFixture : public ::testing::Test {
public:
    std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
};

TEST_F(AlgutilsFixture, sliding_one_element_to_one_pos_to_the_right)
{
    std::vector<int> expected{1, 3, 2, 4, 5, 6, 7, 8, 9, 10};

    auto p = slide(data.begin() + 1, data.begin() + 2, data.begin() + 3);

    EXPECT_EQ(data.begin() + 2, p.first);
    EXPECT_EQ(data.begin() + 3, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_single_element_to_the_right)
{
    std::vector<int> expected{1, 3, 4, 2, 5, 6, 7, 8, 9, 10};

    auto p = slide(data.begin() + 1, data.begin() + 2, data.begin() + 4);

    EXPECT_EQ(data.begin() + 3, p.first);
    EXPECT_EQ(data.begin() + 4, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_last_element_to_the_right)
{
    std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto p = slide(data.begin() + 9, data.begin() + 10, data.begin() + 10);

    EXPECT_EQ(data.begin() + 9, p.first);
    EXPECT_EQ(data.begin() + 10, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_single_element_to_the_left)
{
    std::vector<int> expected{1, 9, 2, 3, 4, 5, 6, 7, 8, 10};

    auto p = slide(data.begin() + 8, data.begin() + 9, data.begin() + 1);

    EXPECT_EQ(data.begin() + 1, p.first);
    EXPECT_EQ(data.begin() + 2, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_first_element_to_the_left)
{
    std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto p = slide(data.begin(), data.begin() + 1, data.begin());

    EXPECT_EQ(data.begin(), p.first);
    EXPECT_EQ(data.begin() + 1, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_group_of_elements_to_the_right)
{
    // std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected{1, 2, 6, 7, 8, 9, 3, 4, 5, 10};

    auto p = slide(data.begin() + 2, data.begin() + 5, data.begin() + 9);

    EXPECT_EQ(data.begin() + 6, p.first);
    EXPECT_EQ(data.begin() + 9, p.second);
    EXPECT_EQ(expected, data);
}


TEST_F(AlgutilsFixture, sliding_group_of_elements_to_the_left)
{
    std::vector<int> expected{1, 7, 8, 9, 2, 3, 4, 5, 6, 10};

    auto p = slide(data.begin() + 6, data.begin() + 9, data.begin() + 1);

    EXPECT_EQ(data.begin() + 1, p.first);
    EXPECT_EQ(data.begin() + 4, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_segment_inside_itself_does_nothing)
{
    std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto p = slide(data.begin() + 1, data.begin() + 4, data.begin() + 1);

    EXPECT_EQ(data.begin() + 1, p.first);
    EXPECT_EQ(data.begin() + 4, p.second);
    EXPECT_EQ(expected, data);
}
