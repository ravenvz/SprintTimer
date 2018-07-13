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

#include "core/SprintBuilder.h"
#include "core/SprintStatistics.h"
#include "core/TagTop.h"
#include "gtest/gtest.h"

using namespace sprint_timer;
using namespace sprint_timer::entities;
using namespace std::chrono_literals;
using dw::DateTime;
using dw::TimeSpan;

namespace {
const TimeSpan defaultTimespan{DateTime::currentDateTime().add(-25min),
                               DateTime::currentDateTime()};
} // namespace

TEST(SprintStatItem, creates_reasonable_defaults_for_empty_distribution)
{
    const std::vector<double> expectedDailyDistribution{0};
    const std::vector<double> expectedWeekdayDistribution
        = std::vector<double>(7, 0);
    const std::vector<double> expectedWorktimeDistribution
        = std::vector<double>(DayPart::numParts, 0);

    const SprintStatItem statistics{std::vector<Sprint>{}, defaultTimespan};
    const auto dailyDistribution = statistics.dailyDistribution();
    const auto weekdayDistribution = statistics.weekdayDistribution();
    const auto worktimeDistribution = statistics.worktimeDistribution();

    EXPECT_EQ(expectedDailyDistribution,
              dailyDistribution.getDistributionVector());
    EXPECT_DOUBLE_EQ(0, dailyDistribution.getAverage());
    EXPECT_DOUBLE_EQ(0, dailyDistribution.getMax());
    EXPECT_DOUBLE_EQ(0, dailyDistribution.getTotal());
    EXPECT_EQ(expectedWeekdayDistribution,
              weekdayDistribution.getDistributionVector());
    EXPECT_DOUBLE_EQ(0, weekdayDistribution.getAverage());
    EXPECT_DOUBLE_EQ(0, weekdayDistribution.getMax());
    EXPECT_DOUBLE_EQ(0, weekdayDistribution.getTotal());
    EXPECT_EQ(expectedWorktimeDistribution,
              worktimeDistribution.getDistributionVector());
    EXPECT_DOUBLE_EQ(0, worktimeDistribution.getAverage());
    EXPECT_DOUBLE_EQ(0, worktimeDistribution.getMax());
    EXPECT_DOUBLE_EQ(0, worktimeDistribution.getTotal());
}

TEST(SprintStatItem, computes_daily_distribution_correctly)
{
    const DateTime start = DateTime::currentDateTime();
    const DateTime end = start.add(DateTime::Days{47});
    const TimeSpan timeSpan{start, end};
    std::vector<Sprint> sprints;
    std::vector<double> expectedDistributionVector(48, 0);
    SprintBuilder sprintBuilder;
    sprintBuilder.withTaskUuid("");
    for (size_t i = 0; i < 48; ++i) {
        for (size_t j = 0; j < i + 1; ++j) {
            const DateTime sprintDateTime = start.add(DateTime::Days{i});
            const TimeSpan sprintInterval{sprintDateTime, sprintDateTime};
            sprints.push_back(
                sprintBuilder.withTimeSpan(sprintInterval).build());
            expectedDistributionVector[i]++;
        }
    }
    const double expected_average{24.5};
    const double expected_max{48};
    const size_t expected_max_value_bin{47};
    const double expected_total{1176};

    const SprintStatItem statistics{sprints, timeSpan};

    const Distribution<double> distribution = statistics.dailyDistribution();

    EXPECT_EQ(expected_max_value_bin, distribution.getMaxValueBin());
    EXPECT_DOUBLE_EQ(expected_average, distribution.getAverage());
    EXPECT_DOUBLE_EQ(expected_max, distribution.getMax());
    EXPECT_DOUBLE_EQ(expected_total, distribution.getTotal());
    EXPECT_EQ(expectedDistributionVector, distribution.getDistributionVector());
}

TEST(SprintStatItem, computes_weekday_distribution_correctly)
{
    std::vector<Sprint> increasingSprints;
    SprintBuilder sprintBuilder = SprintBuilder{}.withTaskUuid("irrelevant");
    // (2015, 6, 1) is Monday, so each weekday occures exactly twice
    // in 14-day timeSpan
    TimeSpan timeSpan{DateTime::fromYMD(2015, 6, 1),
                      DateTime::fromYMD(2015, 6, 14)};
    for (int i = 1; i < 15; ++i) {
        for (int j = 0; j < i; ++j) {
            DateTime sprintDateTime = DateTime::fromYMD(2015, 6, i);
            increasingSprints.push_back(
                sprintBuilder
                    .withTimeSpan(TimeSpan{sprintDateTime, sprintDateTime})
                    .build());
        }
    }
    const double expected_average = 7.5;
    const double expected_max = 10.5;
    const size_t expected_max_value_bin = 6;
    const std::vector<double> expectedDistribution{
        4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5};

    const SprintStatItem statistics{increasingSprints, timeSpan};
    const Distribution<double> weekdayDistribution
        = statistics.weekdayDistribution();

    EXPECT_EQ(expectedDistribution,
              weekdayDistribution.getDistributionVector());
    EXPECT_EQ(expected_max_value_bin, weekdayDistribution.getMaxValueBin());
    EXPECT_DOUBLE_EQ(expected_average, weekdayDistribution.getAverage());
    EXPECT_DOUBLE_EQ(expected_max, weekdayDistribution.getMax());
}

TEST(SprintStatItem, computes_worktime_distribution_correctly)
{
    TimeSpan timeSpan{DateTime::fromYMD(2015, 6, 2),
                      DateTime::fromYMD(2015, 6, 2).add(DateTime::Days(1))};
    std::vector<Sprint> sprints;
    SprintBuilder builder = SprintBuilder{}.withTaskUuid("");
    for (int i = 0; i < 30; ++i) {
        TimeSpan sprintTimespan{
            timeSpan.start().add(std::chrono::hours(i)),
            timeSpan.start().add(std::chrono::minutes(i * 60 + 25))};
        sprints.push_back(builder.withTimeSpan(sprintTimespan).build());
    }
    const std::vector<double> expectedDistribution{7, 7, 4, 4, 4, 4};
    const double expectedAverage{5.0};
    const double expectedMax{7};
    const size_t expectedMaxValueBin{0};

    const SprintStatItem statistics{sprints, timeSpan};
    const auto distribution = statistics.worktimeDistribution();

    EXPECT_EQ(expectedDistribution, distribution.getDistributionVector());
    EXPECT_DOUBLE_EQ(expectedAverage, distribution.getAverage());
    EXPECT_DOUBLE_EQ(expectedMax, distribution.getMax());
    EXPECT_EQ(expectedMaxValueBin, distribution.getMaxValueBin());
}

TEST(SprintStatItem, ignores_sprints_outside_time_range)
{
    const TimeSpan timeSpan{DateTime::fromYMD(2018, 6, 26),
                            DateTime::fromYMD(2018, 6, 28)};
    auto sprintBuilder = SprintBuilder{}.withTaskUuid("");
    const auto startTime = timeSpan.start().add(DateTime::Days(-1));
    std::vector<Sprint> sprints;
    for (int i = 0; i < 5; ++i) {
        sprints.push_back(sprintBuilder
                              .withTimeSpan(TimeSpan{
                                  startTime.add(DateTime::Days(i)),
                                  startTime.add(DateTime::Days(i)).add(25min)})
                              .build());
    }
    const std::vector<double> expectedDailyDistribution{1, 1, 1};
    const std::vector<double> expectedWeekdayDistribution{0, 1, 1, 1, 0, 0, 0};

    const SprintStatItem statistics{sprints, timeSpan};
    const auto dailyDistribution = statistics.dailyDistribution();
    const auto weekdayDistribution = statistics.weekdayDistribution();

    EXPECT_EQ(expectedDailyDistribution,
              dailyDistribution.getDistributionVector());
    EXPECT_EQ(expectedWeekdayDistribution,
              weekdayDistribution.getDistributionVector());
}

class TagTopFixture : public ::testing::Test {
public:
    void push_n(std::vector<Sprint>& sprints, const Sprint& sprint, size_t n)
    {
        for (size_t i = 0; i < n; ++i)
            sprints.push_back(sprint);
    }
    const TimeSpan defaultTimespan{DateTime::currentDateTime().add(-25min),
                                   DateTime::currentDateTime()};
};

TEST_F(TagTopFixture,
       getting_sprints_or_tag_name_throws_exception_when_position_is_invalid)
{
    std::vector<Sprint> sprints;
    SprintBuilder sprintBuilder;
    sprintBuilder.withTaskUuid("1234").withTimeSpan(defaultTimespan);
    push_n(sprints, sprintBuilder.withExplicitTags({Tag{"Tag1"}}).build(), 4);
    push_n(sprints, sprintBuilder.withExplicitTags({Tag{"Tag2"}}).build(), 49);

    TagTop tagTop{sprints, 3};

    ASSERT_THROW((tagTop.sprintsForTagAt(tagTop.topSize() + 1)),
                 std::out_of_range);
    ASSERT_THROW((tagTop.tagNameAt(tagTop.topSize() + 1)), std::out_of_range);
}

TEST_F(TagTopFixture, does_not_reduce_frequency_vector_when_all_tags_fit)
{
    std::vector<Sprint> sprints;
    SprintBuilder sprintBuilder;
    sprintBuilder.withTaskUuid("1234").withTimeSpan(defaultTimespan);
    push_n(sprints, sprintBuilder.withExplicitTags({Tag{"Tag1"}}).build(), 4);
    push_n(sprints, sprintBuilder.withExplicitTags({Tag{"Tag2"}}).build(), 49);
    std::vector<TagTop::TagFrequency> expected{{Tag{"Tag2"}, double(49) / 53},
                                               {Tag{"Tag1"}, double(4) / 53}};

    TagTop tagTop{sprints, 3};

    EXPECT_TRUE(expected == tagTop.tagFrequencies());
    EXPECT_EQ(49, tagTop.sprintsForTagAt(0).size());
    EXPECT_EQ(4, tagTop.sprintsForTagAt(1).size());
}

TEST_F(TagTopFixture,
       does_not_reduce_slice_vector_when_has_less_tags_than_allowed)
{
    std::vector<Sprint> sprints;
    SprintBuilder sprintBuilder;
    sprintBuilder.withTaskUuid("1234").withTimeSpan(defaultTimespan);
    push_n(sprints, sprintBuilder.withExplicitTags({Tag{"Tag1"}}).build(), 4);
    push_n(sprints, sprintBuilder.withExplicitTags({Tag{"Tag2"}}).build(), 49);
    std::vector<TagTop::TagFrequency> expected;
    expected.push_back(std::make_pair(Tag{"Tag2"}, double(49) / 53));
    expected.push_back(std::make_pair(Tag{"Tag1"}, double(4) / 53));

    TagTop map{sprints, 5};

    EXPECT_TRUE("Tag2" == map.tagNameAt(0));
    EXPECT_TRUE("Tag1" == map.tagNameAt(1));
    EXPECT_TRUE(expected == map.tagFrequencies());
    EXPECT_EQ(49, map.sprintsForTagAt(0).size());
    EXPECT_EQ(4, map.sprintsForTagAt(1).size());
}

TEST_F(TagTopFixture, distributes_sprints_to_tags_ignoring_non_tagged)
{
    std::vector<Sprint> sprints;
    SprintBuilder builder;
    builder.withTaskUuid("1234").withTimeSpan(defaultTimespan);
    push_n(sprints, builder.withExplicitTags({Tag{"Tag1"}}).build(), 4);
    push_n(sprints, builder.withExplicitTags({Tag{"Tag2"}}).build(), 49);
    push_n(sprints,
           builder.withExplicitTags({Tag{"Tag2"}, Tag{"Tag1"}}).build(),
           1);
    push_n(sprints,
           builder.withExplicitTags({Tag{"C++"}, Tag{"Tag4"}}).build(),
           10);
    push_n(sprints, builder.withExplicitTags({Tag{"Tag4"}}).build(), 25);
    push_n(sprints, builder.withExplicitTags({Tag{"Tag5"}}).build(), 4);
    push_n(sprints, builder.withExplicitTags({}).build(), 100);
    std::vector<TagTop::TagFrequency> expected{
        std::make_pair(Tag{"Tag2"}, double(50) / 104),
        std::make_pair(Tag{"Tag4"}, double(35) / 104),
        std::make_pair(Tag{"C++"}, double(10) / 104),
        std::make_pair(Tag{"Tag1"}, double(5) / 104),
        std::make_pair(Tag{""}, double(4) / 104)};

    TagTop map{sprints, 5};

    EXPECT_TRUE("Tag2" == map.tagNameAt(0));
    EXPECT_TRUE("Tag4" == map.tagNameAt(1));
    EXPECT_TRUE("C++" == map.tagNameAt(2));
    EXPECT_TRUE("Tag1" == map.tagNameAt(3));
    EXPECT_TRUE("" == map.tagNameAt(4));
    EXPECT_TRUE(expected == map.tagFrequencies());
    EXPECT_EQ(50, map.sprintsForTagAt(0).size());
    EXPECT_EQ(35, map.sprintsForTagAt(1).size());
    EXPECT_EQ(10, map.sprintsForTagAt(2).size());
    EXPECT_EQ(5, map.sprintsForTagAt(3).size());
    EXPECT_EQ(4, map.sprintsForTagAt(4).size());
}

TEST_F(TagTopFixture, reduces_slice_vector_tail_when_has_more_tags_than_allowed)
{
    std::vector<Sprint> sprints;
    SprintBuilder builder;
    builder.withTaskUuid("1234");
    builder.withTimeSpan(defaultTimespan);
    push_n(sprints, builder.withExplicitTags({Tag{"Tag1"}}).build(), 4);
    push_n(sprints, builder.withExplicitTags({Tag{"Tag2"}}).build(), 49);
    push_n(sprints,
           builder.withExplicitTags({Tag{"Tag2"}, Tag{"Tag1"}}).build(),
           1);
    push_n(sprints,
           builder.withExplicitTags({Tag{"Tag3"}, Tag{"Tag4"}}).build(),
           10);
    push_n(sprints, builder.withExplicitTags({Tag{"Tag4"}}).build(), 25);
    push_n(sprints, builder.withExplicitTags({}).build(), 100);
    std::vector<TagTop::TagFrequency> expected{
        std::make_pair(Tag{"Tag2"}, double(50) / 100),
        std::make_pair(Tag{"Tag4"}, double(35) / 100),
        std::make_pair(Tag{"Tag3"}, double(10) / 100),
        std::make_pair(Tag{""}, double(5) / 100)};

    TagTop map{sprints, 4};

    EXPECT_TRUE("Tag2" == map.tagNameAt(0));
    EXPECT_TRUE("Tag4" == map.tagNameAt(1));
    EXPECT_TRUE("Tag3" == map.tagNameAt(2));
    EXPECT_TRUE("" == map.tagNameAt(3));
    EXPECT_TRUE(expected == map.tagFrequencies());
    EXPECT_EQ(50, map.sprintsForTagAt(0).size());
    EXPECT_EQ(35, map.sprintsForTagAt(1).size());
    EXPECT_EQ(10, map.sprintsForTagAt(2).size());
    EXPECT_EQ(5, map.sprintsForTagAt(3).size());
}
