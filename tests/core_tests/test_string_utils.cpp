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

#include "core/utils/StringUtils.h"
#include "gtest/gtest.h"

using namespace sprint_timer;
using namespace sprint_timer::utils;

TEST(TestStringUtils, test_parse_words)
{
    const std::string text{
        "Some    text1, ?that! should?Be,parsed... in C++ ..D--  "};
    const std::vector<std::string> expected{
        "Some", "text1", "that", "should", "Be", "parsed", "in", "C++", "D--"};

    std::vector<std::string> actual;
    parseWords(cbegin(text), cend(text), std::back_inserter(actual));

    EXPECT_EQ(expected, actual);
}

TEST(TestStringUtils, test_join_empty_container)
{
    std::vector<std::string> parts;
    const std::string expected{""};

    const std::string actual = join(parts, ", ");

    EXPECT_EQ(expected, actual);
}

TEST(TestStringUtils, test_join_with_some_parts_empty)
{
    const std::vector<std::string> parts{"Some", "", "are", "empty"};
    const std::string expected{"Some are empty"};

    std::string actual = join(parts, " ");

    EXPECT_EQ(expected, actual);
}

TEST(TestStringUtils, test_join_with_all_parts_empty)
{
    const std::vector<std::string> parts{"", "", "", ""};
    const std::string expected{""};

    const std::string actual = join(parts, " ");

    EXPECT_EQ(expected, actual);
}

TEST(TestStringUtils, test_join_on_list_of_ints)
{
    const std::list<int> parts{1, 2, 3, 4};
    const std::string expected{"1 -> 2 -> 3 -> 4"};

    const std::string actual = join(parts.cbegin(), parts.cend(), " -> ");

    EXPECT_EQ(expected, actual);
}

TEST(TestStringUtils, test_transform_join_on_strings)
{
    const std::vector<std::string> parts{"Some", "", "are", "empty"};
    const std::string expected{"#Some #are #empty"};

    const std::string actual =
        transformJoin(cbegin(parts), cend(parts), " ", [](const auto& elem) {
            std::string res{"#"};
            res += elem;
            return res;
        });

    EXPECT_EQ(expected, actual);
}

TEST(TestStringUtils, test_transform_join_on_ints)
{
    const std::vector<int> parts{1, 2, 3, 4};
    const std::string expected{"1 + 4 + 9 + 16"};

    const std::string actual = transformJoin(
        parts.cbegin(), parts.cend(), " + ", [](int num) { return num * num; });

    EXPECT_EQ(expected, actual);
}

TEST(TestStringUtils, test_transform_join_string_vec)
{
    const std::vector<std::string> parts{"Some", "", "are", "empty"};
    const std::string expected{"#Some #are #empty"};

    const std::string actual = transformJoin(parts, " ", [](const auto& elem) {
        std::string res{"#"};
        res += elem;
        return res;
    });

    EXPECT_EQ(expected, actual);
}

TEST(TestStringUtils, test_starts_with)
{
    EXPECT_TRUE(startsWith("whatever", "what"));
    EXPECT_TRUE(startsWith("what", ""));
    EXPECT_TRUE(startsWith("", ""));

    EXPECT_FALSE(startsWith("what", "whatever"));
    EXPECT_FALSE(startsWith("", "what"));
}

TEST(TestStringUtils, test_ends_with)
{
    EXPECT_TRUE(endsWith("whatever", "ever"));
    EXPECT_TRUE(endsWith("whatever", ""));
    EXPECT_TRUE(endsWith("", ""));

    EXPECT_FALSE(endsWith("whatever", "everest"));
    EXPECT_FALSE(endsWith("", "ever"));
    EXPECT_FALSE(endsWith("ever", "whatever"));
}
