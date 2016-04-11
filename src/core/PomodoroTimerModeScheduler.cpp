#include "PomodoroTimerModeScheduler.h"
#include "config.h"


PomodoroTimerModeScheduler::PomodoroTimerModeScheduler(int pomodorosBeforeBreak)
    : pomodorosBeforeBreak{pomodorosBeforeBreak}
    , completedPomodoros(0)
    , inTheZoneMode(false)
    , currentMode(PomodoroTimerMode::Task)
{
}

void PomodoroTimerModeScheduler::setNextMode()
{
    if (isBreak() || inTheZoneMode) {
        currentMode = PomodoroTimerMode::Task;
        if (inTheZoneMode)
            ++completedPomodoros;
        return;
    }
    completedPomodoros++;
    currentMode = completedPomodoros % pomodorosBeforeBreak == 0
        ? PomodoroTimerMode::LongBreak
        : PomodoroTimerMode::ShortBreak;
}

void PomodoroTimerModeScheduler::cancelMode()
{
    if (isBreak()) {
        currentMode = PomodoroTimerMode::Task;
    }
}

int PomodoroTimerModeScheduler::numCompletedPomodoros() const
{
    return completedPomodoros;
}

void PomodoroTimerModeScheduler::setNumCompletedPomodoros(int numTasks)
{
    completedPomodoros = numTasks;
}

bool PomodoroTimerModeScheduler::isBreak() const
{
    return currentMode != PomodoroTimerMode::Task;
}

void PomodoroTimerModeScheduler::toggleInTheZoneMode()
{
    inTheZoneMode = !inTheZoneMode;
}

PomodoroTimerModeScheduler::PomodoroTimerMode
PomodoroTimerModeScheduler::mode() const
{
    return currentMode;
}
