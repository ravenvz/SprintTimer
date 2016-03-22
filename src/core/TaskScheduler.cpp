#include "TaskScheduler.h"
#include "config.h"


TaskScheduler::TaskScheduler(const IConfig& applicationSettings)
    : applicationSettings(applicationSettings)
    , completedTasks(0)
    , inTheZoneMode(false)
    , currentState(TaskState::TASK)
{
}

void TaskScheduler::startTask()
{
    currentTimeInterval = TimeInterval();
    currentTimeInterval.startTime = QDateTime::currentDateTime();
}

TimeInterval TaskScheduler::finishTask()
{
    TaskState previousState = currentState;
    currentTimeInterval.finishTime = QDateTime::currentDateTime();
    if (previousState != TaskState::TASK) {
        setNextState();
        return currentTimeInterval;
    }
    completedTasks++;
    if (!inTheZoneMode) {
        setNextState();
    }
    return currentTimeInterval;
}

void TaskScheduler::cancelTask()
{
    if (currentState != TaskState::TASK) {
        setNextState();
    }
}

void TaskScheduler::setNextState()
{
    if (currentState != TaskState::TASK) {
        currentState = TaskState::TASK;
    }
    else {
        if (completedTasks % applicationSettings.numTasksBeforeBreak() == 0) {
            currentState = TaskState::LONG_BREAK;
        }
        else {
            currentState = TaskState::SHORT_BREAK;
        }
    }
}

int TaskScheduler::taskDurationInMinutes()
{
    switch (currentState) {
    case TaskState::TASK:
        return applicationSettings.pomodoroDuration();
    case TaskState::LONG_BREAK:
        return applicationSettings.longBreakDuration();
    case TaskState::SHORT_BREAK:
        return applicationSettings.shortBreakDuration();
    default:
        return applicationSettings.shortBreakDuration();
    }
}

int TaskScheduler::numCompletedTasks() { return completedTasks; }

void TaskScheduler::setNumCompletedTasks(int numTasks)
{
    TaskScheduler::completedTasks = numTasks;
}

bool TaskScheduler::isBreak() { return currentState != TaskState::TASK; }

void TaskScheduler::toggleInTheZoneMode() { inTheZoneMode = !inTheZoneMode; }
