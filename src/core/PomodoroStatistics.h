#ifndef POMODORO_POMODOROSTATISTICS_H
#define POMODORO_POMODOROSTATISTICS_H


#include <QtCore/qvector.h>
#include "core/entities/Pomodoro.h"
#include <QDebug>
#include "utils/MathUtils.h"
#include "TaskScheduler.h"
#include "db_layer/db_helper.h"
#include <typeinfo>
#include <iostream>


//class PomoWorkTimeDistribution
//{
//
//public:
//    PomoWorkTimeDistribution(const QVector<Pomodoro>& pomodoros) :
//            Distribution(7)
//    {
//        if (pomodoros.empty()) return;
//        distributeToBins(pomodoros);
//        countWorkHours();
//        normalizeByWorkHours();
//        computeMaxAndAverage();
//    }
//
//    void distributeToBins(const QVector<Pomodoro>& pomodoros) {
//        for (const Pomodoro& pomo : pomodoros) {
//            int pomoHour = pomo.getStartTime().time().hour();
//            if (22 < pomoHour || pomoHour <= 2) {
//                incrementBinValue(0);
//            } else if (2 < pomoHour && pomoHour <= 6) {
//                incrementBinValue(1);
//            } else if (6 < pomoHour && pomoHour <= 10) {
//                incrementBinValue(2);
//            } else if (10 < pomoHour && pomoHour <= 14) {
//                incrementBinValue(3);
//            } else if (14 < pomoHour && pomoHour <= 18) {
//                incrementBinValue(4);
//            } else if (18 < pomoHour && pomoHour <= 22) {
//                incrementBinValue(5);
//            }
//        }
//    }
//
//    void countWorkHours() {
//
//    }
//
//    void normalizeByWorkHours() {
//
//    }
//
//};
//
//// TODO this is actually solvable by a single DB query, not yet sure
//// how to handle it best
//class PomoDailyDistribution
//{
//
//public:
//    PomoDailyDistribution(const QVector<Pomodoro>& pomodoros) :
//        Distribution(30) {
//    }
//
//};

class GoalStatItem
{

public:
    GoalStatItem() {
        lastThirtyDays = new Distribution<unsigned> {PomodoroDataSource::getPomodorosForLastThirtyDays()};
        lastTwelveWeeks = new Distribution<unsigned> {PomodoroDataSource::getPomodorosForLastTwelveWeeks()};
        lastTwelveMonths = new Distribution<unsigned> {PomodoroDataSource::getPomodorosForLastTwelveMonths()};
    }

    ~GoalStatItem() {
        delete lastThirtyDays;
        delete lastTwelveWeeks;
        delete lastTwelveMonths;
    }

    Distribution<unsigned>* getDistributionForLastThirtyDays() const {
        return lastThirtyDays;
    }

    Distribution<unsigned>* getDistributionForLastTwelveWeeks() const {
        return lastTwelveWeeks;
    }

    Distribution<unsigned>* getDistributionForLastTwelveMonths() const {
        return lastTwelveMonths;
    }

private:
    Distribution<unsigned>* lastThirtyDays;
    Distribution<unsigned>* lastTwelveWeeks;
    Distribution<unsigned>* lastTwelveMonths;
};


class PomodoroStatItem
{

public:

    PomodoroStatItem(const QVector<Pomodoro>& pomodoros, const DateInterval& dateInterval) :
        interval(dateInterval)
    {
        dailyDistribution = new Distribution<double> {computeDailyDistribution(pomodoros)};
        weekdayDistribution = new Distribution<double> {computeWeekdayDistribution(pomodoros), countWeekdays()};
    }

    explicit PomodoroStatItem(const DateInterval& dateInterval) :
        interval(dateInterval)
    {
        QVector<Pomodoro> pomodoros = PomodoroDataSource::getPomodorosBetween(dateInterval.startDate,
                                                                                    dateInterval.endDate);
        dailyDistribution = new Distribution<double> {computeDailyDistribution(pomodoros)};
        weekdayDistribution = new Distribution<double> {computeWeekdayDistribution(pomodoros), countWeekdays()};
    }

    PomodoroStatItem(const PomodoroStatItem&) = default;

    ~PomodoroStatItem() {
        delete dailyDistribution;
        delete weekdayDistribution;
    }

    Distribution<double>* getDailyDistribution() const {
        return dailyDistribution;
    }

    Distribution<double>* getWeekdayDistribution() const {
        return weekdayDistribution;
    }

    // const Distribution& getDailyDistribution() const {
    //     return dailyDistribution;
    // }

    QVector<double> computeDailyDistribution(const QVector<Pomodoro>& pomodoros) const {
        if (pomodoros.empty()) return QVector<double> (0, 0);
        QVector<double> distribution (interval.sizeInDays(), 0);
        for (const Pomodoro& pomo : pomodoros) {
            distribution[interval.startDate.daysTo(pomo.getStartTime().date())]++;
        }
        return distribution;
    }

    QVector<double> computeWeekdayDistribution(const QVector<Pomodoro>& pomodoros) const {
        QVector<double> distribution (7, 0);
        if (pomodoros.empty()) return distribution;
        for (const Pomodoro& pomo : pomodoros) {
            distribution[pomo.getStartTime().date().dayOfWeek() - 1]++;
        }
        return distribution;
    }

    QVector<int> countWeekdays() const {
        QVector<int> result (7, 0);
        for (int weekdayNum = 0; weekdayNum <= interval.startDate.daysTo(interval.endDate); ++weekdayNum) {
            result[interval.startDate.addDays(weekdayNum).dayOfWeek() - 1]++;
        }
        return result;
    }
    
private:
    const DateInterval interval;
    Distribution<double>* dailyDistribution;
    Distribution<double>* weekdayDistribution;
    // Distribution workTimeDistribution;
};


#endif //POMODORO_POMODOROSTATISTICS_H
