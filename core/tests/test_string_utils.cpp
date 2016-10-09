/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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
#include "core/StringUtils.h"
#include <TestHarness.h>

TEST_GROUP(TestStringUtils){

};

TEST(TestStringUtils, test_parse_words)
{
    std::string text{
        "Some    text1, ?that! should?Be,parsed... in C++ ..D--  "};
    std::list<std::string> expected{
        "Some", "text1", "that", "should", "Be", "parsed", "in", "C++", "D--"};

    auto actual = StringUtils::parseWords(text);

    CHECK_EQUAL(expected.size(), actual.size())
    CHECK(std::equal(expected.begin(), expected.end(), actual.begin()));
}

TEST(TestStringUtils, test_join_with_some_parts_empty)
{
    std::vector<std::string> parts{"Some", "", "are", "empty"};
    std::string expected{"Some are empty"};

    std::string actual = StringUtils::join(parts, " ");

    CHECK_EQUAL(expected.size(), actual.size())
    CHECK(std::equal(expected.begin(), expected.end(), actual.begin()));
}

TEST(TestStringUtils, test_join_with_all_parts_empty)
{
    std::vector<std::string> parts{"", "", "", ""};
    std::string expected{""};

    std::string actual = StringUtils::join(parts, " ");

    CHECK_EQUAL(expected.size(), actual.size())
    CHECK(std::equal(expected.begin(), expected.end(), actual.begin()));
}

TEST(TestStringUtils, test_join_on_list_of_ints)
{
    std::list<int> parts{1, 2, 3, 4};
    std::string expected{"1 -> 2 -> 3 -> 4"};

    std::string actual
        = StringUtils::join(parts.cbegin(), parts.cend(), " -> ");

    CHECK_EQUAL(expected.size(), actual.size())
    CHECK(std::equal(expected.begin(), expected.end(), actual.begin()));
}

TEST(TestStringUtils, test_starts_with)
{
    CHECK(StringUtils::startsWith("whatever", "what"));
    CHECK(StringUtils::startsWith("what", ""));
    CHECK(StringUtils::startsWith("", ""));

    CHECK(!StringUtils::startsWith("what", "whatever"));
    CHECK(!StringUtils::startsWith("", "what"));
}

TEST(TestStringUtils, test_ends_with)
{
    CHECK(StringUtils::endsWith("whatever", "ever"));
    CHECK(StringUtils::endsWith("whatever", ""));
    CHECK(StringUtils::endsWith("", ""));

    CHECK(!StringUtils::endsWith("whatever", "everest"));
    CHECK(!StringUtils::endsWith("", "ever"));
    CHECK(!StringUtils::endsWith("ever", "whatever"));
}
