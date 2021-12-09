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

#include "core/utils/Algutils.h"
#include "core/utils/StringUtils.h"
#include "gtest/gtest.h"

namespace {

auto differ_by_one = [](int left, int right) {
    return std::abs(right - left) == 1;
};

} // namespace

using namespace sprint_timer;

class AlgutilsFixture : public ::testing::Test {
public:
    std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
};

TEST_F(AlgutilsFixture, sliding_one_element_to_one_pos_to_the_right)
{
    std::vector<int> expected{1, 3, 2, 4, 5, 6, 7, 8, 9, 10};

    auto p = utils::slide(data.begin() + 1, data.begin() + 2, data.begin() + 3);

    EXPECT_EQ(data.begin() + 2, p.first);
    EXPECT_EQ(data.begin() + 3, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_single_element_to_the_right)
{
    std::vector<int> expected{1, 3, 4, 2, 5, 6, 7, 8, 9, 10};

    auto p = utils::slide(data.begin() + 1, data.begin() + 2, data.begin() + 4);

    EXPECT_EQ(data.begin() + 3, p.first);
    EXPECT_EQ(data.begin() + 4, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_last_element_to_the_right)
{
    std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto p =
        utils::slide(data.begin() + 9, data.begin() + 10, data.begin() + 10);

    EXPECT_EQ(data.begin() + 9, p.first);
    EXPECT_EQ(data.begin() + 10, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_first_element_to_the_back)
{
    std::vector<int> expected{2, 3, 4, 5, 6, 7, 8, 9, 10, 1};

    auto p = utils::slide(data.begin(), data.begin() + 1, data.begin() + 10);

    EXPECT_EQ(data.begin() + 9, p.first);
    EXPECT_EQ(data.begin() + 10, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_last_element_to_the_front)
{
    std::vector<int> expected{10, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto p = utils::slide(data.begin() + 9, data.begin() + 10, data.begin());

    EXPECT_EQ(data.begin(), p.first);
    EXPECT_EQ(data.begin() + 1, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_single_element_to_the_left)
{
    std::vector<int> expected{1, 9, 2, 3, 4, 5, 6, 7, 8, 10};

    auto p = utils::slide(data.begin() + 8, data.begin() + 9, data.begin() + 1);

    EXPECT_EQ(data.begin() + 1, p.first);
    EXPECT_EQ(data.begin() + 2, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_first_element_to_the_left)
{
    std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto p = utils::slide(data.begin(), data.begin() + 1, data.begin());

    EXPECT_EQ(data.begin(), p.first);
    EXPECT_EQ(data.begin() + 1, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_group_of_elements_to_the_right)
{
    std::vector<int> expected{1, 2, 6, 7, 8, 9, 3, 4, 5, 10};

    auto p = utils::slide(data.begin() + 2, data.begin() + 5, data.begin() + 9);

    EXPECT_EQ(data.begin() + 6, p.first);
    EXPECT_EQ(data.begin() + 9, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_group_of_elements_to_the_left)
{
    std::vector<int> expected{1, 7, 8, 9, 2, 3, 4, 5, 6, 10};

    auto p = utils::slide(data.begin() + 6, data.begin() + 9, data.begin() + 1);

    EXPECT_EQ(data.begin() + 1, p.first);
    EXPECT_EQ(data.begin() + 4, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, sliding_segment_inside_itself_does_nothing)
{
    std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto p = utils::slide(data.begin() + 1, data.begin() + 4, data.begin() + 1);

    EXPECT_EQ(data.begin() + 1, p.first);
    EXPECT_EQ(data.begin() + 4, p.second);
    EXPECT_EQ(expected, data);
}

TEST_F(AlgutilsFixture, find_all_adjacent_matches_in_empty_container)
{
    const std::vector<int> values;
    std::vector<std::pair<int, int>> output;

    utils::find_all_adjacent_matches(cbegin(values),
                                     cend(values),
                                     std::back_inserter(output),
                                     differ_by_one);

    EXPECT_TRUE(output.empty());
}

TEST_F(AlgutilsFixture, find_all_adjacent_matches_in_singleton_container)
{
    const std::vector<int> values{8};
    std::vector<std::pair<int, int>> output;

    utils::find_all_adjacent_matches(cbegin(values),
                                     cend(values),
                                     std::back_inserter(output),
                                     differ_by_one);

    EXPECT_TRUE(output.empty());
}

TEST_F(AlgutilsFixture, find_all_elements_fully_mismatched_contaner)
{
    const std::vector<int> values{2, 4, 6, 8};
    std::vector<std::pair<int, int>> output;

    utils::find_all_adjacent_matches(cbegin(values),
                                     cend(values),
                                     std::back_inserter(output),
                                     differ_by_one);

    EXPECT_TRUE(output.empty());
}

TEST_F(AlgutilsFixture, find_all_elements_in_fully_matched_container)
{
    const std::vector<int> values{1, 2, 3, 4};
    const std::vector<std::pair<int, int>> expected{{1, 2}, {2, 3}, {3, 4}};
    std::vector<std::pair<int, int>> output;

    utils::find_all_adjacent_matches(cbegin(values),
                                     cend(values),
                                     std::back_inserter(output),
                                     differ_by_one);

    EXPECT_EQ(expected, output);
}

TEST_F(AlgutilsFixture, find_all_elements_in_mixed_container)
{
    const std::vector<int> values{2, 1, 4, 7, 5, 6, 2};
    const std::vector<std::pair<int, int>> expected{{2, 1}, {5, 6}};
    std::vector<std::pair<int, int>> output;

    utils::find_all_adjacent_matches(cbegin(values),
                                     cend(values),
                                     std::back_inserter(output),
                                     differ_by_one);

    EXPECT_EQ(expected, output);
}

TEST_F(AlgutilsFixture, fold_empty_sequence)
{
    const std::vector<int> values;
    const auto init = 7;

    const auto actual = ranges_ext::fold(values, 7, std::plus<int>{});

    EXPECT_EQ(init, actual);
}

TEST_F(AlgutilsFixture, fold_sequence)
{
    const std::vector<int> values{1, 2, 3, 4};
    const int expected{10};

    const auto actual = ranges_ext::fold(values, 0, std::plus<int>{});

    EXPECT_EQ(expected, actual);
}

TEST_F(AlgutilsFixture, fold_sequence_with_projection)
{
    const std::vector<int> values{1, 2, 3, 4};
    const int expected{30};

    const auto actual = ranges_ext::fold(
        values, 0, std::plus<int>{}, [](auto val) { return val * val; });

    EXPECT_EQ(expected, actual);
}

TEST_F(AlgutilsFixture, fold_to_different_type)
{
    const std::vector<int> values{1, 2, 3, 4};
    const std::string expected{"1, 2, 3, 4, "};

    const auto actual = ranges_ext::fold(
        values, std::string{}, [](const auto& acc, const auto& val) {
            return acc + std::to_string(val) + ", ";
        });

    EXPECT_EQ(expected, actual);
}

// TEST_F(AlgutilsFixture, adjacent_view)
// {
//     // auto ints = std::views::iota(1, 6);
//     std::vector<int> ints{1, 2, 3, 4, 5};
//     auto adj = ranges_ext::adjacent_view{ints};
//     using R = decltype(adj);
//     // static_assert(std::ranges::random_access_range<R>);
//     static_assert(std::ranges::view<R>);
//
//     // EXPECT_EQ(adj, ({{1, 2}, {2, 3}, {3, 4}, {4, 5}}));
// }
