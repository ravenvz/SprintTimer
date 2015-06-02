#ifndef POMODORO_TASKSCHEDULER_H
#define POMODORO_TASKSCHEDULER_H

#include <qt/QtCore/QDateTime>


struct Config {
    const unsigned pomodoroDuration;
    const unsigned breakDuration;
    const unsigned longBreakDuration;
    const unsigned tasksBeforeBreak;
};


struct TimeInterval {
    QDateTime startTime;
    QDateTime finishTime;
};


class TaskScheduler {

public:
    TaskScheduler(Config applicationConfig, unsigned completedPomodoros);
    void startTask();
    TimeInterval finishTask();
    void cancelTask();
    void setNextState();
    unsigned getTaskDurationInMinutes();
    bool isBreak();
    unsigned getNumCompletedTasks();
    void toggleInTheZoneMode();

private:
    Config config;
    unsigned completedTasks;
    bool inTheZoneMode;
    enum class TaskState { TASK, SHORT_BREAK, LONG_BREAK  };
    TaskState currentState;
    TimeInterval currentTimeInterval;
};



#endif //POMODORO_TASKSCHEDULER_H
