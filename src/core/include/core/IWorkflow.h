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
#ifndef IWORKFLOW_H_S3AYV9DP
#define IWORKFLOW_H_S3AYV9DP

#include "date_wrapper/date_wrapper.h"
#include <vector>

namespace sprint_timer {

class Workflow;

class WorkflowState {
public:
    virtual ~WorkflowState() = default;
    virtual void enter(Workflow& workflow) const = 0;
    virtual void cancel(Workflow& workflow);
    virtual void exit(Workflow& workflow) = 0;
    virtual void toggleZoneMode(Workflow& workflow);
    virtual std::chrono::seconds duration(const Workflow& workflow) const;
    virtual void onTimerFinished(Workflow& workflow);
};

class Idle final : public WorkflowState {
public:
    void enter(Workflow& workflow) const override;
    void exit(Workflow& workflow) override;
};

class RunningSprint final : public WorkflowState {
public:
    void enter(Workflow& workflow) const override;
    void cancel(Workflow& workflow) override;
    void exit(Workflow& workflow) override;
    std::chrono::seconds duration(const Workflow& workflow) const override;
    void toggleZoneMode(Workflow& workflow) override;
    void onTimerFinished(Workflow& workflow) override;
};

class ShortBreak final : public WorkflowState {
public:
    void enter(Workflow& workflow) const override;
    void cancel(Workflow& workflow) override;
    void exit(Workflow& workflow) override;
    std::chrono::seconds duration(const Workflow& workflow) const override;
    void onTimerFinished(Workflow& workflow) override;
};

class LongBreak final : public WorkflowState {
public:
    void enter(Workflow& workflow) const override;
    void cancel(Workflow& workflow) override;
    void exit(Workflow& workflow) override;
    std::chrono::seconds duration(const Workflow& workflow) const override;
    void onTimerFinished(Workflow& workflow) override;
};

class Zone final : public WorkflowState {
public:
    void enter(Workflow& workflow) const override;
    void exit(Workflow& workflow) override;
    std::chrono::seconds duration(const Workflow& workflow) const override;
    void toggleZoneMode(Workflow& workflow) override;
    void onTimerFinished(Workflow& workflow) override;
};

class SprintFinished final : public WorkflowState {
public:
    void enter(Workflow& workflow) const override;
    void cancel(Workflow& workflow) override;
    void exit(Workflow& workflow) override;
};

class IWorkflow {
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

    class WorkflowListener {
    public:
        virtual ~WorkflowListener() = default;

        virtual void onTimerTick(std::chrono::seconds timeLeft) = 0;

        virtual void
        onWorkflowStateChanged(IWorkflow::StateId currentState) = 0;
    };

    virtual ~IWorkflow() = default;

    virtual void start() = 0;

    virtual void cancel() = 0;

    virtual std::chrono::seconds currentDuration() const = 0;

    virtual void toggleInTheZoneMode() = 0;

    virtual std::vector<dw::DateTimeRange> completedSprints() const = 0;

    virtual void setNumFinishedSprints(int num) = 0;

    virtual void addListener(WorkflowListener* listener) = 0;

    virtual void removeListener(WorkflowListener* listener) = 0;

    virtual void reconfigure(const WorkflowParams& params) = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: IWORKFLOW_H_S3AYV9DP */

