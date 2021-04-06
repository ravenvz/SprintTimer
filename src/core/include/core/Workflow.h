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

#include "core/IWorkflow.h"
#include "core/PeriodicBackgroundRunner.h"
#include "date_wrapper/date_wrapper.h"
#include <chrono>
#include <memory>
#include <thread>
#include <unordered_set>
#include <vector>

namespace sprint_timer {

class Workflow : public IWorkflow {
    friend class WorkflowState;
    friend class ShortBreak;
    friend class LongBreak;
    friend class RunningSprint;
    friend class Zone;
    friend class Idle;
    friend class SprintFinished;

public:
    Workflow(std::chrono::seconds tickPeriod,
             const IWorkflow::WorkflowParams& params);

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

    void setNumFinishedSprints(int num) override;

    int numFinishedSprints() const;

    void addListener(WorkflowListener* listener) override;

    void removeListener(WorkflowListener* listener) override;

    void reconfigure(const WorkflowParams& params) override;

protected:
    Idle idle;
    RunningSprint runningSprint;
    ShortBreak shortBreak;
    LongBreak longBreak;
    Zone zone;
    SprintFinished sprintFinished;
    WorkflowState* currentState;
    std::vector<dw::DateTimeRange> buffer;

    /* Transition to another state while exiting current state properly. */
    void transitionToState(WorkflowState& state);

    void notifyStateChanged(IWorkflow::StateId stateId);

private:
    const std::chrono::seconds tickInterval;
    WorkflowParams params;
    std::unique_ptr<PeriodicBackgroundRunner> periodicBackgroundRunner;
    dw::DateTime mStart;
    int finishedSprints{0};
    std::unordered_set<WorkflowListener*> listeners;

    /* Jump to another state immediately ignoring proper exit from current state
     */
    void jumpIgnoringExitTo(WorkflowState& state);
    bool longBreakConditionMet() const;
    void onTimerRunout();
    void onTimerTick(PeriodicBackgroundRunner::TickPeriod timeLeft);
    void startCountdown();
};

} // namespace sprint_timer

#endif /* end of include guard: WORKFLOW_H_QKH9ETW3 */
