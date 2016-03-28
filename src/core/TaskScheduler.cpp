#include "TaskScheduler.h"
#include "config.h"


TaskScheduler::TaskScheduler(int numTasksBeforeBreak)
    : numTasksBeforeBreak{numTasksBeforeBreak}
    , completedTasks(0)
    , inTheZoneMode(false)
    , currentState(TaskState::Task)
{
}

void TaskScheduler::setNextState()
{
    if (isBreak() || inTheZoneMode) {
        currentState = TaskState::Task;
        if (inTheZoneMode)
            ++completedTasks;
        return;
    }
    completedTasks++;
    currentState = completedTasks % numTasksBeforeBreak == 0
        ? TaskState::LongBreak
        : TaskState::ShortBreak;
}

void TaskScheduler::cancelState()
{
    if (isBreak()) {
        currentState = TaskState::Task;
    }
}

int TaskScheduler::numCompletedTasks() { return completedTasks; }

void TaskScheduler::setNumCompletedTasks(int numTasks)
{
    completedTasks = numTasks;
}

bool TaskScheduler::isBreak() const { return currentState != TaskState::Task; }

void TaskScheduler::toggleInTheZoneMode() { inTheZoneMode = !inTheZoneMode; }

TaskScheduler::TaskState TaskScheduler::state() const { return currentState; }
