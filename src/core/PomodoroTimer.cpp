#include "PomodoroTimer.h"

PomodoroTimer::PomodoroTimer(std::function<void(long timeLeft)> tickCallback,
                             long tickPeriodInMillisecs,
                             const IConfig& applicationSettings)
    : applicationSettings{applicationSettings}
    , taskScheduler{}
    , tickInterval{tickPeriodInMillisecs}
    , onTickCallback{tickCallback}
    , mStart{std::chrono::system_clock::time_point{}}
    , mFinish{std::chrono::system_clock::time_point{}}
{
}

void PomodoroTimer::run()
{
    using namespace date;
    mStart = DateTime::currentDateTime();
    timerPtr = std::make_unique<Timer>(
        std::bind(&PomodoroTimer::onTimerTick, this), tickInterval);
    timerPtr->start();
    currentTaskDuration
        = std::chrono::milliseconds{taskDuration() * millisecondsInMinute};
}

void PomodoroTimer::cancel()
{
    timerPtr->stop();
    taskScheduler.cancelMode();
}

TimeSpan PomodoroTimer::finish()
{
    taskScheduler.setNextMode();
    mFinish = DateTime::currentDateTime();
    return TimeSpan{mStart, mFinish};
}

int PomodoroTimer::taskDuration()
{
    switch (taskScheduler.mode()) {
    case PomodoroTimerModeScheduler::PomodoroTimerMode::Task:
        return applicationSettings.pomodoroDuration();
    case PomodoroTimerModeScheduler::PomodoroTimerMode::LongBreak:
        return applicationSettings.longBreakDuration();
    case PomodoroTimerModeScheduler::PomodoroTimerMode::ShortBreak:
        return applicationSettings.shortBreakDuration();
    default:
        return applicationSettings.shortBreakDuration();
    }
}

bool PomodoroTimer::isBreak() const { return taskScheduler.isBreak(); }

void PomodoroTimer::onTimerTick()
{
    currentTaskDuration -= tickInterval;
    if (currentTaskDuration.count() == 0) {
        timerPtr->stop();
    }
    onTickCallback(currentTaskDuration.count());
}

void PomodoroTimer::toggleInTheZoneMode()
{
    taskScheduler.toggleInTheZoneMode();
}
