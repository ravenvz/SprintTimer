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
#ifndef SPRINTTIMERADAPTER_H_ON45YSRP
#define SPRINTTIMERADAPTER_H_ON45YSRP

#include "core/SprintTimer.h"
#include <unordered_set>

namespace sprint_timer::api {

class TimerWorkflowListener;

class SprintTimerAdapter : SprintTimer::WorkflowListener {
public:
    explicit SprintTimerAdapter(SprintTimer& sprintTimer_);

    ~SprintTimerAdapter();

    SprintTimerAdapter(const SprintTimerAdapter&) = delete;

    SprintTimerAdapter(SprintTimerAdapter&&) = delete;

    auto operator=(const SprintTimerAdapter&) = delete;

    auto operator=(SprintTimerAdapter&&) = delete;

    auto addListener(TimerWorkflowListener* listener) -> void;

    auto removeListener(TimerWorkflowListener* listener) -> void;

private:
    SprintTimer& sprintTimer;
    std::unordered_set<TimerWorkflowListener*> listeners;

    auto onTimerTick(std::chrono::seconds timeLeft) -> void override;

    auto onIdle() -> void override;

    auto onSprintStarted(std::chrono::seconds timeLeft) -> void override;

    auto onSprintFinished(std::vector<Sprint>&& sprints) -> void override;

    auto onBreakStarted(std::chrono::seconds timeLeft) -> void override;

    auto onBreakFinished() -> void override;

    auto onZoneEntered() -> void override;

    auto onZoneLeft() -> void override;
};

} // namespace sprint_timer::api

#endif /* end of include guard: SPRINTTIMERADAPTER_H_ON45YSRP */
