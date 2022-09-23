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

TEST_F(TreeFixture, is_copy_constructible)
{
    const auto actual = sut;

    EXPECT_EQ(sut, actual);
}

TEST_F(TreeFixture, is_copy_assignable)
{
    sprint_timer::Tree<std::string, int> actual;

    actual = sut;

    EXPECT_EQ(sut, actual);
}

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

TEST_F(TreeFixture, add_child_throws_when_parent_is_bogus) {
    sprint_timer::Tree<std::string, int> tree;
    tree.addChild("1", 1, std::nullopt);

    EXPECT_THROW(tree.addChild("2", 2, "3"), std::runtime_error);
}

TEST_F(TreeFixture, flatten_and_unflatten)
{
    const auto flattened = sut.flatten();
    const auto restored =
        sprint_timer::Tree<std::string, int>::unflatten(flattened);

    EXPECT_EQ(sut, restored);
}

TEST_F(TreeFixture, returns_none_when_asked_for_payload_for_missing_key)
{
    EXPECT_EQ(std::nullopt, sut.payload("bogus_key"));
}

TEST_F(TreeFixture, returns_payload_for_given_key)
{
    EXPECT_EQ(8, sut.payload("8"));
    EXPECT_EQ(1, sut.payload("1"));
}

TEST_F(TreeFixture, thows_when_asked_for_parent_of_bogus_child)
{
    EXPECT_THROW(sut.parent("bogus_id"), std::runtime_error);
}

TEST_F(TreeFixture, returns_null_when_asked_for_parent_of_top_level_child)
{
    EXPECT_EQ(std::nullopt, sut.parent("4"));
}

TEST_F(TreeFixture, returns_some_key_when_asked_for_parent_of_lower_level_child)
{
    EXPECT_EQ("7", sut.parent("8").value().get());
}

TEST_F(TreeFixture, returns_top_level_when_asked_for_children_for_bogus_key)
{
    const std::vector<std::string> expected{"1", "4", "9"};

    EXPECT_TRUE(std::ranges::equal(expected, sut.children("bogus_key")));
}

TEST_F(TreeFixture, returns_top_level_children)
{
    const std::vector<std::string> expected{"1", "4", "9"};

    EXPECT_TRUE(std::ranges::equal(expected, sut.children()));
}

TEST_F(TreeFixture, returns_children_for_existing_key)
{
    const std::vector<std::string> expected{"6", "7"};

    EXPECT_TRUE(std::ranges::equal(expected, sut.children("5")));
}

TEST_F(TreeFixture, returns_none_when_asked_for_nth_child_of_missing_key)
{
    EXPECT_EQ(std::nullopt, sut.nthChild("bogus_key", 0));
}

TEST_F(TreeFixture,
       returns_none_when_asked_for_nth_child_of_existing_key_but_out_of_bounds)
{

    EXPECT_EQ(std::nullopt, sut.nthChild("5", 2));
    EXPECT_EQ(std::nullopt, sut.nthChild(3));
}

TEST_F(TreeFixture, returns_nth_child)
{
    EXPECT_EQ(7, sut.nthChild("5", 1));
    EXPECT_EQ(9, sut.nthChild(2));
}

TEST_F(TreeFixture, returns_node_position_in_parent_children)
{
    EXPECT_EQ(std::optional<size_t>{2}, sut.positionInChildren("9"));
    EXPECT_EQ(std::optional<size_t>{1}, sut.positionInChildren("7"));
    EXPECT_EQ(std::optional<size_t>{0}, sut.positionInChildren("5"));
    EXPECT_EQ(std::optional<size_t>{}, sut.positionInChildren("bogus_key"));
}

TEST_F(TreeFixture, returns_keys_in_unspecified_order)
{
    std::vector<std::string> expected{
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    std::vector<std::string> actual;
    std::ranges::copy(sut.keys(), std::back_inserter(actual));

    EXPECT_THAT(actual, ::testing::UnorderedElementsAreArray(expected));
}

TEST_F(TreeFixture, removing_root_nodes)
{
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
     *     11
     *     12
     *       14
     *     13
     * 9
     */

    /*
     * 1
     *   2
     *     10
     *   3
     * 4
     *   5
     *     6
     *     12
     *       14
     *     13
     * 9
     */
    sut.addChild("11", 11, "5");
    sut.addChild("12", 12, "5");
    sut.addChild("14", 14, "12");
    sut.addChild("13", 13, "5");
    sprint_timer::Tree<std::string, int> expected;
    expected.addChild("1", 1, std::nullopt);
    expected.addChild("2", 2, "1");
    expected.addChild("10", 10, "2");
    expected.addChild("3", 3, "1");
    expected.addChild("4", 4, std::nullopt);
    expected.addChild("5", 5, "4");
    expected.addChild("6", 6, "5");
    expected.addChild("12", 12, "5");
    expected.addChild("14", 14, "12");
    expected.addChild("13", 13, "5");
    expected.addChild("9", 9, std::nullopt);

    sut.removeNodes("5", 1, 2);

    EXPECT_EQ(expected, sut);
}

TEST_F(TreeFixture, removing_nodes_throws_when_count_is_too_large)
{
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
    EXPECT_THROW(sut.removeNodes("5", 1, 3), std::runtime_error);
}

TEST_F(TreeFixture, removing_nodes_throws_when_row_specified_does_not_exists)
{
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
    EXPECT_THROW(sut.removeNodes("5", 3, 1), std::runtime_error);
}

TEST_F(TreeFixture, moving_nodes_between_leaves)
{
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

    /*
     * 1
     * 4
     *   5
     *     6
     *     2
     *       10
     *     3
     *     7
     *       8
     * 9
     */
    sprint_timer::Tree<std::string, int> expected;
    expected.addChild("1", 1, std::nullopt);
    expected.addChild("4", 4, std::nullopt);
    expected.addChild("5", 5, "4");
    expected.addChild("6", 6, "5");
    expected.addChild("2", 2, "5");
    expected.addChild("3", 3, "5");
    expected.addChild("10", 10, "2");
    expected.addChild("7", 7, "5");
    expected.addChild("8", 8, "7");
    expected.addChild("9", 9, std::nullopt);

    sut.moveNodes("1", 0, 2, "5", 1);

    EXPECT_EQ(expected, sut);
}

TEST_F(TreeFixture, moving_root_nodes_within_root_itself)
{
    /*
     * 1
     * 2
     * 3
     * 4
     */

    /*
     * 4
     * 1
     * 2
     * 3
     */
    sprint_timer::Tree<std::string, int> actual;
    actual.addChild("1", 1, std::nullopt);
    actual.addChild("2", 2, std::nullopt);
    actual.addChild("3", 3, std::nullopt);
    actual.addChild("4", 4, std::nullopt);

    sprint_timer::Tree<std::string, int> expected;
    expected.addChild("4", 4, std::nullopt);
    expected.addChild("1", 1, std::nullopt);
    expected.addChild("2", 2, std::nullopt);
    expected.addChild("3", 3, std::nullopt);

    actual.moveNodes(std::nullopt, 0, 3, std::nullopt, 4);

    EXPECT_EQ(expected, actual);
}

TEST_F(TreeFixture, moving_root_nodes_in_reparenting_combinations) {
    /*
     * 1
     * 2
     * 3
     * 4
     */

    /*
     * 3
     *   2
     *     1
     * 4
     */

    /*
     * 1
     * 2
     * 3
     * 4
     */

    /*
     * 4
     *   3
     *     2
     *       1
     */
    sprint_timer::Tree<std::string, int> actual;
    actual.addChild("1", 1, std::nullopt);
    actual.addChild("2", 2, std::nullopt);
    actual.addChild("3", 3, std::nullopt);
    actual.addChild("4", 4, std::nullopt);

    sprint_timer::Tree<std::string, int> expected;
    expected.addChild("4", 4, std::nullopt);
    expected.addChild("3", 3, "4");
    expected.addChild("2", 2, "3");
    expected.addChild("1", 1, "2");

    actual.moveNodes(std::nullopt, 1, 1, "3", 0);
    actual.moveNodes(std::nullopt, 0, 1, "2", 0);
    actual.moveNodes(std::nullopt, 0, 1, "4", 0);

    EXPECT_EQ(expected, actual);
}

TEST_F(TreeFixture, moving_nodes_within_same_parent)
{
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

    /*
     * 1
     *   2
     *     10
     *   3
     * 4
     *   5
     *     7
     *       8
     *     6
     * 9
     */
    sprint_timer::Tree<std::string, int> expected;
    expected.addChild("1", 1, std::nullopt);
    expected.addChild("2", 2, "1");
    expected.addChild("10", 10, "2");
    expected.addChild("3", 3, "1");
    expected.addChild("4", 4, std::nullopt);
    expected.addChild("5", 5, "4");
    expected.addChild("7", 7, "5");
    expected.addChild("8", 8, "7");
    expected.addChild("6", 6, "5");
    expected.addChild("9", 9, std::nullopt);

    sut.moveNodes("5", 0, 1, "5", 2);

    EXPECT_EQ(expected, sut);
}

TEST_F(TreeFixture, moving_nodes_between_non_existing_node_throws)
{
    EXPECT_THROW(sut.moveNodes("100500", 0, 1, "2", 0), std::runtime_error);
    EXPECT_THROW(sut.moveNodes("2", 0, 1, "1000400", 0), std::runtime_error);
}

TEST_F(TreeFixture, moving_nodes_with_bogus_source_row_throws)
{
    EXPECT_THROW(sut.moveNodes("1", 5, 1, "2", 0), std::runtime_error);
}

TEST_F(TreeFixture, moving_nodes_to_non_existing_position_appends)
{
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

    /*
     * 1
     *   2
     *     10
     * 4
     *   5
     *     6
     *     7
     *       8
     *     3
     * 9
     */
    sprint_timer::Tree<std::string, int> expected;
    expected.addChild("1", 1, std::nullopt);
    expected.addChild("2", 2, "1");
    expected.addChild("10", 10, "2");
    expected.addChild("4", 4, std::nullopt);
    expected.addChild("5", 5, "4");
    expected.addChild("6", 6, "5");
    expected.addChild("7", 7, "5");
    expected.addChild("8", 8, "7");
    expected.addChild("3", 3, "5");
    expected.addChild("9", 9, std::nullopt);

    sut.moveNodes("1", 1, 1, "5", 200);

    EXPECT_EQ(expected, sut);
}
