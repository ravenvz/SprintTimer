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

#include <utility>
#include "core/StatefulTimer.h"

namespace {
    SprintTimerCore::Idle idle;
    SprintTimerCore::RunningSprint runningSprint;
    SprintTimerCore::ShortBreak shortBreak;
    SprintTimerCore::LongBreak longBreak;
    SprintTimerCore::Zone zone;
    SprintTimerCore::SprintFinished sprintFinished;
} // namespace

namespace SprintTimerCore {

StatefulTimer::StatefulTimer(
        std::function<void(std::chrono::seconds timeLeft)> onTickCallback,
        std::function<void(IStatefulTimer::StateId)> onStateChangedCallback,
        std::chrono::seconds tickPeriod,
        const IConfig& applicationSettings)
        : currentState{&idle}
        , applicationSettings{applicationSettings}
        , tickInterval{tickPeriod}
        , onTickCallback{std::move(onTickCallback)}
        , onStateChangedCallback{std::move(onStateChangedCallback)}
        , mStart{DateTime::currentDateTimeLocal()}
{
}

void StatefulTimer::start()
{
    // TODO probably usage should be limited to Idle and Finished states
    currentState->setNextState(*this);
}

void StatefulTimer::cancel() { currentState->cancelExit(*this); }

std::chrono::seconds StatefulTimer::currentDuration() const
{
    return currentState->duration(*this);
}

void StatefulTimer::toggleInTheZoneMode() { currentState->toggleZoneMode(*this); }

std::vector<TimeSpan> StatefulTimer::completedSprints() const
{
    return buffer;
}

void StatefulTimer::clearSprintsBuffer() { buffer.clear(); }

void StatefulTimer::setNumFinishedSprints(int num)
{
    finishedSprints = num;
}

int StatefulTimer::numFinishedSprints() const { return finishedSprints; }

void StatefulTimer::jumpToState(TimerState& state)
{
    currentState = &state;
    currentState->enter(*this);
}

void StatefulTimer::transitionToState(TimerState& state)
{
    currentState->normalExit(*this);
    currentState = &state;
    currentState->enter(*this);
}

bool StatefulTimer::longBreakConditionMet() const
{
    return finishedSprints%applicationSettings.numSprintsBeforeBreak()==0;
}

void StatefulTimer::onTimerRunout()
{
    currentState->onTimerFinished(*this);
}

void StatefulTimer::notifyStateChanged(IStatefulTimer::StateId stateId)
{
    onStateChangedCallback(stateId);
}

void StatefulTimer::onTimerTick(CountdownTimer::TickPeriod timeLeft)
{
    using namespace std::chrono;
    onTickCallback(duration_cast<seconds>(timeLeft));
}

void StatefulTimer::startCountdown()
{
    countdownTimer.release();
    countdownTimer = std::make_unique<CountdownTimer>(
            [this](CountdownTimer::TickPeriod timeLeft) { this->onTimerTick(timeLeft); },
            [this]() { this->onTimerRunout(); },
            currentDuration(),
            tickInterval);
}

void TimerState::toggleZoneMode(StatefulTimer& timer)
{
}

void TimerState::onTimerFinished(StatefulTimer& timer)
{
}

std::chrono::seconds TimerState::duration(const StatefulTimer& timer) const
{
    return std::chrono::minutes{timer.applicationSettings.sprintDuration()};
}

void Idle::enter(StatefulTimer& timer) const
{
    timer.notifyStateChanged(IStatefulTimer::StateId::IdleEntered);
}

void Idle::cancelExit(StatefulTimer& timer)
{
}

void Idle::normalExit(StatefulTimer& timer) const
{
    timer.notifyStateChanged(IStatefulTimer::StateId::IdleLeft);
}

void Idle::setNextState(StatefulTimer& timer)
{
    timer.transitionToState(runningSprint);
}

void RunningSprint::enter(StatefulTimer& timer) const
{
    timer.notifyStateChanged(IStatefulTimer::StateId::SprintEntered);
    timer.mStart = DateTime::currentDateTimeLocal();
    timer.startCountdown();
}

void RunningSprint::cancelExit(StatefulTimer& timer)
{
    timer.notifyStateChanged(IStatefulTimer::StateId::SprintCancelled);
    if (timer.countdownTimer)
        timer.countdownTimer->stop();
    timer.jumpToState(idle);
}

void RunningSprint::normalExit(StatefulTimer& timer) const
{
    timer.notifyStateChanged(IStatefulTimer::StateId::SprintLeft);
}

void RunningSprint::setNextState(StatefulTimer& timer)
{
    timer.transitionToState(sprintFinished);
}

std::chrono::seconds RunningSprint::duration(const StatefulTimer& timer) const
{
    return std::chrono::minutes{timer.applicationSettings.sprintDuration()};
}

void RunningSprint::toggleZoneMode(StatefulTimer& timer)
{
    timer.jumpToState(zone);
}

void RunningSprint::onTimerFinished(StatefulTimer& timer)
{
    // TODO what if finished state is cancelled after that?
    ++timer.finishedSprints;
    timer.buffer.emplace_back(
            TimeSpan{timer.mStart, DateTime::currentDateTimeLocal()});
    setNextState(timer);
}

void ShortBreak::enter(StatefulTimer& timer) const
{
    timer.notifyStateChanged(IStatefulTimer::StateId::BreakEntered);
    timer.startCountdown();
}

void ShortBreak::cancelExit(StatefulTimer& timer)
{
    if (timer.countdownTimer)
        timer.countdownTimer->stop();
    timer.notifyStateChanged(IStatefulTimer::StateId::BreakCancelled);
    timer.jumpToState(idle);
}

void ShortBreak::normalExit(StatefulTimer& timer) const
{
    timer.notifyStateChanged(IStatefulTimer::StateId::BreakLeft);
}

void ShortBreak::setNextState(StatefulTimer& timer)
{
    timer.transitionToState(idle);
}

std::chrono::seconds ShortBreak::duration(const StatefulTimer& timer) const
{
    return std::chrono::minutes{timer.applicationSettings.shortBreakDuration()};
}

void ShortBreak::onTimerFinished(StatefulTimer& timer)
{
    setNextState(timer);
}

void LongBreak::enter(StatefulTimer& timer) const
{
    timer.notifyStateChanged(IStatefulTimer::StateId::BreakEntered);
    timer.startCountdown();
}

void LongBreak::cancelExit(StatefulTimer& timer)
{
    if (timer.countdownTimer)
        timer.countdownTimer->stop();
    timer.notifyStateChanged(IStatefulTimer::StateId::BreakCancelled);
    timer.jumpToState(idle);
}

void LongBreak::normalExit(StatefulTimer& timer) const
{
    timer.notifyStateChanged(IStatefulTimer::StateId::BreakLeft);
}

void LongBreak::setNextState(StatefulTimer& timer)
{
    timer.transitionToState(idle);
}

std::chrono::seconds LongBreak::duration(const StatefulTimer& timer) const
{
    return std::chrono::minutes{timer.applicationSettings.longBreakDuration()};
}

void LongBreak::onTimerFinished(StatefulTimer& timer)
{
    setNextState(timer);
}

void Zone::enter(StatefulTimer& timer) const
{
    timer.notifyStateChanged(IStatefulTimer::StateId::ZoneEntered);
}

void Zone::cancelExit(StatefulTimer& timer)
{
}

void Zone::normalExit(StatefulTimer& timer) const
{
    timer.notifyStateChanged(IStatefulTimer::StateId::ZoneLeft);
}

std::chrono::seconds Zone::duration(const StatefulTimer& timer) const
{
    return std::chrono::minutes{timer.applicationSettings.sprintDuration()};
}

void Zone::setNextState(StatefulTimer& timer)
{
}

void Zone::toggleZoneMode(StatefulTimer& timer)
{
    // timer.changeState because we don't want to enter/exit to be called
    timer.currentState = &runningSprint;
    timer.notifyStateChanged(IStatefulTimer::StateId::ZoneLeft);
}

void Zone::onTimerFinished(StatefulTimer& timer)
{
    timer.buffer.emplace_back(
            TimeSpan{timer.mStart, DateTime::currentDateTimeLocal()});
    timer.mStart = DateTime::currentDateTimeLocal();
    if (timer.countdownTimer)
        timer.countdownTimer->stop();
    timer.startCountdown();
}

void SprintFinished::enter(StatefulTimer& timer) const
{
    timer.notifyStateChanged(IStatefulTimer::StateId::SprintFinished);
}

void SprintFinished::cancelExit(StatefulTimer& timer)
{
    timer.clearSprintsBuffer();
    timer.jumpToState(idle);
}

void SprintFinished::normalExit(StatefulTimer& timer) const
{
}

void SprintFinished::setNextState(StatefulTimer& timer)
{
    timer.longBreakConditionMet() ? timer.transitionToState(longBreak) : timer.transitionToState(shortBreak);
}

} // namespace SprintTimerCore
