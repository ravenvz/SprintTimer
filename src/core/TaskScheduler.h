#ifndef POMODORO_TASKSCHEDULER_H
#define POMODORO_TASKSCHEDULER_H

#include <QSettings>
#include <QTimer>
#include <core/timeinterval.h>
#include "config.h"


/*
 * Responsible for managing schedule of tasks and breaks,
 * that is for interleaving tasks with short and long breaks.
 */
class TaskScheduler {

public:
    enum class TaskState { Task, ShortBreak, LongBreak };
    explicit TaskScheduler(int numTasksBeforeBreak = 4);
    void setNextState();
    void cancelState();
    bool isBreak() const;
    int numCompletedTasks();
    void setNumCompletedTasks(int numTasks);
    void toggleInTheZoneMode();
    TaskScheduler::TaskState state() const;

private:
    int numTasksBeforeBreak;
    int completedTasks;
    bool inTheZoneMode;
    TaskState currentState;
};


#endif // POMODORO_TASKSCHEDULER_H
