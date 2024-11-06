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

namespace sprint_timer {

namespace replacement {

auto WorkflowFSM::startImpl() -> void { fsm.process(StartEvent{}); }

auto WorkflowFSM::cancelImpl() -> void { }

auto WorkflowFSM::toggleInTheZoneModeImpl() -> void { }

} // namespace replacement

using dw::DateTime;
using dw::DateTimeRange;

WorkflowOldFsm::WorkflowOldFsm(CountdownTimer& timer_,
                               std::chrono::milliseconds tickInterval_,
                               TimerWorkflow::WorkflowParams params_)
    : TimerWorkflow{timer_, tickInterval_, params_}
    , currentState{&idle}
{
}

void WorkflowOldFsm::startImpl()
{
    if (currentState == &sprintFinished || currentState == &idle) {
        currentState->exit(*this);
    }
}

void WorkflowOldFsm::cancelImpl() { currentState->cancel(*this); }

void WorkflowOldFsm::toggleInTheZoneModeImpl()
{
    currentState->toggleZoneMode(*this);
}

void WorkflowOldFsm::jumpIgnoringExitTo(WorkflowState& state)
{
    currentState = &state;
    currentState->enter(*this);
}

void WorkflowOldFsm::transitionToState(WorkflowState& state)
{
    currentState = &state;
    currentState->enter(*this);
}

void WorkflowState::cancel(WorkflowOldFsm& /*workflow*/) { }

void WorkflowState::toggleZoneMode(WorkflowOldFsm& /*timer*/) { }

void WorkflowState::onTimerFinished(WorkflowOldFsm& /*timer*/) { }

std::chrono::seconds
WorkflowState::duration(const WorkflowOldFsm& /*timer*/) const
{
    return std::chrono::seconds{0};
}

void Idle::enter(WorkflowOldFsm& timer) const
{
    timer.notifyStateChanged(TimerWorkflow::StateId::Idle);
}

void Idle::exit(WorkflowOldFsm& timer)
{
    timer.transitionToState(timer.runningSprint);
}

void RunningSprint::enter(WorkflowOldFsm& timer) const
{
    timer.notifyStateChanged(TimerWorkflow::StateId::RunningSprint);
    timer.timerStartedAt = dw::current_date_time_local();
    timer.startCountdown();
}

void RunningSprint::cancel(WorkflowOldFsm& timer)
{
    timer.timer.stop();
    timer.buffer.clear();
    timer.jumpIgnoringExitTo(timer.idle);
}

void RunningSprint::exit(WorkflowOldFsm& timer)
{
    timer.transitionToState(timer.sprintFinished);
}

std::chrono::seconds RunningSprint::duration(const WorkflowOldFsm& timer) const
{
    return timer.params.sprintDuration;
}

void RunningSprint::toggleZoneMode(WorkflowOldFsm& timer)
{
    timer.jumpIgnoringExitTo(timer.zone);
}

void RunningSprint::onTimerFinished(WorkflowOldFsm& timer)
{
    timer.buffer.emplace_back(timer.timerStartedAt,
                              dw::current_date_time_local());
    exit(timer);
}

void ShortBreak::enter(WorkflowOldFsm& timer) const
{
    timer.notifyStateChanged(TimerWorkflow::StateId::BreakStarted);
    timer.startCountdown();
}

void ShortBreak::cancel(WorkflowOldFsm& timer)
{
    timer.timer.stop();
    timer.jumpIgnoringExitTo(timer.idle);
}

void ShortBreak::exit(WorkflowOldFsm& timer)
{
    timer.transitionToState(timer.idle);
    timer.notifyStateChanged(TimerWorkflow::StateId::BreakFinished);
}

std::chrono::seconds ShortBreak::duration(const WorkflowOldFsm& timer) const
{
    return timer.params.shortBreakDuration;
}

void ShortBreak::onTimerFinished(WorkflowOldFsm& timer) { exit(timer); }

void LongBreak::enter(WorkflowOldFsm& timer) const
{
    timer.notifyStateChanged(TimerWorkflow::StateId::BreakStarted);
    timer.startCountdown();
}

void LongBreak::cancel(WorkflowOldFsm& timer)
{
    timer.timer.stop();
    timer.jumpIgnoringExitTo(timer.idle);
}

void LongBreak::exit(WorkflowOldFsm& timer)
{
    timer.transitionToState(timer.idle);
    timer.notifyStateChanged(TimerWorkflow::StateId::BreakFinished);
}

std::chrono::seconds LongBreak::duration(const WorkflowOldFsm& timer) const
{
    return timer.params.longBreakDuration;
}

void LongBreak::onTimerFinished(WorkflowOldFsm& timer) { exit(timer); }

void Zone::enter(WorkflowOldFsm& timer) const
{
    // this check is required due to lame test structure that switches states
    // directly without initializing periodicBackgroundRunner
    timer.timer.setLoop(true);
    timer.notifyStateChanged(TimerWorkflow::StateId::ZoneEntered);
}

std::chrono::seconds Zone::duration(const WorkflowOldFsm& timer) const
{
    return timer.params.sprintDuration;
}

void Zone::exit(WorkflowOldFsm& /*timer*/) { }

void Zone::toggleZoneMode(WorkflowOldFsm& timer)
{
    // this check is required due to lame test structure that switches states
    // directly without initializing periodicBackgroundRunner
    timer.timer.setLoop(false);
    // Changing state directly because we don't want to enter/exit to be called
    timer.currentState = &timer.runningSprint;
    timer.notifyStateChanged(TimerWorkflow::StateId::ZoneLeft);
}

void Zone::onTimerFinished(WorkflowOldFsm& timer)
{
    timer.buffer.emplace_back(timer.timerStartedAt,
                              dw::current_date_time_local());
    timer.timerStartedAt = dw::current_date_time_local();
}

void SprintFinished::enter(WorkflowOldFsm& timer) const
{
    timer.notifyStateChanged(TimerWorkflow::StateId::SprintFinished);
}

void SprintFinished::cancel(WorkflowOldFsm& timer)
{
    timer.buffer.clear();
    timer.jumpIgnoringExitTo(timer.idle);
}

void SprintFinished::exit(WorkflowOldFsm& timer)
{
    timer.setNumFinishedSprints(timer.numFinishedSprints() +
                                static_cast<int>(timer.buffer.size()));
    timer.buffer.clear();
    timer.longBreakConditionMet() ? timer.transitionToState(timer.longBreak)
                                  : timer.transitionToState(timer.shortBreak);
}

} // namespace sprint_timer
