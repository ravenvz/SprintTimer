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
#include "core/SprintTimer.h"

namespace sprint_timer {

SprintTimer::SprintTimer(CountdownTimer& timer_,
                         std::chrono::milliseconds tickInterval_,
                         SprintTimer::WorkflowParams params_)
    : timer{timer_}
    , tickInterval{tickInterval_}
    , params{params_}
    , listeners{std::unordered_set<WorkflowListener*>{}}
    , fsm{Idle{std::ref(*this)}}
{
}

auto SprintTimer::setNumFinishedSprints(int num) -> void
{
    finishedSprints = num;
}

auto SprintTimer::addListener(WorkflowListener* listener) -> void
{
    listeners.insert(listener);
}

auto SprintTimer::removeListener(WorkflowListener* listener) -> void
{
    listeners.erase(listener);
}

auto SprintTimer::reconfigure(WorkflowParams params_) -> void
{
    params = params_;
}

auto SprintTimer::startCountdown(std::chrono::seconds duration) -> void
{
    timer.run(
        CountdownTimer::TimerDuration{duration},
        CountdownTimer::TickPeriod{tickInterval},
        [this](std::chrono::milliseconds timeLeft) { onTimerTick(timeLeft); },
        [this]() { process(TimerFinishedEvent{}); });
}

auto SprintTimer::onTimerTick(std::chrono::milliseconds timeLeft) -> void { }

auto SprintTimer::longBreakConditionMet() const -> bool
{
    return finishedSprints % params.numSprintsBeforeLongBreak == 0;
}

} // namespace sprint_timer
