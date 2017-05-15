/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#include "core/StatefulTimer.h"

StatefulTimer::StatefulTimer(
    std::function<void(long long timeLeft)> onTickCallback,
    std::function<void(IStatefulTimer::State)> onStateChangedCallback,
    long tickPeriodInMillisecs,
    const IConfig& applicationSettings)
    : idleState{std::make_unique<Idle>(*this)}
    , sprintState{std::make_unique<RunningSprint>(*this)}
    , shortBreakState{std::make_unique<ShortBreak>(*this)}
    , longBreakState{std::make_unique<LongBreak>(*this)}
    , zoneState{std::make_unique<Zone>(*this)}
    , finishedState{std::make_unique<Finished>(*this)}
    , currentState{idleState.get()}
    , applicationSettings{applicationSettings}
    , tickInterval{tickPeriodInMillisecs}
    , onTickCallback{onTickCallback}
    , onStateChangedCallback{onStateChangedCallback}
    , mStart{DateTime::currentDateTimeLocal()}
{
}

void StatefulTimer::start()
{
    currentState->setNextState();
    resetTimer();
    currentState->start();
}

void StatefulTimer::cancel() { currentState->cancel(); }

int StatefulTimer::currentDuration() const { return currentState->duration(); }

IStatefulTimer::State StatefulTimer::state() const
{
    return currentState->state();
}

void StatefulTimer::toggleInTheZoneMode() { currentState->toggleZoneMode(); }

std::vector<TimeSpan> StatefulTimer::completedTaskIntervals() const
{
    return buffer;
}

void StatefulTimer::clearIntervalsBuffer() { buffer.clear(); }

void StatefulTimer::setNumFinishedSprints(int num)
{
    finishedSprints = num;
}

int StatefulTimer::numFinishedSprints() const { return finishedSprints; }

void StatefulTimer::onTimerTick()
{
    millisecondsLeft -= tickInterval;
    if (millisecondsLeft.count() == 0) {
        currentState->setNextState();
    }
    onTickCallback(millisecondsLeft.count());
}

void StatefulTimer::resetTimer()
{
    timerPtr = std::make_unique<Timer>(
            [this]() { this->onTimerTick(); },
            tickInterval);
}

void TimerState::notifyStateChanged(IStatefulTimer::State state)
{
    timer.onStateChangedCallback(state);
}

Idle::Idle(StatefulTimer& timer)
    : TimerState{timer}
{
}

IStatefulTimer::State Idle::state() const
{
    return IStatefulTimer::State::Idle;
}

void Idle::setNextState()
{
    timer.currentState = timer.sprintState.get();
    notifyStateChanged(timer.currentState->state());
}

int Idle::duration() const
{
    return timer.applicationSettings.sprintDuration();
}

RunningSprint::RunningSprint(StatefulTimer& timer)
    : TimerState{timer}
{
}

IStatefulTimer::State RunningSprint::state() const
{
    return IStatefulTimer::State::Task;
}

void RunningSprint::start()
{
    timer.mStart = DateTime::currentDateTimeLocal();
    timer.millisecondsLeft
        = std::chrono::milliseconds{duration() * millisecondsInMinute};
    timer.timerPtr->start();
}

void RunningSprint::setNextState()
{
    if (timer.timerPtr)
        timer.timerPtr->stop();
    ++timer.finishedSprints;
    timer.buffer.emplace_back(
        TimeSpan{timer.mStart, DateTime::currentDateTimeLocal()});
    timer.currentState = timer.finishedState.get();
    notifyStateChanged(timer.currentState->state());
}

void RunningSprint::cancel()
{
    if (timer.timerPtr)
        timer.timerPtr->stop();
    timer.currentState = timer.idleState.get();
    notifyStateChanged(timer.currentState->state());
}

int RunningSprint::duration() const
{
    return timer.applicationSettings.sprintDuration();
}

void RunningSprint::toggleZoneMode()
{
    timer.currentState = timer.zoneState.get();
    notifyStateChanged(timer.currentState->state());
}

ShortBreak::ShortBreak(StatefulTimer& timer)
    : TimerState{timer}
{
}

IStatefulTimer::State ShortBreak::state() const
{
    return IStatefulTimer::State::Break;
}

void ShortBreak::start()
{
    timer.millisecondsLeft
        = std::chrono::milliseconds{duration() * millisecondsInMinute};
    timer.timerPtr->start();
}

void ShortBreak::setNextState()
{
    if (timer.timerPtr)
        timer.timerPtr->stop();
    timer.currentState = timer.idleState.get();
    notifyStateChanged(timer.currentState->state());
}

void ShortBreak::cancel() { setNextState(); }

int ShortBreak::duration() const
{
    return timer.applicationSettings.shortBreakDuration();
}

LongBreak::LongBreak(StatefulTimer& timer)
    : TimerState{timer}
{
}

IStatefulTimer::State LongBreak::state() const
{
    return IStatefulTimer::State::LongBreak;
}

void LongBreak::start()
{
    timer.millisecondsLeft
        = std::chrono::milliseconds{duration() * millisecondsInMinute};
    timer.timerPtr->start();
}

void LongBreak::setNextState()
{
    if (timer.timerPtr)
        timer.timerPtr->stop();
    timer.currentState = timer.idleState.get();
    notifyStateChanged(timer.currentState->state());
}

void LongBreak::cancel() { setNextState(); }

int LongBreak::duration() const
{
    return timer.applicationSettings.longBreakDuration();
}

Zone::Zone(StatefulTimer& timer)
    : TimerState{timer}
{
}

void Zone::start() { timer.mStart = DateTime::currentDateTimeLocal(); }

IStatefulTimer::State Zone::state() const
{
    return IStatefulTimer::State::ZoneEntered;
}

void Zone::setNextState()
{
    timer.buffer.emplace_back(
        TimeSpan{timer.mStart, DateTime::currentDateTimeLocal()});
    timer.mStart = DateTime::currentDateTimeLocal();
    timer.millisecondsLeft
        = std::chrono::milliseconds{duration() * millisecondsInMinute};
}

int Zone::duration() const
{
    return timer.applicationSettings.sprintDuration();
}

void Zone::toggleZoneMode()
{
    timer.currentState = timer.sprintState.get();
    notifyStateChanged(IStatefulTimer::State::ZoneLeft);
}

Finished::Finished(StatefulTimer& timer)
    : TimerState{timer}
{
}

IStatefulTimer::State Finished::state() const
{
    return IStatefulTimer::State::Finished;
}

void Finished::setNextState()
{
    if (timer.finishedSprints
            % timer.applicationSettings.numSprintsBeforeBreak()
        == 0) {
        timer.currentState = timer.longBreakState.get();
    }
    else {
        timer.currentState = timer.shortBreakState.get();
    }
    notifyStateChanged(timer.currentState->state());
}

void Finished::cancel()
{
    timer.currentState = timer.idleState.get();
    timer.buffer.clear();
    notifyStateChanged(timer.currentState->state());
}

int Finished::duration() const { return 0; }
