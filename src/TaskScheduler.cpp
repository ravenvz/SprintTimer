#include "TaskScheduler.h"

TaskScheduler::TaskScheduler(Config& config, unsigned completedTasks) :
        config(config),
        completedTasks(completedTasks),
        inTheZoneMode(false),
        currentState(TaskState::TASK)
{
}

void TaskScheduler::startTask() {
    startTime = std::chrono::system_clock::now();
}

void TaskScheduler::finishTask() {
    TaskState previousState = currentState;
    if (previousState != TaskState::TASK) {
        setNextState();
        return;
    }
    completedTasks++;
    setNextState();
//        finishTime = std::chrono::system_clock::now();

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

unsigned TaskScheduler::getTaskDuration() {
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