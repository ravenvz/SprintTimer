#include "core/PomodoroStatistics.h"
#include "core/TaskScheduler.h"
#include <TestHarness.h>


TEST_GROUP(PomoStatItem) {
    const double threshold = 0.00001;
};

TEST(PomoStatItem, test_empty) {
    QVector<Pomodoro> pomodoros;
    DateInterval interval;
    interval.startDate = QDate::currentDate();
    interval.endDate = QDate::currentDate();
    PomodoroStatItem statistics {pomodoros, interval};
    double expected_average = 0;
    double expected_max = 0;
    QVector<double> expected_distribution = QVector<double> (7, 0);

    Distribution<double>* weekdayDistribution = statistics.getWeekdayDistribution();
    QVector<double> distributionVector = weekdayDistribution->getDistributionVector();

    DOUBLES_EQUAL(expected_average, weekdayDistribution->getAverage(), threshold)
    DOUBLES_EQUAL(expected_max, weekdayDistribution->getMax(), threshold)
    CHECK_EQUAL(expected_distribution.size(), distributionVector.size())
    for (int i = 0; i < expected_distribution.size(); ++i) {
        DOUBLES_EQUAL(expected_distribution[i], distributionVector[i], threshold)
    }
}

TEST(PomoStatItem, test_whatever) {
    double expected_average = 7.5;
    double expected_max = 10.5;
    int expected_max_value_bin = 6;
    QVector<Pomodoro> increasingPomodoros;
    DateInterval interval;
    interval.startDate = QDate(2015, 6, 1);
    interval.endDate = QDate(2015, 6, 14);
    for (int i = 1; i < 15; ++i) {
        for (int j = 0; j < i; ++j) {
            increasingPomodoros << Pomodoro {QString("Whatever"),
                                             QDateTime(QDate(2015, 6, i)),
                                             QDateTime(QDate(2015, 6, i))};
        }
    }
    QVector<double> expected_distribution {4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5};
    PomodoroStatItem statistics {increasingPomodoros, interval};

    Distribution<double>* weekdayDistribution = statistics.getWeekdayDistribution();
    QVector<double> distributionVector = weekdayDistribution->getDistributionVector();

    CHECK_EQUAL(expected_max_value_bin, weekdayDistribution->getMaxValueBin())
    DOUBLES_EQUAL(expected_average, weekdayDistribution->getAverage(), threshold)
    DOUBLES_EQUAL(expected_max, weekdayDistribution->getMax(), threshold)
    for (int i = 0; i < expected_distribution.size(); ++i) {
        DOUBLES_EQUAL(expected_distribution[i], distributionVector[i], threshold)
    }
}
