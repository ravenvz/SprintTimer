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
#ifndef WORKFLOW_H_QKH9ETW3
#define WORKFLOW_H_QKH9ETW3

#include "core/IConfig.h"
#include "core/IWorkflow.h"
#include "core/PeriodicBackgroundRunner.h"
#include "date_wrapper/date_wrapper.h"
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

namespace sprint_timer {

class Workflow;

class WorkflowState {
public:
    virtual ~WorkflowState() = default;
    virtual void enter(Workflow& workflow) const = 0;
    virtual void cancelExit(Workflow& workflow) = 0;
    virtual void normalExit(Workflow& workflow) const = 0;
    virtual void setNextState(Workflow& workflow) = 0;
    virtual void toggleZoneMode(Workflow& workflow);
    virtual std::chrono::seconds duration(const Workflow& workflow) const;
    virtual void onTimerFinished(Workflow& workflow);
};

class Idle final : public WorkflowState {
public:
    void enter(Workflow& workflow) const override;
    void cancelExit(Workflow& workflow) override;
    void normalExit(Workflow& workflow) const override;
    void setNextState(Workflow& workflow) override;
};

class RunningSprint final : public WorkflowState {
public:
    void enter(Workflow& workflow) const override;
    void cancelExit(Workflow& workflow) override;
    void normalExit(Workflow& workflow) const override;
    void setNextState(Workflow& workflow) override;
    std::chrono::seconds duration(const Workflow& workflow) const override;
    void toggleZoneMode(Workflow& workflow) override;
    void onTimerFinished(Workflow& workflow) override;
};

class ShortBreak final : public WorkflowState {
public:
    void enter(Workflow& workflow) const override;
    void cancelExit(Workflow& workflow) override;
    void normalExit(Workflow& workflow) const override;
    void setNextState(Workflow& workflow) override;
    std::chrono::seconds duration(const Workflow& workflow) const override;
    void onTimerFinished(Workflow& workflow) override;
};

class LongBreak final : public WorkflowState {
public:
    void enter(Workflow& workflow) const override;
    void cancelExit(Workflow& workflow) override;
    void normalExit(Workflow& workflow) const override;
    void setNextState(Workflow& workflow) override;
    std::chrono::seconds duration(const Workflow& workflow) const override;
    void onTimerFinished(Workflow& workflow) override;
};

class Zone final : public WorkflowState {
public:
    void enter(Workflow& workflow) const override;
    void cancelExit(Workflow& workflow) override;
    void normalExit(Workflow& workflow) const override;
    void setNextState(Workflow& workflow) override;
    std::chrono::seconds duration(const Workflow& workflow) const override;
    void toggleZoneMode(Workflow& workflow) override;
    void onTimerFinished(Workflow& workflow) override;
};

class SprintFinished final : public WorkflowState {
public:
    void enter(Workflow& workflow) const override;
    void cancelExit(Workflow& workflow) override;
    void normalExit(Workflow& workflow) const override;
    void setNextState(Workflow& workflow) override;
};

class Workflow : public IWorkflow {
    friend class WorkflowState;
    friend class ShortBreak;
    friend class LongBreak;
    friend class RunningSprint;
    friend class Zone;
    friend class Idle;
    friend class SprintFinished;

public:
    Workflow(
        std::function<void(std::chrono::seconds timeLeft)> tickCallback,
        std::function<void(IWorkflow::StateId state)> onStateChangedCallback,
        std::chrono::seconds tickPeriod,
        const IConfig& applicationSettings);

    ~Workflow() override = default;

    Workflow(Workflow&&) = delete;
    Workflow& operator=(Workflow&&) = delete;

    Workflow(const Workflow&) = delete;
    Workflow& operator=(const Workflow&) = delete;

    void start() override;

    void cancel() override;

    std::chrono::seconds currentDuration() const override;

    void toggleInTheZoneMode() override;

    std::vector<dw::DateTimeRange> completedSprints() const override;

    void clearSprintsBuffer() override;

    void setNumFinishedSprints(int num) override;

    int numFinishedSprints() const;

protected:
    Idle idle;
    RunningSprint runningSprint;
    ShortBreak shortBreak;
    LongBreak longBreak;
    Zone zone;
    SprintFinished sprintFinished;
    WorkflowState* currentState;

    /* Transition to another state while exiting current state properly. */
    virtual void transitionToState(WorkflowState& state);

private:
    const IConfig& applicationSettings;
    const std::chrono::seconds tickInterval;
    std::function<void(std::chrono::seconds timeLeft)> onTickCallback;
    std::function<void(IWorkflow::StateId)> onStateChangedCallback;
    std::unique_ptr<PeriodicBackgroundRunner> periodicBackgroundRunner;
    dw::DateTime mStart;
    int finishedSprints{0};
    std::vector<dw::DateTimeRange> buffer;

    /* Jump to another state immediately ignoring proper exit from current state
     */
    void jumpToState(WorkflowState& state);
    bool longBreakConditionMet() const;
    void onTimerRunout();
    void notifyStateChanged(IWorkflow::StateId stateId);
    void onTimerTick(PeriodicBackgroundRunner::TickPeriod timeLeft);
    void startCountdown();
};

} // namespace sprint_timer

#endif /* end of include guard: WORKFLOW_H_QKH9ETW3 */
