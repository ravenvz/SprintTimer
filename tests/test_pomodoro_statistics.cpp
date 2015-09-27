#include "core/PomodoroStatistics.h"
#include <TestHarness.h>


TEST_GROUP(WeekdayDistribution) {
    const double threshold = 0.00001;
};

TEST(WeekdayDistribution, test_empty) {
    QVector<Pomodoro> pomodoros;
    double expected_average = 0;
    double expected_max = 0;
    QVector<double> expected_distribution = QVector<double> (7, 0);

    PomoWeekdayDistribution distribution {pomodoros};
    QVector<double> distributionVector = distribution.getDistributionVector();

    DOUBLES_EQUAL(expected_average, distribution.getAverage(), threshold)
    DOUBLES_EQUAL(expected_max, distribution.getMax(), threshold)
    CHECK_EQUAL(expected_distribution.size(), distributionVector.size())
    for (int i = 0; i < expected_distribution.size(); ++i) {
        DOUBLES_EQUAL(expected_distribution[i], distributionVector[i], threshold)
    }
}

TEST(WeekdayDistribution, test_whatever) {
    double expected_average = 7.5;
    double expected_max = 10.5;
    int expected_max_value_bin = 6;
    QVector<Pomodoro> increasingPomodoros;
    for (int i = 1; i < 15; ++i) {
        for (int j = 0; j < i; ++j) {
            increasingPomodoros << Pomodoro {QString("Whatever"),
                                             QDateTime(QDate(2015, 6, i)),
                                             QDateTime(QDate(2015, 6, i))};
        }
    }
    QVector<double> expected_distribution {4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5};

    PomoWeekdayDistribution distribution {increasingPomodoros};
    QVector<double> distributionVector = distribution.getDistributionVector();

    CHECK_EQUAL(expected_max_value_bin, distribution.getMaxValueBin())
    DOUBLES_EQUAL(expected_average, distribution.getAverage(), threshold)
    DOUBLES_EQUAL(expected_max, distribution.getMax(), threshold)
    for (int i = 0; i < expected_distribution.size(); ++i) {
        DOUBLES_EQUAL(expected_distribution[i], distributionVector[i], threshold)
    }
}
