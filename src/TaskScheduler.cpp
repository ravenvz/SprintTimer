#include "TaskScheduler.h"

TaskScheduler::TaskScheduler(Config applicationConfig, unsigned completedTasks) :
        config(applicationConfig),
        completedTasks(completedTasks),
        inTheZoneMode(false),
        currentState(TaskState::TASK)
{
}

void TaskScheduler::startTask() {
    currentTimeInterval = TimeInterval();
    currentTimeInterval.startTime = std::chrono::system_clock::now();
}

TimeInterval TaskScheduler::finishTask() {
    TaskState previousState = currentState;
    currentTimeInterval.finishTime = std::chrono::system_clock::now();
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
        if (completedTasks % config.tasksBeforeBreak == 0) {
            currentState = TaskState::LONG_BREAK;
        } else {
            currentState = TaskState::SHORT_BREAK;
        }
    }
}

unsigned TaskScheduler::getTaskDurationInMinutes() {
    switch (currentState) {
        case TaskState::TASK:
            return config.pomodoroDuration;
        case TaskState::SHORT_BREAK:
            return config.breakDuration;
        default:
            return config.longBreakDuration;
    }
}

unsigned TaskScheduler::getNumCompletedTasks() {
    return completedTasks;
}

bool TaskScheduler::isBreak() {
    return currentState != TaskState::TASK;
}

void TaskScheduler::toggleInTheZoneMode() {
    inTheZoneMode = inTheZoneMode ? false : true;
}
