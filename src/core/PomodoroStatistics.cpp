#include "PomodoroStatistics.h"

//PomodoroStatistics::PomodoroStatistics(QVector<Pomodoro>& pomodoros) :
//    pomodoros(pomodoros),
//
//{
//}
//
//QVector<PomodoroStatItem> PomodoroStatistics::getStatistics() {
//    return statistics;
//}
//
//void PomodoroStatistics::processPomodoros() {
//    if (pomodoros.empty()) {
//        return;
//    }
////    QDate from = pomodoros.first().getStartTime().date();
////    QDate to = pomodoros.last().getStartTime().date();
//    PomoWeekdayDistribution weekDayDistribution;
//    PomoWorkTimeDistribution workTimeDistribution;
////    QVector<int> daysOfWeekCount(7, 0);
////    for (int i = 0; i < from.daysTo(to); ++i) {
////        daysOfWeekCount[from.addDays(i).dayOfWeek()]++;
////    }
//    for (Pomodoro& pomo : pomodoros) {
//        weekDayDistribution.incrementBinValue(pomo.getStartTime().date().dayOfWeek() - 1);
//    }
////    for (int i = 1; i < daysOfWeekCount.size(); ++i) {
////        workdayDistribution[i] /= double(daysOfWeekCount[i]);
////    }
//}
