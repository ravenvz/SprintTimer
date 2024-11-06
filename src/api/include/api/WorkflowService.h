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
#ifndef WORKFLOWSERVICE_H_ATCQE3VY
#define WORKFLOWSERVICE_H_ATCQE3VY

#include "core/Sprint.h"
#include <vector>

namespace sprint_timer::api {

template <typename EventT> class EventHandler {
public:
    virtual ~EventHandler() = default;

    virtual auto handle(EventT event) -> void = 0;
};

template <typename EventT>
class CombinedEventHandler : public EventHandler<EventT> {
public:
    explicit CombinedEventHandler(std::vector<EventHandler<EventT>*> handlers_)
        : handlers{std::move(handlers_)}
    {
    }

    auto handle(EventT event) -> void override
    {
        for (auto* handler : handlers) {
            handler->handle(event);
        }
    }

private:
    std::vector<EventHandler<EventT>*> handlers;
};

class TimerWorkflowListener {
public:
    virtual ~TimerWorkflowListener() = default;

    virtual auto onInit() -> void { }

    virtual auto onIdle() -> void { }

    virtual auto onSprintStarted(std::chrono::seconds timeLeft) -> void { }

    virtual auto
    onSprintFinished(std::vector<dw::DateTimeRange>&& sprints) -> void
    {
    }

    virtual auto onBreakStarted(std::chrono::seconds timeLeft) -> void { }

    virtual auto onBreakFinished() -> void { }

    virtual auto onZoneEntered() -> void { }

    virtual auto onZoneLeft() -> void { }

    virtual auto onTimerTick(std::chrono::seconds timeLeft) -> void { }
};


} // namespace sprint_timer::api

#endif /* end of include guard: WORKFLOWSERVICE_H_ATCQE3VY */
