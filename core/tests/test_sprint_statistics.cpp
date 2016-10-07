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
#include "core/SprintStatistics.h"
#include "core/TagTop.h"
#include <TestHarness.h>

TEST_GROUP(SprintStatItem)
{
    const double threshold = 0.00001;
    const long long irrelevantTodoId = 42;
};

TEST(SprintStatItem, test_empty_daily_statistics)
{
    std::vector<Sprint> sprints;
    SprintStatItem statistics{sprints,
                                TimeSpan{std::chrono::system_clock::now(),
                                         std::chrono::system_clock::now()}};
    double expected_average = 0;
    double expected_max = 0;
    double expected_total = 0;
    std::vector<double> expected_distribution;

    Distribution<double> dailyDistribution = statistics.dailyDistribution();
    std::vector<double> distributionVector
        = dailyDistribution.getDistributionVector();

    CHECK_EQUAL(0, dailyDistribution.getNumBins())
    DOUBLES_EQUAL(expected_average, dailyDistribution.getAverage(), threshold)
    DOUBLES_EQUAL(expected_max, dailyDistribution.getMax(), threshold)
    DOUBLES_EQUAL(expected_total, dailyDistribution.getTotal(), threshold)
    CHECK_EQUAL(expected_distribution.size(), distributionVector.size())
    for (size_t i = 0; i < expected_distribution.size(); ++i) {
        DOUBLES_EQUAL(
            expected_distribution[i], distributionVector[i], threshold)
    }
}

TEST(SprintStatItem, test_empty_weekday_statistics)
{
    std::vector<Sprint> sprints;
    SprintStatItem statistics{sprints,
                                TimeSpan{std::chrono::system_clock::now(),
                                         std::chrono::system_clock::now()}};
    double expected_average = 0;
    double expected_max = 0;
    std::vector<double> expected_distribution = std::vector<double>(7, 0);

    Distribution<double> weekdayDistribution = statistics.weekdayDistribution();
    std::vector<double> distributionVector
        = weekdayDistribution.getDistributionVector();

    DOUBLES_EQUAL(expected_average, weekdayDistribution.getAverage(), threshold)
    DOUBLES_EQUAL(expected_max, weekdayDistribution.getMax(), threshold)
    CHECK_EQUAL(expected_distribution.size(), distributionVector.size())
    for (size_t i = 0; i < expected_distribution.size(); ++i) {
        DOUBLES_EQUAL(
            expected_distribution[i], distributionVector[i], threshold)
    }
}

TEST(SprintStatItem, test_computes_daily_distribution_correctly)
{
    double expected_average = 24.5;
    double expected_max = 48;
    size_t expected_max_value_bin = 47;
    double expected_total = 1176;
    std::vector<Sprint> sprints;
    DateTime start = DateTime::currentDateTime();
    DateTime end = start.addDays(47);
    TimeSpan timeSpan{start, end};
    std::vector<int> expectedDistributionVector(48, 0);
    for (size_t i = 0; i < 48; ++i) {
        for (size_t j = 0; j < i + 1; ++j) {
            DateTime sprintDateTime = start.addDays(static_cast<int>(i));
            TimeSpan sprintInterval{sprintDateTime, sprintDateTime};
            sprints.push_back(Sprint{"Irrelevant", sprintInterval, {}, "", ""});
            expectedDistributionVector[i]++;
        }
    }

    SprintStatItem statistics{sprints, timeSpan};
    Distribution<double> distribution = statistics.dailyDistribution();
    std::vector<double> distributionVector
        = distribution.getDistributionVector();

    CHECK_EQUAL(expectedDistributionVector.size(), distribution.getNumBins())
    CHECK_EQUAL(expected_max_value_bin, distribution.getMaxValueBin())
    DOUBLES_EQUAL(expected_average, distribution.getAverage(), threshold)
    DOUBLES_EQUAL(expected_max, distribution.getMax(), threshold)
    DOUBLES_EQUAL(expected_total, distribution.getTotal(), threshold)

    for (size_t i = 0; i < expectedDistributionVector.size(); ++i) {
        DOUBLES_EQUAL(
            expectedDistributionVector[i], distributionVector[i], threshold)
    }
}

TEST(SprintStatItem, test_computes_weekday_distribution_correctly)
{
    double expected_average = 7.5;
    double expected_max = 10.5;
    size_t expected_max_value_bin = 6;
    std::vector<Sprint> increasingSprints;
    // (2015, 6, 1) is Monday, so each weekday occures exactly twice
    // in 14-day timeSpan
    TimeSpan timeSpan{DateTime::fromYMD(2015, 6, 1),
                      DateTime::fromYMD(2015, 6, 14)};

    for (int i = 1; i < 15; ++i) {
        for (int j = 0; j < i; ++j) {
            DateTime sprintDateTime = DateTime::fromYMD(2015, 6, i);
            TimeSpan sprintInterval{sprintDateTime, sprintDateTime};
            increasingSprints.push_back(
                Sprint{"Whatever", sprintInterval, {}, "uuid", "taskUuid"});
        }
    }

    std::vector<double> expected_distribution{
        4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5};
    SprintStatItem statistics{increasingSprints, timeSpan};

    Distribution<double> weekdayDistribution = statistics.weekdayDistribution();
    std::vector<double> distributionVector
        = weekdayDistribution.getDistributionVector();

    CHECK_EQUAL(expected_max_value_bin, weekdayDistribution.getMaxValueBin())
    DOUBLES_EQUAL(expected_average, weekdayDistribution.getAverage(), threshold)
    DOUBLES_EQUAL(expected_max, weekdayDistribution.getMax(), threshold)
    for (size_t i = 0; i < expected_distribution.size(); ++i) {
        DOUBLES_EQUAL(
            expected_distribution[i], distributionVector[i], threshold)
    }
}

TEST_GROUP(TagTop)
{

    const std::string irrelevantName{"irrelevant"};

    void pushToSprints(std::vector<Sprint>& sprints,
                       std::string name,
                       const std::list<Tag>& tags,
                       int n) {
        for (int i = 0; i < n; ++i) {
            sprints.push_back(Sprint{
                    name,
                    TimeSpan{DateTime::currentDateTime(), DateTime::currentDateTime()},
                    tags,
                    "uuid",
                    "taskUuid"});
        }
    }
};

TEST(TagTop, getting_sprints_or_tag_name_throws_exception_when_position_is_invalid) {
    std::vector<Sprint> sprints;
    pushToSprints(sprints, irrelevantName, {Tag{"Tag1"}}, 4);
    pushToSprints(sprints, irrelevantName, {Tag{"Tag2"}}, 49);

    TagTop tagTop{sprints, 3};

    CHECK_THROWS(std::out_of_range, tagTop.sprintsForTagAt(tagTop.topSize() + 1));
    CHECK_THROWS(std::out_of_range, tagTop.tagNameAt(tagTop.topSize() + 1));
}

TEST(TagTop, does_not_reduce_frequency_vector_when_all_tags_fit)
{
    std::vector<Sprint> sprints;
    pushToSprints(sprints, irrelevantName, {Tag{"Tag1"}}, 4);
    pushToSprints(sprints, irrelevantName, {Tag{"Tag2"}}, 49);
    std::vector<TagTop::TagFrequency> expected{
            {Tag{"Tag2"}, double(49) / 53},
            {Tag{"Tag1"}, double(4) / 53}
    };

    TagTop tagTop{sprints, 3};

    CHECK(expected == tagTop.tagFrequencies())
    CHECK_EQUAL(49, tagTop.sprintsForTagAt(0).size())
    CHECK_EQUAL(4, tagTop.sprintsForTagAt(1).size())
//    CHECK_EQUAL(0, tagTop.sprintsForTagAt(2).size())
}

TEST(TagTop,
     does_not_reduce_slice_vector_when_has_less_tags_than_allowed)
{
    std::vector<Sprint> sprints;
    pushToSprints(sprints, irrelevantName, {Tag{"Tag1"}}, 4);
    pushToSprints(sprints, irrelevantName, {Tag{"Tag2"}}, 49);
    std::vector<TagTop::TagFrequency> expected;
    expected.push_back(std::make_pair(Tag{"Tag2"}, double(49) / 53));
    expected.push_back(std::make_pair(Tag{"Tag1"}, double(4) / 53));

    TagTop map{sprints, 5};

    CHECK("Tag2" == map.tagNameAt(0))
    CHECK("Tag1" == map.tagNameAt(1))
    CHECK(expected == map.tagFrequencies())
    CHECK_EQUAL(49, map.sprintsForTagAt(0).size())
    CHECK_EQUAL(4, map.sprintsForTagAt(1).size())
}

TEST(TagTop, distributes_sprints_to_tags_ignoring_non_tagged)
{
    std::vector<Sprint> sprints;
    pushToSprints(sprints, irrelevantName, {Tag{"Tag1"}}, 4);
    pushToSprints(sprints, irrelevantName, {Tag{"Tag2"}}, 49);
    pushToSprints(sprints, irrelevantName, {Tag{"Tag2"}, Tag{"Tag1"}}, 1);
    pushToSprints(sprints, irrelevantName, {Tag{"C++"}, Tag{"Tag4"}}, 10);
    pushToSprints(sprints, irrelevantName, {Tag{"Tag4"}}, 25);
    pushToSprints(sprints, irrelevantName, {Tag{"Tag5"}}, 4);
    pushToSprints(sprints, irrelevantName, {}, 100);
    std::vector<TagTop::TagFrequency> expected{
            std::make_pair(Tag{"Tag2"}, double(50) / 104),
            std::make_pair(Tag{"Tag4"}, double(35) / 104),
            std::make_pair(Tag{"C++"}, double(10) / 104),
            std::make_pair(Tag{"Tag1"}, double(5) / 104),
            std::make_pair(Tag{""}, double(4) / 104)
    };

    TagTop map{sprints, 5};

    CHECK("Tag2" == map.tagNameAt(0))
    CHECK("Tag4" == map.tagNameAt(1))
    CHECK("C++" == map.tagNameAt(2))
    CHECK("Tag1" == map.tagNameAt(3))
    CHECK("" == map.tagNameAt(4))
    CHECK(expected == map.tagFrequencies())
    CHECK_EQUAL(50, map.sprintsForTagAt(0).size())
    CHECK_EQUAL(35, map.sprintsForTagAt(1).size())
    CHECK_EQUAL(10, map.sprintsForTagAt(2).size())
    CHECK_EQUAL(5, map.sprintsForTagAt(3).size())
    CHECK_EQUAL(4, map.sprintsForTagAt(4).size())
}

TEST(TagTop,
     reduces_slice_vector_tail_when_has_more_tags_than_allowed)
{
    std::vector<Sprint> sprints;
    pushToSprints(sprints, irrelevantName, {Tag{"Tag1"}}, 4);
    pushToSprints(sprints, irrelevantName, {Tag{"Tag2"}}, 49);
    pushToSprints(sprints, irrelevantName, {Tag{"Tag2"}, Tag{"Tag1"}}, 1);
    pushToSprints(sprints, irrelevantName, {Tag{"Tag3"}, Tag{"Tag4"}}, 10);
    pushToSprints(sprints, irrelevantName, {Tag{"Tag4"}}, 25);
    pushToSprints(sprints, irrelevantName, {}, 100);
    std::vector<TagTop::TagFrequency> expected{
        std::make_pair(Tag{"Tag2"}, double(50) / 100),
        std::make_pair(Tag{"Tag4"}, double(35) / 100),
        std::make_pair(Tag{"Tag3"}, double(10) / 100),
        std::make_pair(Tag{""}, double(5) / 100)};

    TagTop map{sprints, 4};

    CHECK("Tag2" == map.tagNameAt(0))
    CHECK("Tag4" == map.tagNameAt(1))
    CHECK("Tag3" == map.tagNameAt(2))
    CHECK("" == map.tagNameAt(3))
    CHECK(expected == map.tagFrequencies())
    CHECK_EQUAL(50, map.sprintsForTagAt(0).size())
    CHECK_EQUAL(35, map.sprintsForTagAt(1).size())
    CHECK_EQUAL(10, map.sprintsForTagAt(2).size())
    CHECK_EQUAL(5, map.sprintsForTagAt(3).size())
}
