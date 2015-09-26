#ifndef POMODORO_POMODOROSTATISTICS_H
#define POMODORO_POMODOROSTATISTICS_H


#include <QVector>
#include "entities/Pomodoro.h"

class PomodoroStatistics
{

public:
    PomodoroStatistics(QVector<Pomodoro>& pomodoros);
    virtual ~PomodoroStatistics();

private:
    double dailyAverage;
    int maxCompleted;
    QVector<double> weekdayDistribution;
    QVector<double> workTimeDistribution;
    double timeBinAverage;

};


#endif //POMODORO_POMODOROSTATISTICS_H
