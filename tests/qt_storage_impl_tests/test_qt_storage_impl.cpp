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

// TODO remove when Gtest drops std::tr1
// Workaround for C++17 as std::tr1 no longer available and Gtest uses it
#define GTEST_LANG_CXX11 1

#include "qt_storage_impl/utils.h"
#include <list>
#include <iterator>
#include "gtest/gtest.h"

using namespace sprint_timer::storage::utils;

TEST(TestTwoWayDiff, test_diff_left_and_right)
{
    std::list<std::string> oldTags{"Tag1", "Tag2", "Tag3"};
    std::list<std::string> newTags{"Tag2", "Tag3", "Tag4"};
    std::list<std::string> expectedRemove{"Tag1"};
    std::list<std::string> expectedInsert{"Tag4"};
    std::list<std::string> tagsToRemove;
    std::list<std::string> tagsToInsert;

    twoWayDiff(cbegin(oldTags),
               cend(oldTags),
               cbegin(newTags),
               cend(newTags),
               std::back_inserter(tagsToRemove),
               std::back_inserter(tagsToInsert));

    EXPECT_TRUE(expectedRemove == tagsToRemove);
    EXPECT_TRUE(expectedInsert == tagsToInsert);
}

TEST(TestTwoWayDiff, test_diff_left_empty)
{
    std::list<std::string> oldTags{};
    std::list<std::string> newTags{"Tag2", "Tag3", "Tag4"};
    std::list<std::string> expectedRemove{};
    std::list<std::string> expectedInsert{"Tag2", "Tag3", "Tag4"};
    std::list<std::string> tagsToRemove;
    std::list<std::string> tagsToInsert;

    twoWayDiff(cbegin(oldTags),
               cend(oldTags),
               cbegin(newTags),
               cend(newTags),
               std::back_inserter(tagsToRemove),
               std::back_inserter(tagsToInsert));

    EXPECT_TRUE(expectedRemove == tagsToRemove);
    EXPECT_TRUE(expectedInsert == tagsToInsert);
}

TEST(TestTwoWayDiff, test_diff_right_empty)
{
    std::list<std::string> oldTags{"Tag2", "Tag3", "Tag4"};
    std::list<std::string> newTags{};
    std::list<std::string> expectedRemove{"Tag2", "Tag3", "Tag4"};
    std::list<std::string> expectedInsert{};
    std::list<std::string> tagsToRemove;
    std::list<std::string> tagsToInsert;

    twoWayDiff(cbegin(oldTags),
               cend(oldTags),
               cbegin(newTags),
               cend(newTags),
               std::back_inserter(tagsToRemove),
               std::back_inserter(tagsToInsert));

    EXPECT_TRUE(expectedRemove == tagsToRemove);
    EXPECT_TRUE(expectedInsert == tagsToInsert);
}

TEST(TestTwoWayDiff, test_all_unique)
{
    std::list<std::string> oldTags{"Tag1", "Tag2", "Tag3"};
    std::list<std::string> newTags{"Tag4", "Tag5", "Tag6"};
    std::list<std::string> expectedRemove{"Tag1", "Tag2", "Tag3"};
    std::list<std::string> expectedInsert{"Tag4", "Tag5", "Tag6"};
    std::list<std::string> tagsToRemove;
    std::list<std::string> tagsToInsert;

    twoWayDiff(cbegin(oldTags),
               cend(oldTags),
               cbegin(newTags),
               cend(newTags),
               std::back_inserter(tagsToRemove),
               std::back_inserter(tagsToInsert));

    EXPECT_TRUE(expectedRemove == tagsToRemove);
    EXPECT_TRUE(expectedInsert == tagsToInsert);
}
