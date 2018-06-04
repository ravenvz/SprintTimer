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

#include "core/entities/Task.h"
#include "gtest/gtest.h"

using dw::DateTime;

TEST(TestTask, constructs_task_from_string_description_where_all_parts_present)
{
    const Task item{"#Test All parts present *5"};

    EXPECT_TRUE(std::list<Tag>{Tag{"Test"}} == item.tags());
    EXPECT_EQ(std::string{"All parts present"}, item.name());
    EXPECT_EQ(5, item.estimatedCost());
}

TEST(TestTask, should_set_estimated_cost_to_one_if_non_given_when_constructing_from_description)
{
    const Task item{"#Test Todo with tag"};

    EXPECT_TRUE(std::list<Tag>{Tag{"Test"}} == item.tags());
    EXPECT_EQ(std::string{"Todo with tag"}, item.name());
    EXPECT_EQ(1, item.estimatedCost());
}

TEST(TestTask, constructs_from_description_with_no_tags_given)
{
    const Task item{"Simple todo *2"};

    EXPECT_EQ(std::string{"Simple todo"}, item.name());
    EXPECT_EQ(2, item.estimatedCost());
    EXPECT_TRUE(item.tags().empty());
}

TEST(TestTask, construct_from_description_when_no_name_given)
{
    const Task item{"#Tag #Test *4"};
    std::list<Tag> expected{Tag{"Tag"}, Tag{"Test"}};

    const auto actual = item.tags();

    EXPECT_TRUE(std::equal(expected.cbegin(), expected.cend(), actual.cbegin(), actual.cend()));
    EXPECT_EQ(4, item.estimatedCost());
}

TEST(TestTask, should_parse_only_last_num_estimated_when_constructing_from_description)
{
    const Task item{"Multiple estimated *4 *9"};

    EXPECT_EQ(9, item.estimatedCost());
}

TEST(TestTask, only_treats_words_preceeded_by_single_hash_as_tags)
{
    const Task item{"##My #tag1  #   ##    beautiful,marvelous, great   content"};

    EXPECT_TRUE(std::list<Tag>{Tag{"tag1"}} == item.tags());
    EXPECT_EQ(std::string{"##My # ## beautiful,marvelous, great content"},
                item.name());
}

TEST(TestTask, ostream_operator)
{
    const Task item{"I am item with no tags",
                     4,
                     2,
                     "uuid",
                     std::list<Tag>(),
                     false,
                     DateTime::currentDateTimeLocal()};
    const auto uuid = item.uuid();
    std::string expected{"I am item with no tags 2/4"};
    expected += " Uuid: ";
    expected += uuid;

    std::stringstream ss;
    ss << item;
    std::string actual{ss.str()};

    EXPECT_EQ(expected, actual);
}
