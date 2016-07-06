/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "core/PomodoroTimer.h"

PomodoroTimer::PomodoroTimer(std::function<void(long timeLeft)> tickCallback,
                             long tickPeriodInMillisecs,
                             const IConfig& applicationSettings)
    : applicationSettings{applicationSettings}
    , tickInterval{tickPeriodInMillisecs}
    , onTickCallback{tickCallback}
    , mStart{DateTime::currentDateTimeLocal()}
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
    mStart = DateTime::currentDateTimeLocal();
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
    return TimeSpan{mStart, DateTime::currentDateTimeLocal()};
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
