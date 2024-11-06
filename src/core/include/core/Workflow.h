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
#ifndef WORKFLOW_H_QKH9ETW3
#define WORKFLOW_H_QKH9ETW3

#include "core/PeriodicBackgroundRunner.h"
#include "core/TimerWorkflow.h"
#include "cpp_utils/patterns/State.h"
#include "cpp_utils/types/NamedType.h"
#include "date_wrapper/date_wrapper.h"
#include <chrono>
#include <memory>
#include <optional>
#include <thread>
#include <unordered_set>
#include <variant>
#include <vector>

namespace sprint_timer {

namespace replacement {

// template <typename TickCallback, typename RunoutCallback> class
// CountdownTimer { public:
//     struct TickPeriodTag { };
//     struct TimerDurationTag { };
//     using TickPeriod =
//         types::ImplicitNamedType<std::chrono::milliseconds, TickPeriodTag>;
//     using TimerDuration =
//         types::ImplicitNamedType<std::chrono::milliseconds,
//         TimerDurationTag>;
//
//     auto run(TimerDuration duration,
//              TickPeriod tickPeriod,
//              TickCallback onTick,
//              RunoutCallback onRunout) -> void
//     {
//         runner = std::make_unique<PeriodicBackgroundRunner>(
//             onTick, onRunout, duration, tickPeriod);
//     }
//
//     auto stop() -> void
//     {
//         if (runner) {
//             runner->stop();
//         }
//     }
//
// private:
//     std::unique_ptr<PeriodicBackgroundRunner> runner;
// };

class WorkflowFSM;

class WorkflowFSM : public TimerWorkflow {
public:
    WorkflowFSM(CountdownTimer& timer_,
                std::chrono::milliseconds tickInterval_,
                TimerWorkflow::WorkflowParams params_)
        : TimerWorkflow{timer_, tickInterval_, params_}
        , fsm{TransitionTable{}, Idle{std::ref(*this)}}
    {
    }

private:
    friend class BaseState;
    friend class Idle;
    friend class RunningSprint;
    friend class ShortBreak;
    friend class Break;
    friend class SprintFinished;

    class BaseState {
    protected:
        explicit BaseState(std::reference_wrapper<WorkflowFSM> context_)
            : context{context_}
        {
        }

        std::reference_wrapper<WorkflowFSM> context;
    };

    class Idle : BaseState {
    public:
        using BaseState::context;

        explicit Idle(std::reference_wrapper<WorkflowFSM> ctx_)
            : BaseState{ctx_}
        {
        }
    };

    class RunningSprint : BaseState {
    public:
        using BaseState::context;

        explicit RunningSprint(std::reference_wrapper<WorkflowFSM> ctx_)
            : BaseState{ctx_}
        {
            context.get().notifyStateChanged(
                TimerWorkflow::StateId::RunningSprint);
            context.get().timerStartedAt = dw::current_date_time_local();
            context.get().startCountdown();
        }
    };

    class ShortBreak : BaseState {
    public:
        using BaseState::context;

        explicit ShortBreak(std::reference_wrapper<WorkflowFSM> ctx_)
            : BaseState{ctx_}
        {
        }
    };

    class LongBreak : BaseState {
    public:
        using BaseState::context;

        explicit LongBreak(std::reference_wrapper<WorkflowFSM> ctx_)
            : BaseState{ctx_}
        {
        }
    };

    class Zone : BaseState {
    public:
        using BaseState::context;

        explicit Zone(std::reference_wrapper<WorkflowFSM> ctx_)
            : BaseState{ctx_}
        {
        }
    };

    class SprintFinished : BaseState {
    public:
        using BaseState::context;

        explicit SprintFinished(std::reference_wrapper<WorkflowFSM> ctx_)
            : BaseState{ctx_}
        {
            context.get().notifyStateChanged(
                TimerWorkflow::StateId::SprintFinished);
        }
    };

    using State = std::variant<Idle,
                               RunningSprint,
                               ShortBreak,
                               LongBreak,
                               Zone,
                               SprintFinished>;

    using MaybeState = std::optional<State>;

    struct StartEvent { };

    struct CancellationEvent { };

    struct TimerFinishedEvent { };

    struct ZoneToggledEvent { };

    class TransitionTable {
    public:
        auto operator()(Idle& state, StartEvent /*unused*/) -> MaybeState
        {
            return RunningSprint{state.context};
        }

        auto operator()(RunningSprint& state, TimerFinishedEvent) -> MaybeState
        {
            return SprintFinished{state.context};
        }

        template <typename StateT, typename EventT>
        auto operator()(StateT& /*unused*/,
                        EventT /*unused*/) const -> MaybeState
        {
            return MaybeState{};
        }
    };

    auto notifyStateChanged(TimerWorkflow::StateId stateId) -> void;

    patterns::FSMExternalTransitions<TransitionTable,
                                     Idle,
                                     RunningSprint,
                                     ShortBreak,
                                     LongBreak,
                                     Zone,
                                     SprintFinished>
        fsm;

    auto startImpl() -> void override;

    auto cancelImpl() -> void override;

    auto toggleInTheZoneModeImpl() -> void override;

    auto onTimerRunoutImpl() -> void override;
};

} // namespace replacement

class WorkflowOldFsm;

class WorkflowState {
public:
    virtual ~WorkflowState() = default;
    virtual void enter(WorkflowOldFsm& workflow) const = 0;
    virtual void cancel(WorkflowOldFsm& workflow);
    virtual void exit(WorkflowOldFsm& workflow) = 0;
    virtual void toggleZoneMode(WorkflowOldFsm& workflow);
    virtual std::chrono::seconds duration(const WorkflowOldFsm& workflow) const;
    virtual void onTimerFinished(WorkflowOldFsm& workflow);
};

class Idle final : public WorkflowState {
public:
    void enter(WorkflowOldFsm& workflow) const override;
    void exit(WorkflowOldFsm& workflow) override;
};

class RunningSprint final : public WorkflowState {
public:
    void enter(WorkflowOldFsm& workflow) const override;
    void cancel(WorkflowOldFsm& workflow) override;
    void exit(WorkflowOldFsm& workflow) override;
    std::chrono::seconds
    duration(const WorkflowOldFsm& workflow) const override;
    void toggleZoneMode(WorkflowOldFsm& workflow) override;
    void onTimerFinished(WorkflowOldFsm& workflow) override;
};

class ShortBreak final : public WorkflowState {
public:
    void enter(WorkflowOldFsm& workflow) const override;
    void cancel(WorkflowOldFsm& workflow) override;
    void exit(WorkflowOldFsm& workflow) override;
    std::chrono::seconds
    duration(const WorkflowOldFsm& workflow) const override;
    void onTimerFinished(WorkflowOldFsm& workflow) override;
};

class LongBreak final : public WorkflowState {
public:
    void enter(WorkflowOldFsm& workflow) const override;
    void cancel(WorkflowOldFsm& workflow) override;
    void exit(WorkflowOldFsm& workflow) override;
    std::chrono::seconds
    duration(const WorkflowOldFsm& workflow) const override;
    void onTimerFinished(WorkflowOldFsm& workflow) override;
};

class Zone final : public WorkflowState {
public:
    void enter(WorkflowOldFsm& workflow) const override;
    void exit(WorkflowOldFsm& workflow) override;
    std::chrono::seconds
    duration(const WorkflowOldFsm& workflow) const override;
    void toggleZoneMode(WorkflowOldFsm& workflow) override;
    void onTimerFinished(WorkflowOldFsm& workflow) override;
};

class SprintFinished final : public WorkflowState {
public:
    void enter(WorkflowOldFsm& workflow) const override;
    void cancel(WorkflowOldFsm& workflow) override;
    void exit(WorkflowOldFsm& workflow) override;
};

class WorkflowOldFsm : public TimerWorkflow {
    friend class WorkflowState;
    friend class ShortBreak;
    friend class LongBreak;
    friend class RunningSprint;
    friend class Zone;
    friend class Idle;
    friend class SprintFinished;

public:
    WorkflowOldFsm(CountdownTimer& timer_,
                   std::chrono::milliseconds tickInterval_,
                   TimerWorkflow::WorkflowParams params_);

    ~WorkflowOldFsm() override = default;

    WorkflowOldFsm(WorkflowOldFsm&&) = delete;
    auto operator=(WorkflowOldFsm&&) -> WorkflowOldFsm& = delete;

    WorkflowOldFsm(const WorkflowOldFsm&) = delete;
    auto operator=(const WorkflowOldFsm&) -> WorkflowOldFsm& = delete;

    auto numFinishedSprints() const -> int;

protected:
    Idle idle;
    RunningSprint runningSprint;
    ShortBreak shortBreak;
    LongBreak longBreak;
    Zone zone;
    SprintFinished sprintFinished;
    WorkflowState* currentState;

    /* Transition to another state while exiting current state properly. */
    void transitionToState(WorkflowState& state);

    void notifyStateChanged(TimerWorkflow::StateId stateId);

private:
    /* Jump to another state immediately ignoring proper exit from current state
     */
    auto startImpl() -> void override;

    auto cancelImpl() -> void override;

    auto toggleInTheZoneModeImpl() -> void override;

    auto onTimerRunoutImpl() -> void override;

    void jumpIgnoringExitTo(WorkflowState& state);
};

} // namespace sprint_timer

#endif /* end of include guard: WORKFLOW_H_QKH9ETW3 */
