#ifndef POMODORO_TASKSCHEDULER_H
#define POMODORO_TASKSCHEDULER_H

#include <QSettings>
#include <QTimer>
#include <core/timeinterval.h>
#include "config.h"


class TaskScheduler {

public:
    explicit TaskScheduler(const IConfig& applicationSettings);
    void startTask();
    TimeInterval finishTask();
    void cancelTask();
    void setNextState();
    int taskDurationInMinutes();
    bool isBreak();
    int numCompletedTasks();
    void setNumCompletedTasks(int numTasks);
    void toggleInTheZoneMode();

private:
    const IConfig& applicationSettings;
    int completedTasks;
    bool inTheZoneMode;
    enum class TaskState { TASK, SHORT_BREAK, LONG_BREAK };
    TaskState currentState;
    TimeInterval currentTimeInterval;
};


#endif // POMODORO_TASKSCHEDULER_H
