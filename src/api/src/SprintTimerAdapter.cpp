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
#include "api/SprintTimerAdapter.h"
#include "api/WorkflowService.h"
#include <ranges>

namespace {

auto not_null = [](auto* listener) { return listener != nullptr; };

auto to_date_range = [](const auto& sprint) { return sprint.timeSpan(); };

} // namespace

namespace sprint_timer::api {

using std::views::filter;

SprintTimerAdapter::SprintTimerAdapter(SprintTimer& sprintTimer_)
    : sprintTimer{sprintTimer_}
{
    sprintTimer.addListener(this);
}

SprintTimerAdapter::~SprintTimerAdapter() { sprintTimer.removeListener(this); }

auto SprintTimerAdapter::addListener(TimerWorkflowListener* listener) -> void
{
    listeners.insert(listener);
}

auto SprintTimerAdapter::removeListener(TimerWorkflowListener* listener) -> void
{
    listeners.erase(listener);
}

auto SprintTimerAdapter::onTimerTick(std::chrono::seconds timeLeft) -> void
{
    for (auto* listener : filter(listeners, not_null)) {
        listener->onTimerTick(timeLeft);
    }
}

auto SprintTimerAdapter::onIdle() -> void
{
    for (auto* listener : filter(listeners, not_null)) {
        listener->onIdle();
    }
}

auto SprintTimerAdapter::onSprintStarted(std::chrono::seconds timeLeft) -> void
{
    for (auto* listener : filter(listeners, not_null)) {
        listener->onSprintStarted(timeLeft);
    }
}

auto SprintTimerAdapter::onSprintFinished(std::vector<Sprint>&& sprints) -> void
{
    for (auto* listener : filter(listeners, not_null)) {
        listener->onSprintFinished(
            sprints | std::views::transform(to_date_range) |
            std::ranges::to<std::vector<dw::DateTimeRange>>());
    }
}

auto SprintTimerAdapter::onBreakStarted(std::chrono::seconds timeLeft) -> void
{
    for (auto* listener : filter(listeners, not_null)) {
        listener->onBreakStarted(timeLeft);
    }
}

auto SprintTimerAdapter::onBreakFinished() -> void
{

    for (auto* listener : filter(listeners, not_null)) {
        listener->onBreakFinished();
    }
}

auto SprintTimerAdapter::onZoneEntered() -> void
{
    for (auto* listener : filter(listeners, not_null)) {
        listener->onZoneEntered();
    }
}

auto SprintTimerAdapter::onZoneLeft() -> void
{
    for (auto* listener : filter(listeners, not_null)) {
        listener->onZoneLeft();
    }
}

} // namespace sprint_timer::api
