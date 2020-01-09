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
#include "gtest/gtest.h"
#include <core/Workflow.h>

using namespace sprint_timer;

namespace {

// #<{(| Fixed configuration for testing purposes. |)}>#
// class TestConfig : public IConfig {
//
// public:
//     int soundVolume() const override { return mSoundVolume; }
//
//     void setSoundVolume(int soundVolume) override
//     {
//         TestConfig::mSoundVolume = soundVolume;
//     }
//
//     std::chrono::minutes sprintDuration() const override
//     {
//         return mSprintDuration;
//     }
//
//     void setSprintDuration(std::chrono::minutes minutes) override
//     {
//         TestConfig::mSprintDuration = minutes;
//     }
//
//     std::chrono::minutes shortBreakDuration() const override
//     {
//         return mShortBreakDuration;
//     }
//
//     void setShortBreakDuration(std::chrono::minutes duration) override
//     {
//         TestConfig::mShortBreakDuration = duration;
//     }
//
//     std::chrono::minutes longBreakDuration() const override
//     {
//         return mLongBreakDuration;
//     }
//
//     void setLongBreakDuration(std::chrono::minutes duration) override
//     {
//         TestConfig::mLongBreakDuration = duration;
//     }
//
//     int numSprintsBeforeBreak() const override { return mTasksBeforeBreak; }
//
//     void setNumSprintsBeforeBreak(int tasksBeforeBreak) override
//     {
//         TestConfig::mTasksBeforeBreak = tasksBeforeBreak;
//     }
//
//     bool soundIsEnabled() const override { return mPlaySound; }
//
//     void setPlaySound(bool playSound) override
//     {
//         TestConfig::mPlaySound = playSound;
//     }
//
//     std::string soundFilePath() const override { return ""; }
//
//     void setSoundFilePath(const std::string& filePath) override {}
//
//     int timerFlavour() const override { return 0; }
//
//     void setTimerFlavour(int timerVariation) override {}
//
//     utils::WeekdaySelection workdays() const override
//     {
//         return utils::WeekdaySelection{0};
//     }
//
//     void setWorkdays(const utils::WeekdaySelection& workdays) override {}
//
//     dw::Weekday firstDayOfWeek() const override { return dw::Weekday::Monday;
//     }
//
//     void setFirstDayOfWeek(dw::Weekday firstDayOfWeek) override {}
//
// private:
//     std::chrono::minutes mSprintDuration{30};
//     std::chrono::minutes mShortBreakDuration{10};
//     std::chrono::minutes mLongBreakDuration{20};
//     int mTasksBeforeBreak{4};
//     bool mPlaySound{false};
//     int mSoundVolume{0};
// };

/* Extends Workflow to provide public method to set state, along with
 * method to transition to next state. That allows to test state transitions
 * without having to deal with actual counting timer. */
class WorkflowTest : public Workflow {
public:
    WorkflowTest(
        std::function<void(std::chrono::seconds)> tickCallback,
        std::function<void(IWorkflow::StateId)> onStateChangedCallback,
        std::chrono::seconds tickPeriod,
        const IConfig& applicationSettings)
        : Workflow{tickCallback,
                        onStateChangedCallback,
                        tickPeriod,
                        applicationSettings}
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

    void transition_to_next_state() { currentState->setNextState(*this); }
};

} // namespace


class WorkflowStates : public ::testing::Test {

protected:
    virtual void SetUp()
    {
        std::chrono::seconds tick{1};
        auto onTickCallbackStub = [](std::chrono::seconds) {};
        IWorkflow::StateId state_id;
        auto onStateChangedCallbackStub
            = [&state_id](IWorkflow::StateId state) { state_id = state; };
        workflow = std::make_unique<WorkflowTest>(
            onTickCallbackStub, onStateChangedCallbackStub, tick, configMock);
    }

    ConfigMock configMock;
    std::unique_ptr<WorkflowTest> workflow;
};

TEST_F(WorkflowStates, should_initially_be_in_idle_state)
{
    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

TEST_F(WorkflowStates, should_transition_to_sprint_state_from_idle_state)
{
    workflow->transition_to_next_state();

    EXPECT_EQ(workflow->sprint_state(), workflow->state());
}

TEST_F(WorkflowStates, should_transition_to_finished_state_after_task)
{
    workflow->set_state(workflow->sprint_state());

    workflow->transition_to_next_state();

    EXPECT_EQ(workflow->finished_state(), workflow->state());
}

TEST_F(WorkflowStates, should_transition_to_break_after_finished)
{
    const int numSprintsBeforeLongBreak{4};
    ON_CALL(configMock, numSprintsBeforeBreak)
        .WillByDefault(::testing::Return(numSprintsBeforeLongBreak));
    workflow->set_state(workflow->finished_state());
    workflow->setNumFinishedSprints(1);

    workflow->transition_to_next_state();

    EXPECT_EQ(workflow->short_break_state(), workflow->state());
}

TEST_F(WorkflowStates,
       should_transition_to_long_break_after_finished_when_met_req)
{
    const int numSprintsBeforeLongBreak{4};
    ON_CALL(configMock, numSprintsBeforeBreak)
        .WillByDefault(::testing::Return(numSprintsBeforeLongBreak));
    workflow->setNumFinishedSprints(numSprintsBeforeLongBreak - 1);
    workflow->set_state(workflow->finished_state());

    workflow->transition_to_next_state();

    EXPECT_EQ(workflow->long_break_state(), workflow->state());
}

TEST_F(WorkflowStates, should_transition_to_idle_from_break)
{
    workflow->set_state(workflow->short_break_state());

    workflow->transition_to_next_state();

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

TEST_F(WorkflowStates, should_transition_to_idle_from_long_break)
{
    workflow->set_state(workflow->long_break_state());

    workflow->transition_to_next_state();

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

/* Test zone state transitions */

TEST_F(WorkflowStates, zone_transition_should_be_ignored_in_idle_state)
{
    workflow->toggleInTheZoneMode();

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

TEST_F(WorkflowStates,
       zone_transition_should_be_ignored_in_short_break_state)
{
    workflow->set_state(workflow->short_break_state());

    workflow->toggleInTheZoneMode();

    EXPECT_EQ(workflow->short_break_state(), workflow->state());
}

TEST_F(WorkflowStates,
       zone_transition_should_be_ignored_in_long_break_state)
{
    workflow->set_state(workflow->long_break_state());

    workflow->toggleInTheZoneMode();

    EXPECT_EQ(workflow->long_break_state(), workflow->state());
}

TEST_F(WorkflowStates, zone_transition_should_be_ignored_in_finished_state)
{
    workflow->set_state(workflow->finished_state());

    workflow->toggleInTheZoneMode();

    EXPECT_EQ(workflow->finished_state(), workflow->state());
}

TEST_F(WorkflowStates,
       zone_transition_should_transition_to_zone_when_in_running_sprint_state)
{
    workflow->set_state(workflow->sprint_state());

    workflow->toggleInTheZoneMode();

    EXPECT_EQ(workflow->zone_state(), workflow->state());
}

TEST_F(WorkflowStates,
       zone_transition_should_transition_to_running_sprint_when_in_zone_state)
{
    workflow->set_state(workflow->zone_state());

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

TEST_F(WorkflowStates,
       transitions_to_idle_state_when_cancelling_finished_state)
{
    workflow->set_state(workflow->finished_state());

    workflow->cancel();

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

TEST_F(WorkflowStates,
       transitions_to_idle_state_when_cancelling_task_state)
{
    workflow->set_state(workflow->sprint_state());

    workflow->cancel();

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

TEST_F(WorkflowStates,
       transitions_to_idle_state_when_cancelling_short_break)
{
    workflow->set_state(workflow->short_break_state());

    workflow->cancel();

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}

TEST_F(WorkflowStates,
       transitions_to_idle_state_when_cancelling_long_break)
{
    workflow->set_state(workflow->long_break_state());

    workflow->cancel();

    EXPECT_EQ(workflow->idle_state(), workflow->state());
}
