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
#include "mocks/ConfigMock.h"
#include "gmock/gmock.h"
#include <core/Workflow.h>

using namespace sprint_timer;

namespace {

constexpr std::chrono::seconds sprintDuration{1500};
constexpr std::chrono::seconds shortBreakDuration{300};
constexpr std::chrono::seconds longBreakDuration{900};
constexpr int32_t sprintsBeforeBreakStarted{4};
constexpr IWorkflow::WorkflowParams workflowParams{
    sprintDuration, shortBreakDuration, longBreakDuration, 4};

/* Extends Workflow to provide public method to set state, along with
 * method to transition to next state. That allows to test state transitions
 * without having to deal with actual counting timer. */
class WorkflowTest : public Workflow {
public:
    WorkflowTest(std::chrono::seconds tickPeriod,
                 const IWorkflow::WorkflowParams& workflowParams_)
        : Workflow{tickPeriod, workflowParams_}
    {
    }

    void set_state(WorkflowState* state) { transitionToState(*state); }

    WorkflowState* state() { return currentState; }
    WorkflowState* idle_state() { return &idle; }
    WorkflowState* sprint_state() { return &runningSprint; }
    WorkflowState* finished_state() { return &sprintFinished; }
    WorkflowState* short_break_state() { return &shortBreak; }
    WorkflowState* long_break_state() { return &longBreak; }
    WorkflowState* zone_state() { return &zone; }

    void transition_to_next_state() { currentState->exit(*this); }

    void fill_buffer(const std::vector<dw::DateTimeRange>& intervals)
    {
        buffer = intervals;
    }
};

} // namespace

class WorkflowListenerMock : public IWorkflow::WorkflowListener {
public:
    MOCK_METHOD(void, onTimerTick, (std::chrono::seconds), (override));

    MOCK_METHOD(void, onWorkflowStateChanged, (IWorkflow::StateId), (override));
};

class WorkflowStates : public ::testing::Test {

protected:
    void SetUp() override
    {
        std::chrono::seconds tick{1};
        workflow = std::make_unique<WorkflowTest>(tick, workflowParams);
        workflow->addListener(&listener);
    }

    ConfigMock configMock;
    WorkflowListenerMock listener;
    std::unique_ptr<WorkflowTest> workflow;
};

TEST_F(WorkflowStates, should_initially_be_in_idle_state)
{
    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

TEST_F(WorkflowStates, should_transition_to_sprint_state_from_idle_state)
{
    EXPECT_CALL(listener,
                onWorkflowStateChanged(IWorkflow::StateId::RunningSprint));

    workflow->transition_to_next_state();

    EXPECT_EQ(workflow->sprint_state(), workflow->state());
}

TEST_F(WorkflowStates, should_transition_to_finished_state_after_task)
{
    workflow->set_state(workflow->sprint_state());
    EXPECT_CALL(listener,
                onWorkflowStateChanged(IWorkflow::StateId::SprintFinished));

    workflow->transition_to_next_state();

    EXPECT_EQ(workflow->finished_state(), workflow->state());
}

TEST_F(WorkflowStates, should_transition_to_break_after_finished)
{
    workflow->set_state(workflow->finished_state());
    workflow->setNumFinishedSprints(1);
    EXPECT_CALL(listener,
                onWorkflowStateChanged(IWorkflow::StateId::BreakStarted));

    workflow->transition_to_next_state();

    EXPECT_EQ(workflow->short_break_state(), workflow->state());
}

TEST_F(WorkflowStates,
       should_transition_to_long_break_after_finished_when_met_req)
{
    workflow->setNumFinishedSprints(sprintsBeforeBreakStarted - 1);
    workflow->set_state(workflow->finished_state());
    EXPECT_CALL(listener,
                onWorkflowStateChanged(IWorkflow::StateId::BreakStarted));

    workflow->transition_to_next_state();

    EXPECT_EQ(workflow->long_break_state(), workflow->state());
}

TEST_F(WorkflowStates, should_transition_to_idle_from_break)
{
    workflow->set_state(workflow->short_break_state());
    EXPECT_CALL(listener,
                onWorkflowStateChanged(IWorkflow::StateId::BreakFinished));
    EXPECT_CALL(listener, onWorkflowStateChanged(IWorkflow::StateId::Idle));

    workflow->transition_to_next_state();

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

TEST_F(WorkflowStates, should_transition_to_idle_from_long_break)
{
    workflow->set_state(workflow->long_break_state());
    EXPECT_CALL(listener,
                onWorkflowStateChanged(IWorkflow::StateId::BreakFinished));
    EXPECT_CALL(listener, onWorkflowStateChanged(IWorkflow::StateId::Idle));

    workflow->transition_to_next_state();

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

/* Test zone state transitions */

TEST_F(WorkflowStates, zone_transition_should_be_ignored_in_idle_state)
{
    workflow->toggleInTheZoneMode();
    EXPECT_CALL(listener,
                onWorkflowStateChanged(IWorkflow::StateId::ZoneEntered))
        .Times(0);
    EXPECT_CALL(listener, onWorkflowStateChanged(IWorkflow::StateId::ZoneLeft))
        .Times(0);

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

TEST_F(WorkflowStates, zone_transition_should_be_ignored_in_short_break_state)
{
    workflow->set_state(workflow->short_break_state());
    EXPECT_CALL(listener,
                onWorkflowStateChanged(IWorkflow::StateId::ZoneEntered))
        .Times(0);
    EXPECT_CALL(listener, onWorkflowStateChanged(IWorkflow::StateId::ZoneLeft))
        .Times(0);

    workflow->toggleInTheZoneMode();

    EXPECT_EQ(workflow->short_break_state(), workflow->state());
}

TEST_F(WorkflowStates, zone_transition_should_be_ignored_in_long_break_state)
{
    workflow->set_state(workflow->long_break_state());
    EXPECT_CALL(listener,
                onWorkflowStateChanged(IWorkflow::StateId::ZoneEntered))
        .Times(0);
    EXPECT_CALL(listener, onWorkflowStateChanged(IWorkflow::StateId::ZoneLeft))
        .Times(0);

    workflow->toggleInTheZoneMode();

    EXPECT_EQ(workflow->long_break_state(), workflow->state());
}

TEST_F(WorkflowStates, zone_transition_should_be_ignored_in_finished_state)
{
    workflow->set_state(workflow->finished_state());
    EXPECT_CALL(listener,
                onWorkflowStateChanged(IWorkflow::StateId::ZoneEntered))
        .Times(0);
    EXPECT_CALL(listener, onWorkflowStateChanged(IWorkflow::StateId::ZoneLeft))
        .Times(0);

    workflow->toggleInTheZoneMode();

    EXPECT_EQ(workflow->finished_state(), workflow->state());
}

TEST_F(WorkflowStates,
       zone_transition_should_transition_to_zone_when_in_running_sprint_state)
{
    workflow->set_state(workflow->sprint_state());
    EXPECT_CALL(listener,
                onWorkflowStateChanged(IWorkflow::StateId::ZoneEntered));

    workflow->toggleInTheZoneMode();

    EXPECT_EQ(workflow->zone_state(), workflow->state());
}

TEST_F(WorkflowStates,
       zone_transition_should_transition_to_running_sprint_when_in_zone_state)
{
    workflow->set_state(workflow->zone_state());
    EXPECT_CALL(listener, onWorkflowStateChanged(IWorkflow::StateId::ZoneLeft));

    workflow->toggleInTheZoneMode();

    EXPECT_EQ(workflow->sprint_state(), workflow->state());
}

/* Test cancelling state transitions */

TEST_F(WorkflowStates, cancelling_state_should_be_ignored_in_idle_state)
{
    workflow->cancel();

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

TEST_F(WorkflowStates, cancelling_state_should_be_ignored_in_zone_state)
{
    workflow->set_state(workflow->zone_state());

    workflow->cancel();

    EXPECT_EQ(workflow->zone_state(), workflow->state());
}

TEST_F(WorkflowStates, transitions_to_idle_state_when_cancelling_finished_state)
{
    workflow->set_state(workflow->finished_state());
    EXPECT_CALL(listener, onWorkflowStateChanged(IWorkflow::StateId::Idle));

    workflow->cancel();

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

TEST_F(WorkflowStates, transitions_to_idle_state_when_cancelling_task)
{
    workflow->set_state(workflow->sprint_state());
    EXPECT_CALL(listener, onWorkflowStateChanged(IWorkflow::StateId::Idle));

    workflow->cancel();

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

TEST_F(WorkflowStates, transitions_to_idle_state_when_cancelling_short_break)
{
    workflow->set_state(workflow->short_break_state());
    EXPECT_CALL(listener, onWorkflowStateChanged(IWorkflow::StateId::Idle));

    workflow->cancel();

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

TEST_F(WorkflowStates, transitions_to_idle_state_when_cancelling_long_break)
{
    workflow->set_state(workflow->long_break_state());
    EXPECT_CALL(listener, onWorkflowStateChanged(IWorkflow::StateId::Idle));

    workflow->cancel();

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

TEST_F(WorkflowStates, clears_buffer_when_exits_finished_state)
{
    using namespace std::chrono_literals;
    const dw::DateTimeRange someInterval{dw::current_date_time(),
                                         dw::current_date_time() + 25min};
    const std::vector<dw::DateTimeRange> intervals{
        someInterval, dw::add_offset(someInterval, 25min)};
    workflow->fill_buffer(intervals);
    workflow->set_state(workflow->finished_state());

    workflow->transition_to_next_state();

    ASSERT_TRUE(workflow->completedSprints().empty());
}

TEST_F(WorkflowStates, clears_buffer_when_cancels_finished_state)
{
    using namespace std::chrono_literals;
    const dw::DateTimeRange someInterval{dw::current_date_time(),
                                         dw::current_date_time() + 25min};
    const std::vector<dw::DateTimeRange> intervals{
        someInterval, dw::add_offset(someInterval, 25min)};
    workflow->fill_buffer(intervals);
    workflow->set_state(workflow->finished_state());

    workflow->cancel();

    ASSERT_TRUE(workflow->completedSprints().empty());
}
