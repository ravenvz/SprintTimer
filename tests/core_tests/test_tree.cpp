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
#include "core/Tree.h"
#include "gmock/gmock.h"

sprint_timer::Tree<std::string, int> make_sample_tree()
{
    using sprint_timer::Tree;
    Tree<std::string, int> sut;
    /*
     * 1
     *   2
     *     10
     *   3
     * 4
     *   5
     *     6
     *     7
     *       8
     * 9
     */
    sut.addChild("1", 1, std::nullopt);
    sut.addChild("2", 2, "1");
    sut.addChild("3", 3, "1");
    sut.addChild("4", 4, std::nullopt);
    sut.addChild("5", 5, "4");
    sut.addChild("6", 6, "5");
    sut.addChild("7", 7, "5");
    sut.addChild("8", 8, "7");
    sut.addChild("9", 9, std::nullopt);
    sut.addChild("10", 10, "2");
    return sut;
}

class TreeFixture : public ::testing::Test {
public:
    sprint_timer::Tree<std::string, int> sut{make_sample_tree()};
};

TEST_F(TreeFixture, able_to_transform_tree_to_tree_with_another_payload_type)
{
    sprint_timer::Tree<std::string, std::string> expected;
    expected.addChild("1", "1", std::nullopt);
    expected.addChild("2", "4", "1");
    expected.addChild("3", "9", "1");
    expected.addChild("4", "16", std::nullopt);
    expected.addChild("5", "25", "4");
    expected.addChild("6", "36", "5");
    expected.addChild("7", "49", "5");
    expected.addChild("8", "64", "7");
    expected.addChild("9", "81", std::nullopt);
    expected.addChild("10", "100", "2");
    auto square = [](const int& x) { return std::to_string(x * x); };

    sprint_timer::Tree<std::string, std::string> actual{
        sut.mapped<std::string>(square)};

    EXPECT_EQ(expected, actual);
}

TEST_F(TreeFixture, flatten_and_unflatten)
{
    const auto flattened = sut.flatten();
    const auto restored =
        sprint_timer::Tree<std::string, int>::unflatten(flattened);

    EXPECT_EQ(sut, restored);
}
