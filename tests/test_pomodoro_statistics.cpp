#include "core/PomodoroStatistics.h"
#include <TestHarness.h>

TEST_GROUP(PomoStatItem)
{
    const double threshold = 0.00001;
    const long long irrelevantTodoId = 42;
};

TEST(PomoStatItem, test_empty_daily_statistics)
{
    std::vector<Pomodoro> pomodoros;
    PomodoroStatItem statistics{pomodoros, TimeSpan{}};
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

TEST(PomoStatItem, test_empty_weekday_statistics)
{
    std::vector<Pomodoro> pomodoros;
    PomodoroStatItem statistics{pomodoros, TimeSpan{}};
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

TEST(PomoStatItem, test_computes_daily_distribution_correctly)
{
    double expected_average = 24.5;
    double expected_max = 48;
    size_t expected_max_value_bin = 47;
    double expected_total = 1176;
    std::vector<Pomodoro> pomodoros;
    DateTime start = DateTime::currentDateTime();
    DateTime end = start.addDays(47);
    TimeSpan timeSpan{start, end};
    std::vector<int> expectedDistributionVector(48, 0);
    for (size_t i = 0; i < 48; ++i) {
        for (size_t j = 0; j < i + 1; ++j) {
            DateTime pomoDateTime = start.addDays(static_cast<int>(i));
            TimeSpan pomoInterval{pomoDateTime, pomoDateTime};
            pomodoros.push_back(Pomodoro{"Irrelevant", pomoInterval, {}});
            expectedDistributionVector[i]++;
        }
    }

    PomodoroStatItem statistics{pomodoros, timeSpan};
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

TEST(PomoStatItem, test_computes_weekday_distribution_correctly)
{
    double expected_average = 7.5;
    double expected_max = 10.5;
    size_t expected_max_value_bin = 6;
    std::vector<Pomodoro> increasingPomodoros;
    // (2015, 6, 1) is Monday, so each weekday occures exactly twice
    // in 14-day timeSpan
    TimeSpan timeSpan{DateTime::fromYMD(2015, 6, 1),
                      DateTime::fromYMD(2015, 6, 14)};

    for (int i = 1; i < 15; ++i) {
        for (int j = 0; j < i; ++j) {
            DateTime pomoDateTime = DateTime::fromYMD(2015, 6, i);
            TimeSpan pomoInterval{pomoDateTime, pomoDateTime};
            increasingPomodoros.push_back(
                Pomodoro{"Whatever", pomoInterval, {}});
        }
    }

    std::vector<double> expected_distribution{
        4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5};
    PomodoroStatItem statistics{increasingPomodoros, timeSpan};

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

TEST_GROUP(TagDistribution){

    void pushToPomodoros(std::vector<Pomodoro> & pomodoros,
                         std::string name,
                         const std::list<std::string>& tags,
                         int n){
        for (int i = 0; i < n; ++i){pomodoros.push_back(Pomodoro{
            name,
            TimeSpan{DateTime::currentDateTime(), DateTime::currentDateTime()},
            tags});
}
}
}
;

TEST(TagDistribution, test_does_not_reduce_slice_vector_when_all_tags_fit)
{
    std::vector<Pomodoro> pomodoros;
    pushToPomodoros(pomodoros, "irrelevant", {"Tag1"}, 4);
    pushToPomodoros(pomodoros, "irrelevant", {"Tag2"}, 49);
    TagDistribution map{pomodoros, 3};
    std::vector<TagCount> expected;
    expected.push_back(std::make_pair("Tag2", double(49) / 53));
    expected.push_back(std::make_pair("Tag1", double(4) / 53));

    CHECK(expected == map.topTagsDistribution())

    CHECK_EQUAL(49, map.pomodorosForNthTopTag(0).size())
    CHECK_EQUAL(4, map.pomodorosForNthTopTag(1).size())
}

TEST(TagDistribution,
     test_does_not_reduce_slice_vector_when_has_less_tags_than_allowed)
{
    std::vector<Pomodoro> pomodoros;
    pushToPomodoros(pomodoros, "irrelevant", {"Tag1"}, 4);
    pushToPomodoros(pomodoros, "irrelevant", {"Tag2"}, 49);
    TagDistribution map{pomodoros, 5};
    std::vector<TagCount> expected;
    expected.push_back(std::make_pair("Tag2", double(49) / 53));
    expected.push_back(std::make_pair("Tag1", double(4) / 53));
    CHECK("Tag2" == map.getNthTopTagName(0))
    CHECK("Tag1" == map.getNthTopTagName(1))

    CHECK(expected == map.topTagsDistribution())

    CHECK_EQUAL(49, map.pomodorosForNthTopTag(0).size())
    CHECK_EQUAL(4, map.pomodorosForNthTopTag(1).size())
}

TEST(TagDistribution, test_distributes_pomodoros_to_tags_ignoring_non_tagged)
{
    std::vector<Pomodoro> pomodoros;
    pushToPomodoros(pomodoros, "irrelevant", {"Tag1"}, 4);
    pushToPomodoros(pomodoros, "irrelevant", {"Tag2"}, 49);
    pushToPomodoros(pomodoros, "irrelevant", {"Tag2", "Tag1"}, 1);
    pushToPomodoros(pomodoros, "irrelevant", {"C++", "Tag4"}, 10);
    pushToPomodoros(pomodoros, "irrelevant", {"Tag4"}, 25);
    pushToPomodoros(pomodoros, "irrelevant", {"Tag5"}, 4);
    pushToPomodoros(pomodoros, "irrelevant", {}, 100);
    TagDistribution map{pomodoros, 5};
    std::vector<TagCount> expected;
    expected.push_back(std::make_pair("Tag2", double(50) / 104));
    expected.push_back(std::make_pair("Tag4", double(35) / 104));
    expected.push_back(std::make_pair("C++", double(10) / 104));
    expected.push_back(std::make_pair("Tag1", double(5) / 104));
    expected.push_back(std::make_pair("", double(4) / 104));

    CHECK("Tag2" == map.getNthTopTagName(0))
    CHECK("Tag4" == map.getNthTopTagName(1))
    CHECK("C++" == map.getNthTopTagName(2))
    CHECK("Tag1" == map.getNthTopTagName(3))
    CHECK("" == map.getNthTopTagName(4))

    CHECK(expected == map.topTagsDistribution())

    CHECK_EQUAL(50, map.pomodorosForNthTopTag(0).size())
    CHECK_EQUAL(35, map.pomodorosForNthTopTag(1).size())
    CHECK_EQUAL(10, map.pomodorosForNthTopTag(2).size())
    CHECK_EQUAL(5, map.pomodorosForNthTopTag(3).size())
    CHECK_EQUAL(4, map.pomodorosForNthTopTag(4).size())
}

TEST(TagDistribution,
     test_reduces_slice_vector_tail_when_has_more_tags_than_allowed)
{
    std::vector<Pomodoro> pomodoros;
    pushToPomodoros(pomodoros, "irrelevant", {"Tag1"}, 4);
    pushToPomodoros(pomodoros, "irrelevant", {"Tag2"}, 49);
    pushToPomodoros(pomodoros, "irrelevant", {"Tag2", "Tag1"}, 1);
    pushToPomodoros(pomodoros, "irrelevant", {"Tag3", "Tag4"}, 10);
    pushToPomodoros(pomodoros, "irrelevant", {"Tag4"}, 25);
    pushToPomodoros(pomodoros, "irrelevant", {}, 100);
    TagDistribution map{pomodoros, 4};
    std::vector<TagCount> expected{std::make_pair("Tag2", double(50) / 100),
                                   std::make_pair("Tag4", double(35) / 100),
                                   std::make_pair("Tag3", double(10) / 100),
                                   std::make_pair("", double(5) / 100)};

    CHECK("Tag2" == map.getNthTopTagName(0))
    CHECK("Tag4" == map.getNthTopTagName(1))
    CHECK("Tag3" == map.getNthTopTagName(2))
    CHECK("" == map.getNthTopTagName(3))

    CHECK(expected == map.topTagsDistribution())

    CHECK_EQUAL(50, map.pomodorosForNthTopTag(0).size())
    CHECK_EQUAL(35, map.pomodorosForNthTopTag(1).size())
    CHECK_EQUAL(10, map.pomodorosForNthTopTag(2).size())
    CHECK_EQUAL(5, map.pomodorosForNthTopTag(3).size())
}
