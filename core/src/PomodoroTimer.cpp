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

int PomodoroTimer::currentDuration() const { return currentState->duration(); }

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

void PomodoroTimerState::notifyStateChanged(IPomodoroTimer::State state)
{
    timer.onStateChangedCallback(state);
}

IdleState::IdleState(PomodoroTimer& timer)
    : PomodoroTimerState{timer}
{
}

IPomodoroTimer::State IdleState::state() const
{
    return IPomodoroTimer::State::Idle;
}

void IdleState::setNextState()
{
    timer.currentState = timer.pomodoroState.get();
    notifyStateChanged(timer.currentState->state());
}

int IdleState::duration() const
{
    return timer.applicationSettings.pomodoroDuration();
}

TaskState::TaskState(PomodoroTimer& timer)
    : PomodoroTimerState{timer}
{
}

IPomodoroTimer::State TaskState::state() const
{
    return IPomodoroTimer::State::Task;
}

void TaskState::start()
{
    timer.mStart = DateTime::currentDateTimeLocal();
    timer.millisecondsLeft
        = std::chrono::milliseconds{duration() * timer.millisecondsInMinute};
    timer.timerPtr->start();
}

void TaskState::setNextState()
{
    if (timer.timerPtr)
        timer.timerPtr->stop();
    ++timer.completedPomodoros;
    timer.buffer.emplace_back(
        TimeSpan{timer.mStart, DateTime::currentDateTimeLocal()});
    timer.currentState = timer.finishedState.get();
    notifyStateChanged(timer.currentState->state());
}

void TaskState::cancel()
{
    if (timer.timerPtr)
        timer.timerPtr->stop();
    timer.currentState = timer.idleState.get();
    notifyStateChanged(timer.currentState->state());
}

int TaskState::duration() const
{
    return timer.applicationSettings.pomodoroDuration();
}

void TaskState::toggleZoneMode()
{
    timer.currentState = timer.zoneState.get();
    notifyStateChanged(timer.currentState->state());
}

ShortBreakState::ShortBreakState(PomodoroTimer& timer)
    : PomodoroTimerState{timer}
{
}

IPomodoroTimer::State ShortBreakState::state() const
{
    return IPomodoroTimer::State::Break;
}

void ShortBreakState::start()
{
    timer.millisecondsLeft
        = std::chrono::milliseconds{duration() * timer.millisecondsInMinute};
    timer.timerPtr->start();
}

void ShortBreakState::setNextState()
{
    if (timer.timerPtr)
        timer.timerPtr->stop();
    timer.currentState = timer.idleState.get();
    notifyStateChanged(timer.currentState->state());
}

void ShortBreakState::cancel() { setNextState(); }

int ShortBreakState::duration() const
{
    return timer.applicationSettings.shortBreakDuration();
}

LongBreakState::LongBreakState(PomodoroTimer& timer)
    : PomodoroTimerState{timer}
{
}

IPomodoroTimer::State LongBreakState::state() const
{
    return IPomodoroTimer::State::LongBreak;
}

void LongBreakState::start()
{
    timer.millisecondsLeft
        = std::chrono::milliseconds{duration() * timer.millisecondsInMinute};
    timer.timerPtr->start();
}

void LongBreakState::setNextState()
{
    if (timer.timerPtr)
        timer.timerPtr->stop();
    timer.currentState = timer.idleState.get();
    notifyStateChanged(timer.currentState->state());
}

void LongBreakState::cancel() { setNextState(); }

int LongBreakState::duration() const
{
    return timer.applicationSettings.longBreakDuration();
}

ZoneState::ZoneState(PomodoroTimer& timer)
    : PomodoroTimerState{timer}
{
}

void ZoneState::start() { timer.mStart = DateTime::currentDateTimeLocal(); }

IPomodoroTimer::State ZoneState::state() const
{
    return IPomodoroTimer::State::ZoneEntered;
}

void ZoneState::setNextState()
{
    timer.buffer.emplace_back(
        TimeSpan{timer.mStart, DateTime::currentDateTimeLocal()});
    timer.mStart = DateTime::currentDateTimeLocal();
    timer.millisecondsLeft
        = std::chrono::milliseconds{duration() * timer.millisecondsInMinute};
}

int ZoneState::duration() const
{
    return timer.applicationSettings.pomodoroDuration();
}

void ZoneState::toggleZoneMode()
{
    timer.currentState = timer.pomodoroState.get();
    notifyStateChanged(IPomodoroTimer::State::ZoneLeft);
}

FinishedState::FinishedState(PomodoroTimer& timer)
    : PomodoroTimerState{timer}
{
}

IPomodoroTimer::State FinishedState::state() const
{
    return IPomodoroTimer::State::Finished;
}

void FinishedState::setNextState()
{
    if (timer.completedPomodoros
            % timer.applicationSettings.numPomodorosBeforeBreak()
        == 0) {
        timer.currentState = timer.longBreakState.get();
    }
    else {
        timer.currentState = timer.shortBreakState.get();
    }
    notifyStateChanged(timer.currentState->state());
}

void FinishedState::cancel()
{
    timer.currentState = timer.idleState.get();
    notifyStateChanged(timer.currentState->state());
}

int FinishedState::duration() const { return 0; }
