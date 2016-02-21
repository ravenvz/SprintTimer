#include "TaskScheduler.h"
#include "config.h"


TaskScheduler::TaskScheduler(IConfig* applicationSettings) :
        applicationSettings(applicationSettings),
        completedTasks(0),
        inTheZoneMode(false),
        currentState(TaskState::TASK)
{
}

void TaskScheduler::startTask() {
    currentTimeInterval = TimeInterval();
    currentTimeInterval.startTime = QDateTime::currentDateTime();
}

TimeInterval TaskScheduler::finishTask() {
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

void TaskScheduler::cancelTask() {
    if (currentState != TaskState::TASK) {
        setNextState();
    }
}

void TaskScheduler::setNextState() {
    if (currentState != TaskState::TASK) {
        currentState = TaskState::TASK;
    } else {
        if (completedTasks % applicationSettings->getTasksBeforeBreak() == 0) {
            currentState = TaskState::LONG_BREAK;
        } else {
            currentState = TaskState::SHORT_BREAK;
        }
    }
}

unsigned TaskScheduler::getTaskDurationInMinutes() {
    switch (currentState) {
        case TaskState::TASK:
            return applicationSettings->getPomodoroDuration();
        case TaskState::LONG_BREAK:
            return applicationSettings->getLongBreakDuration();
        case TaskState::SHORT_BREAK:
            return applicationSettings->getShortBreakDuration();
        default:
            return applicationSettings->getShortBreakDuration();
    }
}

unsigned TaskScheduler::numCompletedTasks() {
    return completedTasks;
}

void TaskScheduler::setNumCompletedTasks(unsigned numTasks) {
    TaskScheduler::completedTasks = numTasks;
}

bool TaskScheduler::isBreak() {
    return currentState != TaskState::TASK;
}

void TaskScheduler::toggleInTheZoneMode() {
    inTheZoneMode = !inTheZoneMode;
}
