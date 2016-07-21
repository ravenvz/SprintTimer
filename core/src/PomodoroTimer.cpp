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

PomodoroTimer::PomodoroTimer(
    std::function<void(long timeLeft)> onTickCallback,
    std::function<void(IPomodoroTimer::State)> onStateChangedCallback,
    long tickPeriodInMillisecs,
    const IConfig& applicationSettings)
    : applicationSettings{applicationSettings}
    , tickInterval{tickPeriodInMillisecs}
    , onTickCallback{onTickCallback}
    , onStateChangedCallback{onStateChangedCallback}
    , mStart{DateTime::currentDateTimeLocal()}
    , idleState{std::make_unique<IdleState>(*this)}
    , pomodoroState{std::make_unique<TaskState>(*this)}
    , shortBreakState{std::make_unique<ShortBreakState>(*this)}
    , longBreakState{std::make_unique<LongBreakState>(*this)}
    , zoneState{std::make_unique<ZoneState>(*this)}
    , finishedState{std::make_unique<FinishedState>(*this)}
    , currentState{idleState.get()}
{
}

void PomodoroTimer::start()
{
    currentState->setNextState();
    resetTimer();
    currentState->start();
}

void PomodoroTimer::cancel() { currentState->cancel(); }

int PomodoroTimer::taskDuration() const { return currentState->duration(); }

IPomodoroTimer::State PomodoroTimer::state() const
{
    return currentState->state();
}

void PomodoroTimer::toggleInTheZoneMode() { currentState->toggleZoneMode(); }

std::vector<TimeSpan> PomodoroTimer::completedTaskIntervals() const
{
    return buffer;
}

void PomodoroTimer::clearIntervalsBuffer() { buffer.clear(); }

void PomodoroTimer::setNumCompletedPomodoros(int num)
{
    completedPomodoros = num;
}

int PomodoroTimer::numCompletedPomodoros() const { return completedPomodoros; }

void PomodoroTimer::onTimerTick()
{
    millisecondsLeft -= tickInterval;
    if (millisecondsLeft.count() == 0) {
        currentState->setNextState();
    }
    onTickCallback(millisecondsLeft.count());
}

void PomodoroTimer::resetTimer()
{
    timerPtr = std::make_unique<Timer>(
        std::bind(&PomodoroTimer::onTimerTick, this), tickInterval);
}
