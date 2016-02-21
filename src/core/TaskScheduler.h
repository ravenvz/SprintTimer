#ifndef POMODORO_TASKSCHEDULER_H
#define POMODORO_TASKSCHEDULER_H

#include <QSettings>
#include <core/timeinterval.h>
#include "config.h"


class TaskScheduler {

public:
    explicit TaskScheduler(IConfig* applicationSettings);
    void startTask();
    TimeInterval finishTask();
    void cancelTask();
    void setNextState();
    unsigned getTaskDurationInMinutes();
    bool isBreak();
    unsigned numCompletedTasks();
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
