#ifndef POMODORO_TASKSCHEDULER_H
#define POMODORO_TASKSCHEDULER_H

#include <QSettings>
#include <core/timeinterval.h>
#include "config.h"


struct DateInterval {
    QDate startDate;
    QDate endDate;

    QString toString() const {
        return QString("%1 - %2").arg(startDate.toString()).arg(endDate.toString());
    }

    int sizeInDays() const {
        return int(startDate.daysTo(endDate) + 1);
    }
};


class TaskScheduler {

public:
    TaskScheduler(IConfig* applicationSettings);
    void startTask();
    TimeInterval finishTask();
    void cancelTask();
    void setNextState();
    unsigned getTaskDurationInMinutes();
    bool isBreak();
    unsigned getNumCompletedTasks();
    void setNumCompletedTasks(unsigned numTasks);
    void toggleInTheZoneMode();

private:
    IConfig* applicationSettings;
    unsigned completedTasks;
    bool inTheZoneMode;
    enum class TaskState { TASK, SHORT_BREAK, LONG_BREAK  };
    TaskState currentState;
    TimeInterval currentTimeInterval;
};


#endif //POMODORO_TASKSCHEDULER_H
