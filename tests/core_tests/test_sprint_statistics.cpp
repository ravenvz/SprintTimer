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

#include "core/SprintBuilder.h"
#include "core/SprintStatistics.h"
#include "core/TagTop.h"
#include "gtest/gtest.h"

using namespace sprint_timer;
using namespace sprint_timer::entities;
using namespace std::chrono_literals;
using dw::DateTime;
using dw::DateTimeRange;

namespace {

const DateTimeRange defaultTimespan{dw::current_date_time() - 25min,
                                    dw::current_date_time()};

const dw::DateRange someDateRange{dw::current_date(),
                                  dw::current_date() + dw::Days{1}};

} // namespace

TEST(WeekdayStatistics, creates_reasonable_defaults_for_empty_distribution)
{
    const std::vector<double> expected = std::vector<double>(7, 0.0);

    const auto actual = weekdayStatistics(std::vector<Sprint>{}, someDateRange);

    EXPECT_EQ(expected, actual.getDistributionVector());
    EXPECT_DOUBLE_EQ(0, actual.getAverage());
    EXPECT_DOUBLE_EQ(0, actual.getMax());
    EXPECT_DOUBLE_EQ(0, actual.getTotal());
}


TEST(WeekdayStatistics, returns_distribution)
{
    using namespace dw;
    std::vector<Sprint> increasingSprints;
    SprintBuilder sprintBuilder = SprintBuilder{}.withTaskUuid("irrelevant");
    // (2015, 6, 1) is Monday, so each weekday occures exactly twice
    // in 14-day timeSpan
    const DateRange dateRange{Date{Year{2015}, Month{6}, Day{1}},
                              Date{Year{2015}, Month{6}, Day{14}}};
    for (unsigned i = 1; i < 15; ++i) {
        for (unsigned j = 0; j < i; ++j) {
            const DateTime sprintDateTime
                = DateTime{Date{Year{2015}, Month{6}, Day{i}}};
            increasingSprints.push_back(
                sprintBuilder
                    .withTimeSpan(DateTimeRange{sprintDateTime, sprintDateTime})
                    .build());
        }
    }
    const double expected_average = 7.5;
    const double expected_max = 10.5;
    const size_t expected_max_value_bin = 6;
    const std::vector<double> expectedDistribution{
        4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5};

    const auto distribution = weekdayStatistics(increasingSprints, dateRange);

    EXPECT_EQ(expectedDistribution, distribution.getDistributionVector());
    EXPECT_EQ(expected_max_value_bin, distribution.getMaxValueBin());
    EXPECT_DOUBLE_EQ(expected_average, distribution.getAverage());
    EXPECT_DOUBLE_EQ(expected_max, distribution.getMax());
}


TEST(WorkingHoursStatistics, creates_reasonable_defaults_for_empty_distribution)
{
    const std::vector<double> expected
        = std::vector<double>(DayPart::numParts, 0);

    const auto actual = workingHoursStatistics(std::vector<Sprint>{});

    EXPECT_EQ(expected, actual.getDistributionVector());
    EXPECT_DOUBLE_EQ(0, actual.getAverage());
    EXPECT_DOUBLE_EQ(0, actual.getMax());
    EXPECT_DOUBLE_EQ(0, actual.getTotal());
}


TEST(WorkingHoursStatistics, returns_distribution)
{
    using namespace dw;
    const DateTime start{DateTime{Date{Year{2015}, Month{6}, Day{2}}}};
    std::vector<Sprint> sprints;
    SprintBuilder builder = SprintBuilder{}.withTaskUuid("");
    for (int i = 0; i < 30; ++i) {
        DateTimeRange sprintTimespan{start + std::chrono::hours{i},
                                     start + std::chrono::minutes(i * 60 + 25)};
        sprints.push_back(builder.withTimeSpan(sprintTimespan).build());
    }
    const std::vector<double> expectedDistribution{7, 7, 4, 4, 4, 4};
    const double expectedAverage{5.0};
    const double expectedMax{7};
    const size_t expectedMaxValueBin{0};

    const auto distribution = workingHoursStatistics(sprints);

    EXPECT_EQ(expectedDistribution, distribution.getDistributionVector());
    EXPECT_DOUBLE_EQ(expectedAverage, distribution.getAverage());
    EXPECT_DOUBLE_EQ(expectedMax, distribution.getMax());
    EXPECT_EQ(expectedMaxValueBin, distribution.getMaxValueBin());
}


TEST(DailyStatistics, creates_reasonable_defaults_for_empty_distribution)
{
    using namespace dw;
    const std::vector<double> expected{0};

    const auto actual = dailyStatistics(
        std::vector<Sprint>{}, DateRange{current_date(), current_date()});

    EXPECT_EQ(expected, actual.getDistributionVector());
    EXPECT_DOUBLE_EQ(0, actual.getAverage());
    EXPECT_DOUBLE_EQ(0, actual.getMax());
    EXPECT_DOUBLE_EQ(0, actual.getTotal());
}

TEST(DailyStatistics, returns_distribution)
{
    using namespace dw;
    const Date start = current_date();
    const Date end = start + Days{47};
    const DateRange dateRange{start, end};
    std::vector<Sprint> sprints;
    std::vector<double> expected(48, 0);
    const DateTime sprintStart = current_date_time();
    SprintBuilder sprintBuilder;
    sprintBuilder.withTaskUuid("");
    for (size_t i = 0; i < 48; ++i) {
        for (size_t j = 0; j < i + 1; ++j) {
            const DateTime sprintDateTime = sprintStart + dw::Days{i};
            const DateTimeRange sprintTimeSpan{sprintDateTime, sprintDateTime};
            sprints.push_back(
                sprintBuilder.withTimeSpan(sprintTimeSpan).build());
            expected[i]++;
        }
    }
    const double expected_average{24.5};
    const double expected_max{48};
    const size_t expected_max_value_bin{47};
    const double expected_total{1176};

    const auto distribution = dailyStatistics(sprints, dateRange);

    EXPECT_EQ(expected_max_value_bin, distribution.getMaxValueBin());
    EXPECT_DOUBLE_EQ(expected_average, distribution.getAverage());
    EXPECT_DOUBLE_EQ(expected_max, distribution.getMax());
    EXPECT_DOUBLE_EQ(expected_total, distribution.getTotal());
    EXPECT_EQ(expected, distribution.getDistributionVector());
}


TEST(DailyStatistics, ignores_sprints_outside_time_range)
{
    using namespace dw;
    const DateRange dateRange{Date{Year{2018}, Month{6}, Day{26}},
                              Date{Year{2018}, Month{6}, Day{28}}};
    const DateTime start{DateTime{dateRange.start()}};
    auto sprintBuilder = SprintBuilder{}.withTaskUuid("");
    const auto startTime = start - Days{1};
    std::vector<Sprint> sprints;
    for (int i = 0; i < 5; ++i) {
        sprints.push_back(
            sprintBuilder
                .withTimeSpan(DateTimeRange{startTime + dw::Days{i},
                                            startTime + dw::Days{i} + 25min})
                .build());
    }
    const std::vector<double> expectedDailyDistribution{1, 1, 1};

    const auto distribution = dailyStatistics(sprints, dateRange);

    EXPECT_EQ(expectedDailyDistribution, distribution.getDistributionVector());
}


TEST(WeekdayStatistics, ignores_sprints_outside_time_range)
{
    using namespace dw;
    const DateRange dateRange{Date{Year{2018}, Month{6}, Day{26}},
                              Date{Year{2018}, Month{6}, Day{28}}};
    const DateTime start{DateTime{dateRange.start()}};
    auto sprintBuilder = SprintBuilder{}.withTaskUuid("");
    const auto startTime = start - Days{1};
    std::vector<Sprint> sprints;
    for (int i = 0; i < 5; ++i) {
        sprints.push_back(
            sprintBuilder
                .withTimeSpan(DateTimeRange{startTime + dw::Days{i},
                                            startTime + dw::Days{i} + 25min})
                .build());
    }
    const std::vector<double> expectedWeekdayDistribution{0, 1, 1, 1, 0, 0, 0};

    const auto distribution = weekdayStatistics(sprints, dateRange);

    EXPECT_EQ(expectedWeekdayDistribution,
              distribution.getDistributionVector());
}


class TagTopFixture : public ::testing::Test {
public:
    void push_n(std::vector<Sprint>& sprints, const Sprint& sprint, size_t n)
    {
        for (size_t i = 0; i < n; ++i)
            sprints.push_back(sprint);
    }
    const DateTimeRange defaultTimespan{dw::current_date_time() - 25min,
                                        dw::current_date_time()};
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
