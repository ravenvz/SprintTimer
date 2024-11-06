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
#ifndef TIMERWORKFLOW_H_1POUTXG7
#define TIMERWORKFLOW_H_1POUTXG7

#include "core/PeriodicBackgroundRunner.h"
#include "core/Sprint.h"
#include "cpp_utils/patterns/State.h"
#include "cpp_utils/types/NamedType.h"
#include "date_wrapper/date_wrapper.h"
#include <functional>
#include <optional>
#include <unordered_set>
#include <utility>
#include <vector>

#include <iostream>

namespace sprint_timer {

class CountdownTimer {
public:
    struct TickPeriodTag { };
    struct TimerDurationTag { };

    using TickPeriod =
        types::ImplicitNamedType<std::chrono::milliseconds, TickPeriodTag>;
    using TimerDuration =
        types::ImplicitNamedType<std::chrono::milliseconds, TimerDurationTag>;

    using OnTick = std::function<void(std::chrono::milliseconds)>;
    using OnRunout = std::function<void(void)>;

    virtual ~CountdownTimer() = default;

    auto run(TimerDuration duration,
             TickPeriod tickPeriod,
             OnTick onTick,
             OnRunout onRunout) -> void
    {
        runImpl(duration, tickPeriod, std::move(onTick), std::move(onRunout));
    }

    auto stop() -> void { stopImpl(); }

    auto setLoop(bool cyclic) -> void { setLoopImpl(cyclic); }

private:
    virtual auto runImpl(TimerDuration duration,
                         TickPeriod tickPeriod,
                         OnTick onTick,
                         OnRunout onRunout) -> void = 0;

    virtual auto stopImpl() -> void = 0;

    virtual auto setLoopImpl(bool cyclic) -> void = 0;
};

class BackgroundCountdownTimer : public CountdownTimer {
private:
    auto runImpl(TimerDuration duration,
                 TickPeriod tickPeriod,
                 OnTick onTick,
                 OnRunout onRunout) -> void override
    {
        runner = std::make_unique<PeriodicBackgroundRunner>(
            onTick, onRunout, duration, tickPeriod);
    }

    auto stopImpl() -> void override
    {
        if (runner) {
            runner->stop();
        }
    }

    auto setLoopImpl(bool cyclic) -> void override
    {
        if (runner) {
            runner->setCyclic(cyclic);
        }
    }

    std::unique_ptr<PeriodicBackgroundRunner> runner;
};

class SprintTimer {
public:
    enum class StateId {
        Idle,
        RunningSprint,
        SprintFinished,
        BreakStarted,
        BreakFinished,
        ZoneEntered,
        ZoneLeft
    };

    struct WorkflowParams {
        std::chrono::seconds sprintDuration;
        std::chrono::seconds shortBreakDuration;
        std::chrono::seconds longBreakDuration;
        int32_t numSprintsBeforeLongBreak;
    };

    struct StartEvent {
        dw::DateTime currentTime;
    };

    struct CancelEvent { };

    struct TimerFinishedEvent { };

    struct ZoneToggledEvent { };

    struct SubmitEvent { };

    class WorkflowListener {
    public:
        virtual ~WorkflowListener() = default;

        virtual auto onTimerTick(std::chrono::seconds timeLeft) -> void { }

        virtual auto onIdle() -> void { }

        virtual auto onSprintStarted(std::chrono::seconds timeLeft) -> void { }

        virtual auto onSprintFinished(std::vector<Sprint>&& sprints) -> void { }

        virtual auto onBreakStarted(std::chrono::seconds timeLeft) -> void { }

        virtual auto onBreakFinished() -> void { }

        virtual auto onZoneEntered() -> void { }

        virtual auto onZoneLeft() -> void { }
    };

    SprintTimer(CountdownTimer& timer_,
                std::chrono::milliseconds tickInterval_,
                WorkflowParams params_);

    virtual ~SprintTimer() = default;

    template <typename EventT> auto process(EventT event) -> SprintTimer&
    {
        fsm.process(std::move(event));
        return *this;
    }

    [[nodiscard]] auto currentDuration() const -> std::chrono::seconds;

    auto setNumFinishedSprints(int num) -> void;

    auto addListener(WorkflowListener* listener) -> void;

    auto removeListener(WorkflowListener* listener) -> void;

    auto reconfigure(WorkflowParams params) -> void;

private:
    class BaseState;
    class Idle;
    class RunningSprint;
    class Break;
    class SprintFinished;
    class Zone;

    friend class BaseState;
    friend class Idle;
    friend class RunningSprint;
    friend class Break;
    friend class SprintFinished;
    friend class Zone;

    using State =
        std::variant<Idle, RunningSprint, Break, SprintFinished, Zone>;

    using MaybeState = std::optional<State>;

    class BaseState {
    protected:
        explicit BaseState(std::reference_wrapper<SprintTimer> context_)
            : context{context_}
        {
        }

        template <typename EventT> auto process(EventT /*event*/) -> MaybeState
        {
            return MaybeState{};
        }

        std::reference_wrapper<SprintTimer> context;
    };

    class Idle : BaseState {
    public:
        using BaseState::context;
        using BaseState::process;

        explicit Idle(std::reference_wrapper<SprintTimer> ctx_)
            : BaseState{ctx_}
        {
            context.get().notifyStateChanged(
                [](auto* listener) { listener->onIdle(); });
        }

        auto process(StartEvent event) -> MaybeState
        {
            return RunningSprint{context, event.currentTime};
        }
    };

    class RunningSprint : BaseState {
    public:
        using BaseState::context;
        using BaseState::process;

        // Creates new instance but doesn't notify listener. Useful when
        // returning from the Zone mode.
        static auto
        backFromZone(std::reference_wrapper<SprintTimer> ctx_) -> RunningSprint
        {
            return RunningSprint{ctx_};
        }

        RunningSprint(std::reference_wrapper<SprintTimer> ctx_,
                      dw::DateTime currentTime_)
            : BaseState{ctx_}
        {
            auto& workflow = context.get();
            workflow.notifyStateChanged([&](auto* listener) {
                listener->onSprintStarted(workflow.params.sprintDuration);
            });
            workflow.timerStartedAt = currentTime_;
            workflow.startCountdown(workflow.params.sprintDuration);
        }

        auto process(TimerFinishedEvent /*unused*/) -> MaybeState
        {
            auto& workflow = context.get();
            workflow.buffer.emplace_back(
                workflow.timerStartedAt +
                    static_cast<int>(workflow.buffer.size()) *
                        workflow.params.sprintDuration,
                std::chrono::duration_cast<std::chrono::minutes>(
                    workflow.params.sprintDuration));
            return SprintFinished{context};
        }

        auto process(ZoneToggledEvent /*unused*/) -> MaybeState
        {
            return Zone{context};
        }

        auto process(CancelEvent /*unused*/) -> MaybeState
        {
            context.get().timer.stop();
            return Idle{context};
        }

    private:
        explicit RunningSprint(std::reference_wrapper<SprintTimer> ctx_)
            : BaseState{ctx_}
        {
        }
    };

    class Break : BaseState {
    public:
        using BaseState::context;
        using BaseState::process;

        Break(std::reference_wrapper<SprintTimer> ctx_,
              std::chrono::seconds duration)
            : BaseState{ctx_}
        {
            auto& workflow = context.get();
            workflow.notifyStateChanged([&duration](auto* listener) {
                listener->onBreakStarted(duration);
            });
            workflow.startCountdown(duration);
        }

        auto process(TimerFinishedEvent /*unused*/) -> MaybeState
        {
            return Idle{context};
        }

        auto process(CancelEvent /*unused*/) -> MaybeState
        {
            context.get().timer.stop();
            return Idle{context};
        }
    };

    class SprintFinished : BaseState {
    public:
        using BaseState::context;
        using BaseState::process;

        explicit SprintFinished(std::reference_wrapper<SprintTimer> ctx_)
            : BaseState{ctx_}
        {
            auto& workflow = context.get();
            numPendingSprints += static_cast<int>(std::ssize(workflow.buffer));
            workflow.finishedSprints += numPendingSprints;
            workflow.notifyStateChanged([&](auto* listener) {
                listener->onSprintFinished(std::move(workflow.buffer));
            });
            workflow.buffer = std::vector<Sprint>{};
        }

        auto process(StartEvent /*unused*/) -> MaybeState
        {
            auto& workflow = context.get();
            return Break{context,
                         workflow.longBreakConditionMet()
                             ? workflow.params.longBreakDuration
                             : workflow.params.shortBreakDuration};
        }

        auto process(CancelEvent /*unused*/) -> MaybeState
        {
            context.get().finishedSprints -= numPendingSprints;
            return Idle{context};
        }

    private:
        int numPendingSprints{0};
    };

    class Zone : BaseState {
    public:
        using BaseState::context;
        using BaseState::process;

        explicit Zone(std::reference_wrapper<SprintTimer> ctx_)
            : BaseState{ctx_}
        {
            context.get().timer.setLoop(true);
            context.get().notifyStateChanged(
                [](auto* listener) { listener->onZoneEntered(); });
        }

        auto process(TimerFinishedEvent /*event*/) -> MaybeState
        {
            auto& workflow = context.get();
            workflow.buffer.emplace_back(
                workflow.timerStartedAt +
                    static_cast<int>(workflow.buffer.size()) *
                        workflow.params.sprintDuration,
                std::chrono::duration_cast<std::chrono::minutes>(
                    workflow.params.sprintDuration));
            return Zone{*this};
        }

        auto process(ZoneToggledEvent /*unused*/) -> MaybeState
        {
            context.get().notifyStateChanged(
                [](auto* listener) { listener->onZoneLeft(); });
            context.get().timer.setLoop(false);
            return RunningSprint::backFromZone(context);
        }
    };

    CountdownTimer& timer;
    std::chrono::milliseconds tickInterval;
    WorkflowParams params;
    std::unordered_set<WorkflowListener*> listeners;
    dw::DateTime timerStartedAt{dw::current_date_time()};
    int finishedSprints{0};
    std::vector<Sprint> buffer;
    patterns::
        FSMStateTransitions<Idle, RunningSprint, Break, SprintFinished, Zone>
            fsm;

    auto startCountdown(std::chrono::seconds duration) -> void;

    auto onTimerTick(std::chrono::milliseconds timeLeft) -> void;

    auto longBreakConditionMet() const -> bool;

    template <typename CallbackT>
    auto notifyStateChanged(CallbackT callback) -> void
    {
        for (auto* listener : listeners) {
            if (listener != nullptr) {
                callback(listener);
            }
        }
    }
};

} // namespace sprint_timer

#endif /* end of include guard: TIMERWORKFLOW_H_1POUTXG7 */

