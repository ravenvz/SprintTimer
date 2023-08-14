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
#include "core/TagTop.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
#include <ranges>

using sprint_timer::Tag;
using sprint_timer::TagTop;
using namespace ::testing;

namespace {

constexpr std::chrono::minutes sprintDuration{25};

struct SprintFactory {
    SprintFactory(dw::DateTimeRange timeSpan_,
                  std::vector<sprint_timer::Tag> tags_)
        : timeSpan{timeSpan_}
        , tags{tags_}
    {
    }

    sprint_timer::TagTop::sprint_tags_t operator()()
    {
        return {dw::add_offset(timeSpan, iteration++ * sprintDuration), tags};
    }

private:
    dw::DateTimeRange timeSpan;
    std::vector<sprint_timer::Tag> tags;
    int iteration{0};
};

std::vector<sprint_timer::TagTop::sprint_tags_t> buildSomeSprints();

bool contains(
    const std::pair<dw::DateTimeRange, std::vector<sprint_timer::Tag>>& element,
    const sprint_timer::Tag& value)
{
    return std::ranges::find(element.second, value) !=
           std::cend(element.second);
}

std::vector<dw::DateTimeRange> findSprintsWithTag(
    std::span<const std::pair<dw::DateTimeRange,
                              std::vector<sprint_timer::Tag>>> sprints,
    const sprint_timer::Tag& tag)
{
    std::vector<dw::DateTimeRange> sprintsForTag;
    std::ranges::copy(sprints | std::views::filter([&](const auto& el) {
                          return contains(el, tag);
                      }) | std::views::elements<0>,
                      std::back_inserter(sprintsForTag));
    return sprintsForTag;
}

} // namespace

class TestTopTagFixture : public ::testing::Test {
public:
    const std::vector<sprint_timer::TagTop::sprint_tags_t> sprints =
        buildSomeSprints();
};

TEST_F(TestTopTagFixture, finds_top_tags_when_top_size_is_greater_than_num_tags)
{
    const size_t numTopTags{10};
    const TagTop sut{sprints, numTopTags};

    const auto actual = sut.tagFrequencies();

    EXPECT_THAT(actual,
                ElementsAre(Pair(Tag{"Tag1"}, DoubleNear(0.24, 0.01)),
                            Pair(Tag{"Tag2"}, DoubleNear(0.217, 0.01)),
                            Pair(Tag{"Tag8"}, DoubleNear(0.132, 0.01)),
                            Pair(Tag{"Tag9"}, DoubleNear(0.12, 0.01)),
                            Pair(Tag{"Tag3"}, DoubleNear(0.108, 0.01)),
                            Pair(Tag{"Tag4"}, DoubleNear(0.096, 0.01)),
                            Pair(Tag{"Tag5"}, DoubleNear(0.084, 0.01))));
}

TEST_F(TestTopTagFixture,
       compresses_least_frequent_tag_when_top_size_is_smaller_than_num_tags)
{
    const size_t numTopTags{4};
    const TagTop sut{sprints, numTopTags};

    const auto actual = sut.tagFrequencies();

    EXPECT_THAT(actual,
                ElementsAre(Pair(Tag{"Tag1"}, DoubleNear(0.24, 0.01)),
                            Pair(Tag{"Tag2"}, DoubleNear(0.217, 0.01)),
                            Pair(Tag{"Tag8"}, DoubleNear(0.132, 0.01)),
                            Pair(Tag{""}, DoubleNear(0.411, 0.01))));
}

TEST_F(TestTopTagFixture,
       renames_least_frequent_tag_when_top_size_equals_to_num_tags)
{
    const size_t numTopTags{7};
    const TagTop sut{sprints, numTopTags};

    const auto actual = sut.tagFrequencies();

    EXPECT_THAT(actual,
                ElementsAre(Pair(Tag{"Tag1"}, DoubleNear(0.24, 0.01)),
                            Pair(Tag{"Tag2"}, DoubleNear(0.217, 0.01)),
                            Pair(Tag{"Tag8"}, DoubleNear(0.132, 0.01)),
                            Pair(Tag{"Tag9"}, DoubleNear(0.12, 0.01)),
                            Pair(Tag{"Tag3"}, DoubleNear(0.108, 0.01)),
                            Pair(Tag{"Tag4"}, DoubleNear(0.096, 0.01)),
                            Pair(Tag{""}, DoubleNear(0.084, 0.01))));
}

TEST_F(TestTopTagFixture, when_top_size_is_zero_combines_all_tags_in_one)
{
    const size_t numTopTags{0};
    const TagTop sut{sprints, numTopTags};

    const auto actual = sut.tagFrequencies();

    EXPECT_THAT(actual, ElementsAre(Pair(Tag{""}, DoubleNear(1.0, 0.01))));
}

TEST_F(TestTopTagFixture, returns_sprints_for_given_tag)
{
    const size_t numTopTags{4};
    const auto sprintsForTag1{findSprintsWithTag(sprints, Tag{"Tag1"})};
    const auto sprintsForTag2{findSprintsWithTag(sprints, Tag{"Tag2"})};
    const auto sprintsForTag8{findSprintsWithTag(sprints, Tag{"Tag8"})};
    std::vector<dw::DateTimeRange> others;
    std::ranges::copy(sprints | std::views::drop(11) | std::views::elements<0>,
                      std::back_inserter(others));

    const TagTop sut{sprints, numTopTags};

    EXPECT_EQ(sprintsForTag1, sut.sprintsForTagAt(0));
    EXPECT_EQ(sprintsForTag2, sut.sprintsForTagAt(1));
    EXPECT_EQ(sprintsForTag8, sut.sprintsForTagAt(2));
    EXPECT_EQ(others, sut.sprintsForTagAt(3));
}

namespace {

std::vector<sprint_timer::TagTop::sprint_tags_t> buildSomeSprints()
{
    using sprint_timer::Tag;
    using sprint_timer::TagTop;
    std::vector<sprint_timer::TagTop::sprint_tags_t> result;
    const dw::DateTimeRange someTimeSpan{
        dw::current_date_time(), dw::current_date_time() + sprintDuration};
    std::ranges::generate_n(
        std::back_inserter(result),
        11,
        SprintFactory(someTimeSpan, {Tag{"Tag1"}, Tag{"Tag8"}}));
    std::ranges::generate_n(
        std::back_inserter(result),
        10,
        SprintFactory(dw::add_offset(someTimeSpan, dw::Days{1}),
                      {Tag{"Tag2"}, Tag{"Tag9"}}));
    std::ranges::generate_n(
        std::back_inserter(result),
        9,
        SprintFactory(dw::add_offset(someTimeSpan, dw::Days{2}),
                      {Tag{"Tag3"}, Tag{"Tag1"}}));
    std::ranges::generate_n(
        std::back_inserter(result),
        8,
        SprintFactory(dw::add_offset(someTimeSpan, dw::Days{3}),
                      {Tag{"Tag4"}, Tag{"Tag2"}}));
    std::ranges::generate_n(
        std::back_inserter(result),
        7,
        SprintFactory(dw::add_offset(someTimeSpan, dw::Days{4}),
                      {Tag{"Tag5"}}));
    return result;
}

} // namespace
