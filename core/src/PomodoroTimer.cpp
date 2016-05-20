#include "PomodoroTimer.h"

PomodoroTimer::PomodoroTimer(std::function<void(long timeLeft)> tickCallback,
                             long tickPeriodInMillisecs,
                             const IConfig& applicationSettings)
    : applicationSettings{applicationSettings}
    , tickInterval{tickPeriodInMillisecs}
    , onTickCallback{tickCallback}
    , mStart{std::chrono::system_clock::time_point{}}
    , shortBreakState{std::make_unique<ShortBreakState>(*this)}
    , longBreakState{std::make_unique<LongBreakState>(*this)}
    , pomodoroState{std::make_unique<PomodoroState>(*this)}
    , currentState{pomodoroState.get()}
{
}

void PomodoroTimer::run()
{
    if (running)
        return;
    running = true;
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
    if (running) {
        timerPtr->stop();
    }
    running = false;
    currentState->cancel();
}

TimeSpan PomodoroTimer::finish()
{
    if (currentState == pomodoroState.get())
        completedPomodoros++;
    currentState->setNextState();
    if (running)
        timerPtr->stop();
    running = false;
    return TimeSpan{mStart, DateTime::currentDateTime()};
}

int PomodoroTimer::taskDuration() { return currentState->duration(); }

bool PomodoroTimer::isBreak() const { return currentState->isBreak(); }

void PomodoroTimer::onTimerTick()
{
    currentTaskDuration -= tickInterval;
    if (currentTaskDuration.count() == 0) {
        timerPtr->stop();
    }
    onTickCallback(currentTaskDuration.count());
}

void PomodoroTimer::toggleInTheZoneMode() { inTheZoneMode = !inTheZoneMode; }

void PomodoroTimer::setNumCompletedPomodoros(int num)
{
    completedPomodoros = num;
}

int PomodoroTimer::numCompletedPomodoros() const { return completedPomodoros; }
