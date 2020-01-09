/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "core/Workflow.h"
#include <utility>

namespace sprint_timer {

using dw::DateTime;
using dw::DateTimeRange;

Workflow::Workflow(
    std::function<void(std::chrono::seconds timeLeft)> onTickCallback,
    std::function<void(IWorkflow::StateId)> onStateChangedCallback,
    std::chrono::seconds tickPeriod,
    const IConfig& applicationSettings)
    : currentState{&idle}
    , applicationSettings{applicationSettings}
    , tickInterval{tickPeriod}
    , onTickCallback{std::move(onTickCallback)}
    , onStateChangedCallback{std::move(onStateChangedCallback)}
    , mStart{dw::current_date_time_local()}
{
}

void Workflow::start()
{
    // TODO probably usage should be limited to Idle and Finished states
    currentState->setNextState(*this);
}

void Workflow::cancel() { currentState->cancelExit(*this); }

std::chrono::seconds Workflow::currentDuration() const
{
    return currentState->duration(*this);
}

void Workflow::toggleInTheZoneMode()
{
    currentState->toggleZoneMode(*this);
}

std::vector<DateTimeRange> Workflow::completedSprints() const
{
    return buffer;
}

void Workflow::clearSprintsBuffer() { buffer.clear(); }

void Workflow::setNumFinishedSprints(int num) { finishedSprints = num; }

int Workflow::numFinishedSprints() const { return finishedSprints; }

void Workflow::jumpToState(WorkflowState& state)
{
    currentState = &state;
    currentState->enter(*this);
}

void Workflow::transitionToState(WorkflowState& state)
{
    currentState->normalExit(*this);
    currentState = &state;
    currentState->enter(*this);
}

bool Workflow::longBreakConditionMet() const
{
    return (finishedSprints + 1) % applicationSettings.numSprintsBeforeBreak()
        == 0;
}

void Workflow::onTimerRunout() { currentState->onTimerFinished(*this); }

void Workflow::notifyStateChanged(IWorkflow::StateId stateId)
{
    onStateChangedCallback(stateId);
}

void Workflow::onTimerTick(PeriodicBackgroundRunner::TickPeriod timeLeft)
{
    using namespace std::chrono;
    onTickCallback(duration_cast<seconds>(timeLeft));
}

void Workflow::startCountdown()
{
    periodicBackgroundRunner = std::make_unique<PeriodicBackgroundRunner>(
        [this](PeriodicBackgroundRunner::TickPeriod timeLeft) {
            this->onTimerTick(timeLeft);
        },
        [this]() { this->onTimerRunout(); },
        currentDuration(),
        tickInterval);
}

void WorkflowState::toggleZoneMode(Workflow& timer) {}

void WorkflowState::onTimerFinished(Workflow& timer) {}

std::chrono::seconds WorkflowState::duration(const Workflow& timer) const
{
    return std::chrono::minutes{timer.applicationSettings.sprintDuration()};
}

void Idle::enter(Workflow& timer) const
{
    timer.notifyStateChanged(IWorkflow::StateId::IdleEntered);
}

void Idle::cancelExit(Workflow& timer) {}

void Idle::normalExit(Workflow& timer) const
{
    timer.notifyStateChanged(IWorkflow::StateId::IdleLeft);
}

void Idle::setNextState(Workflow& timer)
{
    timer.transitionToState(timer.runningSprint);
}

void RunningSprint::enter(Workflow& timer) const
{
    timer.notifyStateChanged(IWorkflow::StateId::SprintEntered);
    timer.mStart = dw::current_date_time_local();
    timer.startCountdown();
}

void RunningSprint::cancelExit(Workflow& timer)
{
    timer.notifyStateChanged(IWorkflow::StateId::SprintCancelled);
    if (timer.periodicBackgroundRunner)
        timer.periodicBackgroundRunner->stop();
    timer.jumpToState(timer.idle);
}

void RunningSprint::normalExit(Workflow& timer) const
{
    timer.notifyStateChanged(IWorkflow::StateId::SprintLeft);
}

void RunningSprint::setNextState(Workflow& timer)
{
    timer.transitionToState(timer.sprintFinished);
}

std::chrono::seconds RunningSprint::duration(const Workflow& timer) const
{
    return std::chrono::minutes{timer.applicationSettings.sprintDuration()};
}

void RunningSprint::toggleZoneMode(Workflow& timer)
{
    timer.jumpToState(timer.zone);
}

void RunningSprint::onTimerFinished(Workflow& timer)
{
    timer.buffer.emplace_back(
        DateTimeRange{timer.mStart, dw::current_date_time_local()});
    setNextState(timer);
}

void ShortBreak::enter(Workflow& timer) const
{
    timer.notifyStateChanged(IWorkflow::StateId::BreakEntered);
    timer.startCountdown();
}

void ShortBreak::cancelExit(Workflow& timer)
{
    if (timer.periodicBackgroundRunner)
        timer.periodicBackgroundRunner->stop();
    timer.notifyStateChanged(IWorkflow::StateId::BreakCancelled);
    timer.jumpToState(timer.idle);
}

void ShortBreak::normalExit(Workflow& timer) const
{
    timer.notifyStateChanged(IWorkflow::StateId::BreakLeft);
}

void ShortBreak::setNextState(Workflow& timer)
{
    timer.transitionToState(timer.idle);
}

std::chrono::seconds ShortBreak::duration(const Workflow& timer) const
{
    return std::chrono::minutes{timer.applicationSettings.shortBreakDuration()};
}

void ShortBreak::onTimerFinished(Workflow& timer) { setNextState(timer); }

void LongBreak::enter(Workflow& timer) const
{
    timer.notifyStateChanged(IWorkflow::StateId::BreakEntered);
    timer.startCountdown();
}

void LongBreak::cancelExit(Workflow& timer)
{
    if (timer.periodicBackgroundRunner)
        timer.periodicBackgroundRunner->stop();
    timer.notifyStateChanged(IWorkflow::StateId::BreakCancelled);
    timer.jumpToState(timer.idle);
}

void LongBreak::normalExit(Workflow& timer) const
{
    timer.notifyStateChanged(IWorkflow::StateId::BreakLeft);
}

void LongBreak::setNextState(Workflow& timer)
{
    timer.transitionToState(timer.idle);
}

std::chrono::seconds LongBreak::duration(const Workflow& timer) const
{
    return std::chrono::minutes{timer.applicationSettings.longBreakDuration()};
}

void LongBreak::onTimerFinished(Workflow& timer) { setNextState(timer); }

void Zone::enter(Workflow& timer) const
{
    // this check is required due to lame test structure that switches states
    // directly without initializing periodicBackgroundRunner
    if (timer.periodicBackgroundRunner)
        timer.periodicBackgroundRunner->setCyclic(true);
    timer.notifyStateChanged(IWorkflow::StateId::ZoneEntered);
}

void Zone::cancelExit(Workflow& timer) {}

void Zone::normalExit(Workflow& timer) const {}

std::chrono::seconds Zone::duration(const Workflow& timer) const
{
    return std::chrono::minutes{timer.applicationSettings.sprintDuration()};
}

void Zone::setNextState(Workflow& timer) {}

void Zone::toggleZoneMode(Workflow& timer)
{
    // this check is required due to lame test structure that switches states
    // directly without initializing periodicBackgroundRunner
    if (timer.periodicBackgroundRunner)
        timer.periodicBackgroundRunner->setCyclic(false);
    // Changing state directly because we don't want to enter/exit to be called
    timer.currentState = &timer.runningSprint;
    timer.notifyStateChanged(IWorkflow::StateId::ZoneLeft);
}

void Zone::onTimerFinished(Workflow& timer)
{
    timer.buffer.emplace_back(
        DateTimeRange{timer.mStart, dw::current_date_time_local()});
    timer.mStart = dw::current_date_time_local();
}

void SprintFinished::enter(Workflow& timer) const
{
    timer.notifyStateChanged(IWorkflow::StateId::SprintFinished);
}

void SprintFinished::cancelExit(Workflow& timer)
{
    timer.clearSprintsBuffer();
    timer.jumpToState(timer.idle);
}

void SprintFinished::normalExit(Workflow& timer) const {}

void SprintFinished::setNextState(Workflow& timer)
{
    timer.longBreakConditionMet() ? timer.transitionToState(timer.longBreak)
                                  : timer.transitionToState(timer.shortBreak);
}

} // namespace sprint_timer
