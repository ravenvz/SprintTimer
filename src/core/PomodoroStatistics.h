#ifndef POMODORO_POMODOROSTATISTICS_H
#define POMODORO_POMODOROSTATISTICS_H


#include <QtCore/qvector.h>
#include "core/entities/Pomodoro.h"
#include <QDebug>
#include "utils/MathUtils.h"


class PomoWeekdayDistribution : public Distribution
{

public:
    PomoWeekdayDistribution(const QVector<Pomodoro>& pomodoros) :
        Distribution(7)
    {
        if (pomodoros.empty()) return;
        distributeToBins(pomodoros);
        countWeekdays();
        normalizeByWeekdayCount();
        computeMaxAndAverage(pomodoros.size());
    }

    void distributeToBins(const QVector<Pomodoro>& pomodoros) {
        for (const Pomodoro& pomo : pomodoros) {
            QDate pomoDate = pomo.getStartTime().date();
            minDate = std::min(minDate, pomoDate);
            maxDate = std::max(maxDate, pomoDate);
            incrementBinValue(pomo.getStartTime().date().dayOfWeek() - 1);
        }
    }

    void countWeekdays() {
        for (int weekdayNum = 0; weekdayNum <= minDate.daysTo(maxDate); ++weekdayNum) {
            weekdayCount[minDate.addDays(weekdayNum).dayOfWeek() - 1]++;
        }
    }

    void normalizeByWeekdayCount() {
        for (int i = 0; i < distribution.size(); ++i) {
            if (weekdayCount[i] > 0)
                distribution[i] /= weekdayCount[i];
        }
    }

private:
    // TODO if ensured that pomodoros are sorted by date, it is possible
    // to use first and last pomodoros to retrieve dates.
    QDate minDate = QDate(3000, 1, 1);
    QDate maxDate = QDate(1, 1, 1);
    QVector<int> weekdayCount = QVector<int> (7, 0);

};


class PomoWorkTimeDistribution : public Distribution
{

public:
    PomoWorkTimeDistribution(const QVector<Pomodoro>& pomodoros) :
            Distribution(7)
    {
        if (pomodoros.empty()) return;
        distributeToBins(pomodoros);
        countWorkHours();
        normalizeByWorkHours();
        computeMaxAndAverage(numBins);
    }

    void distributeToBins(const QVector<Pomodoro>& pomodoros) {
        for (const Pomodoro& pomo : pomodoros) {
            int pomoHour = pomo.getStartTime().time().hour();
            if (22 < pomoHour || pomoHour <= 2) {
                incrementBinValue(0);
            } else if (2 < pomoHour && pomoHour <= 6) {
                incrementBinValue(1);
            } else if (6 < pomoHour && pomoHour <= 10) {
                incrementBinValue(2);
            } else if (10 < pomoHour && pomoHour <= 14) {
                incrementBinValue(3);
            } else if (14 < pomoHour && pomoHour <= 18) {
                incrementBinValue(4);
            } else if (18 < pomoHour && pomoHour <= 22) {
                incrementBinValue(5);
            }
        }
    }

    void countWorkHours() {

    }

    void normalizeByWorkHours() {

    }

};

// TODO this is actually solvable by a single DB query, not yet sure
// how to handle it best
class PomoDailyDistribution : public Distribution
{

public:
    PomoDailyDistribution(QVector<Pomodoro>& pomodoros) :
        Distribution(30) {
    }

};


class PomodoroStatItem
{
public:
    PomodoroStatItem(QVector<Pomodoro>& pomodoros) :
        weekdayDistribution(PomoWeekdayDistribution {pomodoros}),
        workTimeDistribution(PomoWorkTimeDistribution {pomodoros}),
        dailyDistribution(PomoDailyDistribution {pomodoros})
    {
    }

    const PomoWeekdayDistribution& getWeekdayDistribution() const {
        return weekdayDistribution;
    }

    const PomoWorkTimeDistribution& getWorkTimeDistribution() const {
        return workTimeDistribution;
    }

    const PomoDailyDistribution& getDailyDistribution() const {
        return dailyDistribution;
    }

private:
    PomoWeekdayDistribution weekdayDistribution;
    PomoWorkTimeDistribution workTimeDistribution;
    PomoDailyDistribution dailyDistribution;
};


#endif //POMODORO_POMODOROSTATISTICS_H
