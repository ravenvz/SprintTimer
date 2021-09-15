/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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

#include <iostream>

namespace sprint_timer {

using dw::DateTime;
using dw::DateTimeRange;

Workflow::Workflow(std::chrono::seconds tickPeriod_,
                   const IWorkflow::WorkflowParams& params_)
    : currentState{&idle}
    , tickInterval{tickPeriod_}
    , params{params_}
    , mStart{dw::current_date_time_local()}
{
}

void Workflow::start()
{
    if (currentState == &sprintFinished || currentState == &idle)
        currentState->exit(*this);
}

void Workflow::cancel() { currentState->cancel(*this); }

std::chrono::seconds Workflow::currentDuration() const
{
    return currentState->duration(*this);
}

void Workflow::toggleInTheZoneMode() { currentState->toggleZoneMode(*this); }

std::vector<DateTimeRange> Workflow::completedSprints() const { return buffer; }

void Workflow::setNumFinishedSprints(int num) { finishedSprints = num; }

int Workflow::numFinishedSprints() const { return finishedSprints; }

void Workflow::jumpIgnoringExitTo(WorkflowState& state)
{
    currentState = &state;
    currentState->enter(*this);
}

void Workflow::transitionToState(WorkflowState& state)
{
    currentState = &state;
    currentState->enter(*this);
}

bool Workflow::longBreakConditionMet() const
{
    std::cout << "Number of sprints in the buffer: " << buffer.size()
              << std::endl;
    std::cout << "Number of finished sprints as indicated by workflow: "
              << numFinishedSprints() << std::endl;
    return finishedSprints % params.numSprintsBeforeLongBreak == 0;
}

void Workflow::onTimerRunout() { currentState->onTimerFinished(*this); }

void Workflow::notifyStateChanged(IWorkflow::StateId stateId)
{
    for (auto* listener : listeners) {
        listener->onWorkflowStateChanged(stateId);
    }
}

void Workflow::onTimerTick(PeriodicBackgroundRunner::TickPeriod timeLeft)
{
    using namespace std::chrono;
    for (auto* listener : listeners) {
        listener->onTimerTick(duration_cast<seconds>(timeLeft));
    }
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

void Workflow::addListener(WorkflowListener* listener)
{
    listeners.insert(listener);
}

void Workflow::removeListener(WorkflowListener* listener)
{
    listeners.erase(listener);
}

void Workflow::reconfigure(const WorkflowParams& params_) { params = params_; }

void WorkflowState::cancel(Workflow& /*workflow*/) { }

void WorkflowState::toggleZoneMode(Workflow& /*timer*/) { }

void WorkflowState::onTimerFinished(Workflow& /*timer*/) { }

std::chrono::seconds WorkflowState::duration(const Workflow& /*timer*/) const
{
    return std::chrono::seconds{0};
}

void Idle::enter(Workflow& timer) const
{
    timer.notifyStateChanged(IWorkflow::StateId::Idle);
}

void Idle::exit(Workflow& timer)
{
    timer.transitionToState(timer.runningSprint);
}

void RunningSprint::enter(Workflow& timer) const
{
    timer.notifyStateChanged(IWorkflow::StateId::RunningSprint);
    timer.mStart = dw::current_date_time_local();
    timer.startCountdown();
}

void RunningSprint::cancel(Workflow& timer)
{
    if (timer.periodicBackgroundRunner) {
        timer.periodicBackgroundRunner->stop();
    }
    timer.buffer.clear();
    timer.jumpIgnoringExitTo(timer.idle);
}

void RunningSprint::exit(Workflow& timer)
{
    timer.transitionToState(timer.sprintFinished);
}

std::chrono::seconds RunningSprint::duration(const Workflow& timer) const
{
    return timer.params.sprintDuration;
}

void RunningSprint::toggleZoneMode(Workflow& timer)
{
    timer.jumpIgnoringExitTo(timer.zone);
}

void RunningSprint::onTimerFinished(Workflow& timer)
{
    timer.buffer.emplace_back(
        DateTimeRange{timer.mStart, dw::current_date_time_local()});
    exit(timer);
}

void ShortBreak::enter(Workflow& timer) const
{
    timer.notifyStateChanged(IWorkflow::StateId::BreakStarted);
    timer.startCountdown();
}

void ShortBreak::cancel(Workflow& timer)
{
    if (timer.periodicBackgroundRunner) {
        timer.periodicBackgroundRunner->stop();
    }
    timer.jumpIgnoringExitTo(timer.idle);
}

void ShortBreak::exit(Workflow& timer)
{
    timer.transitionToState(timer.idle);
    timer.notifyStateChanged(IWorkflow::StateId::BreakFinished);
}

std::chrono::seconds ShortBreak::duration(const Workflow& timer) const
{
    return timer.params.shortBreakDuration;
}

void ShortBreak::onTimerFinished(Workflow& timer) { exit(timer); }

void LongBreak::enter(Workflow& timer) const
{
    std::cout << "Entering the long break" << std::endl;
    std::cout << "Number of sprints in the buffer: " << timer.buffer.size()
              << std::endl;
    std::cout << "Number of finished sprints as indicated by workflow: "
              << timer.numFinishedSprints() << std::endl;
    timer.notifyStateChanged(IWorkflow::StateId::BreakStarted);
    timer.startCountdown();
}

void LongBreak::cancel(Workflow& timer)
{
    if (timer.periodicBackgroundRunner) {
        timer.periodicBackgroundRunner->stop();
    }
    timer.jumpIgnoringExitTo(timer.idle);
}

void LongBreak::exit(Workflow& timer)
{
    timer.transitionToState(timer.idle);
    timer.notifyStateChanged(IWorkflow::StateId::BreakFinished);
}

std::chrono::seconds LongBreak::duration(const Workflow& timer) const
{
    return timer.params.longBreakDuration;
}

void LongBreak::onTimerFinished(Workflow& timer) { exit(timer); }

void Zone::enter(Workflow& timer) const
{
    // this check is required due to lame test structure that switches states
    // directly without initializing periodicBackgroundRunner
    if (timer.periodicBackgroundRunner) {
        timer.periodicBackgroundRunner->setCyclic(true);
    }
    timer.notifyStateChanged(IWorkflow::StateId::ZoneEntered);
}

std::chrono::seconds Zone::duration(const Workflow& timer) const
{
    return timer.params.sprintDuration;
}

void Zone::exit(Workflow& /*timer*/) { }

void Zone::toggleZoneMode(Workflow& timer)
{
    // this check is required due to lame test structure that switches states
    // directly without initializing periodicBackgroundRunner
    if (timer.periodicBackgroundRunner) {
        timer.periodicBackgroundRunner->setCyclic(false);
    }
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

void SprintFinished::cancel(Workflow& timer)
{
    timer.buffer.clear();
    timer.jumpIgnoringExitTo(timer.idle);
}

void SprintFinished::exit(Workflow& timer)
{
    timer.setNumFinishedSprints(timer.numFinishedSprints() +
                                static_cast<int>(timer.buffer.size()));
    timer.buffer.clear();
    timer.longBreakConditionMet() ? timer.transitionToState(timer.longBreak)
                                  : timer.transitionToState(timer.shortBreak);
}

} // namespace sprint_timer
