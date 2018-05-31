/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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

#include "core/StringUtils.h"
#include "gtest/gtest.h"

TEST(TestStringUtils, test_parse_words)
{
    const std::string text{
        "Some    text1, ?that! should?Be,parsed... in C++ ..D--  "};
    const std::list<std::string> expected{
        "Some", "text1", "that", "should", "Be", "parsed", "in", "C++", "D--"};

    const auto actual = StringUtils::parseWords(text);

    EXPECT_EQ(expected.size(), actual.size());
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
}

TEST(TestStringUtils, test_join_empty_container)
{
    std::vector<std::string> parts;
    const std::string expected{""};

    const std::string actual = StringUtils::join(parts, ", ");

    EXPECT_EQ(expected.size(), actual.size());
    EXPECT_TRUE(std::equal(expected.cbegin(), expected.cend(), actual.cbegin()));
}

TEST(TestStringUtils, test_join_with_some_parts_empty)
{
    const std::vector<std::string> parts{"Some", "", "are", "empty"};
    const std::string expected{"Some are empty"};

    std::string actual = StringUtils::join(parts, " ");

    EXPECT_EQ(expected.size(), actual.size());
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
}

TEST(TestStringUtils, test_join_with_all_parts_empty)
{
    const std::vector<std::string> parts{"", "", "", ""};
    const std::string expected{""};

    const std::string actual = StringUtils::join(parts, " ");

    EXPECT_EQ(expected.size(), actual.size());
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
}

TEST(TestStringUtils, test_join_on_list_of_ints)
{
    const std::list<int> parts{1, 2, 3, 4};
    const std::string expected{"1 -> 2 -> 3 -> 4"};

    const std::string actual
        = StringUtils::join(parts.cbegin(), parts.cend(), " -> ");

    EXPECT_EQ(expected.size(), actual.size());
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
}

TEST(TestStringUtils, test_starts_with)
{
    EXPECT_TRUE(StringUtils::startsWith("whatever", "what"));
    EXPECT_TRUE(StringUtils::startsWith("what", ""));
    EXPECT_TRUE(StringUtils::startsWith("", ""));

    EXPECT_FALSE(StringUtils::startsWith("what", "whatever"));
    EXPECT_FALSE(StringUtils::startsWith("", "what"));
}

TEST(TestStringUtils, test_ends_with)
{
    EXPECT_TRUE(StringUtils::endsWith("whatever", "ever"));
    EXPECT_TRUE(StringUtils::endsWith("whatever", ""));
    EXPECT_TRUE(StringUtils::endsWith("", ""));

    EXPECT_FALSE(StringUtils::endsWith("whatever", "everest"));
    EXPECT_FALSE(StringUtils::endsWith("", "ever"));
    EXPECT_FALSE(StringUtils::endsWith("ever", "whatever"));
}
