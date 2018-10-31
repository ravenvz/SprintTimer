/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

// TODO remove when Gtest drops std::tr1
// Workaround for C++17 as std::tr1 no longer available and Gtest uses it
#define GTEST_LANG_CXX11 1

#include "core/StatefulTimer.h"
#include "gtest/gtest.h"

using namespace sprint_timer;

namespace {

/* Fixed configuration for testing purposes. */
class TestConfig : public IConfig {

public:
    int soundVolume() const override { return mSoundVolume; }

    void setSoundVolume(int soundVolume) override
    {
        TestConfig::mSoundVolume = soundVolume;
    }

    std::chrono::minutes sprintDuration() const override
    {
        return mSprintDuration;
    }

    void setSprintDuration(std::chrono::minutes minutes) override
    {
        TestConfig::mSprintDuration = minutes;
    }

    std::chrono::minutes shortBreakDuration() const override
    {
        return mShortBreakDuration;
    }

    void setShortBreakDuration(std::chrono::minutes duration) override
    {
        TestConfig::mShortBreakDuration = duration;
    }

    std::chrono::minutes longBreakDuration() const override
    {
        return mLongBreakDuration;
    }

    void setLongBreakDuration(std::chrono::minutes duration) override
    {
        TestConfig::mLongBreakDuration = duration;
    }

    int numSprintsBeforeBreak() const override { return mTasksBeforeBreak; }

    void setNumSprintsBeforeBreak(int tasksBeforeBreak) override
    {
        TestConfig::mTasksBeforeBreak = tasksBeforeBreak;
    }

    bool soundIsEnabled() const override { return mPlaySound; }

    void setPlaySound(bool playSound) override
    {
        TestConfig::mPlaySound = playSound;
    }

    int dailyGoal() const override { return 0; }

    void setDailyGoal(int numSprints) override {}

    int weeklyGoal() const override { return 0; }

    void setWeeklyGoal(int numSprints) override {}

    int monthlyGoal() const override { return 0; }

    void setMonthlyGoal(int numSprints) override {}

    std::string soundFilePath() const override { return ""; }

    void setSoundFilePath(const std::string& filePath) override {}

    int timerFlavour() const override { return 0; }

    void setTimerFlavour(int timerVariation) override {}

    utils::WeekdaySelection workdays() const override
    {
        return utils::WeekdaySelection{0};
    }

    void setWorkdays(const utils::WeekdaySelection& workdays) override {}

private:
    std::chrono::minutes mSprintDuration{30};
    std::chrono::minutes mShortBreakDuration{10};
    std::chrono::minutes mLongBreakDuration{20};
    int mTasksBeforeBreak{4};
    bool mPlaySound{false};
    int mSoundVolume{0};
};

/* Extends StatefulTimer to provide public method to set state, along with
 * method to transition to next state. That allows to test state transitions
 * without having to deal with actual counting timer. */
class StatefulTimerTest : public StatefulTimer {
public:
    StatefulTimerTest(
        std::function<void(std::chrono::seconds)> tickCallback,
        std::function<void(IStatefulTimer::StateId)> onStateChangedCallback,
        std::chrono::seconds tickPeriod,
        const IConfig& applicationSettings)
        : StatefulTimer{tickCallback,
                        onStateChangedCallback,
                        tickPeriod,
                        applicationSettings}
    {
    }

    void set_state(TimerState* state) { transitionToState(*state); }

    TimerState* state() { return currentState; }
    TimerState* idle_state() { return &idle; }
    TimerState* sprint_state() { return &runningSprint; }
    TimerState* finished_state() { return &sprintFinished; }
    TimerState* short_break_state() { return &shortBreak; }
    TimerState* long_break_state() { return &longBreak; }
    TimerState* zone_state() { return &zone; }

    void transition_to_next_state() { currentState->setNextState(*this); }
};

} // namespace


class StatefulTimerStates : public ::testing::Test {

protected:
    virtual void SetUp()
    {
        std::chrono::seconds tick{1};
        auto onTickCallbackStub = [](std::chrono::seconds) {};
        IStatefulTimer::StateId state_id;
        auto onStateChangedCallbackStub
            = [&state_id](IStatefulTimer::StateId state) { state_id = state; };
        timer = std::make_unique<StatefulTimerTest>(
            onTickCallbackStub, onStateChangedCallbackStub, tick, testSettings);
    }

    TestConfig testSettings;
    std::unique_ptr<StatefulTimerTest> timer;
};

TEST_F(StatefulTimerStates, test_timer_should_initially_be_in_idle_state)
{
    EXPECT_EQ(timer->idle_state(), timer->state());
}

TEST_F(StatefulTimerStates,
       test_should_transition_to_sprint_state_from_idle_state)
{
    timer->transition_to_next_state();
    EXPECT_EQ(timer->sprint_state(), timer->state());
}

TEST_F(StatefulTimerStates, test_should_transition_to_finished_state_after_task)
{
    timer->set_state(timer->sprint_state());
    timer->transition_to_next_state();

    EXPECT_EQ(timer->finished_state(), timer->state());
}

TEST_F(StatefulTimerStates, test_should_transition_to_break_after_finished)
{
    timer->set_state(timer->finished_state());
    timer->setNumFinishedSprints(1);
    timer->transition_to_next_state();

    EXPECT_EQ(timer->short_break_state(), timer->state());
}

TEST_F(StatefulTimerStates,
       test_should_transition_to_long_break_after_finished_when_met_req)
{
    const int numTaskBeforeLongBreak{testSettings.numSprintsBeforeBreak()};
    timer->setNumFinishedSprints(numTaskBeforeLongBreak - 1);
    timer->set_state(timer->finished_state());
    timer->transition_to_next_state();

    EXPECT_EQ(timer->long_break_state(), timer->state());
}

TEST_F(StatefulTimerStates, test_should_transition_to_idle_from_break)
{
    timer->set_state(timer->short_break_state());
    timer->transition_to_next_state();

    EXPECT_EQ(timer->idle_state(), timer->state());
}

TEST_F(StatefulTimerStates, test_should_transition_to_idle_from_long_break)
{
    timer->set_state(timer->long_break_state());
    timer->transition_to_next_state();

    EXPECT_EQ(timer->idle_state(), timer->state());
}

/* Test zone state transitions */

TEST_F(StatefulTimerStates, zone_transition_should_be_ignored_in_idle_state)
{
    timer->toggleInTheZoneMode();

    EXPECT_EQ(timer->idle_state(), timer->state());
}

TEST_F(StatefulTimerStates,
       zone_transition_should_be_ignored_in_short_break_state)
{
    timer->set_state(timer->short_break_state());
    timer->toggleInTheZoneMode();
    EXPECT_EQ(timer->short_break_state(), timer->state());
}

TEST_F(StatefulTimerStates,
       zone_transition_should_be_ignored_in_long_break_state)
{
    timer->set_state(timer->long_break_state());
    timer->toggleInTheZoneMode();
    EXPECT_EQ(timer->long_break_state(), timer->state());
}

TEST_F(StatefulTimerStates, zone_transition_should_be_ignored_in_finished_state)
{
    timer->set_state(timer->finished_state());
    timer->toggleInTheZoneMode();
    EXPECT_EQ(timer->finished_state(), timer->state());
}

TEST_F(StatefulTimerStates,
       zone_transition_should_transition_to_zone_when_in_running_sprint_state)
{
    timer->set_state(timer->sprint_state());
    timer->toggleInTheZoneMode();
    EXPECT_EQ(timer->zone_state(), timer->state());
}

TEST_F(StatefulTimerStates,
       zone_transition_should_transition_to_task_when_in_zone_state)
{
    timer->set_state(timer->zone_state());
    timer->toggleInTheZoneMode();
    EXPECT_EQ(timer->sprint_state(), timer->state());
}

/* Test cancelling state transitions */

TEST_F(StatefulTimerStates, cancelling_state_should_be_ignored_in_idle_state)
{
    timer->cancel();
    EXPECT_EQ(timer->idle_state(), timer->state());
}

TEST_F(StatefulTimerStates, cancelling_state_should_be_ignored_in_zone_state)
{
    timer->set_state(timer->zone_state());
    timer->cancel();
    EXPECT_EQ(timer->zone_state(), timer->state());
}

TEST_F(StatefulTimerStates,
       transitions_to_idle_state_when_cancelling_finished_state)
{
    timer->set_state(timer->finished_state());
    timer->cancel();
    EXPECT_EQ(timer->idle_state(), timer->state());
}

TEST_F(StatefulTimerStates,
       transitions_to_idle_state_when_cancelling_task_state)
{
    timer->set_state(timer->sprint_state());
    timer->cancel();
    EXPECT_EQ(timer->idle_state(), timer->state());
}

TEST_F(StatefulTimerStates,
       transitions_to_idle_state_when_cancelling_short_break)
{
    timer->set_state(timer->short_break_state());
    timer->cancel();
    EXPECT_EQ(timer->idle_state(), timer->state());
}

TEST_F(StatefulTimerStates,
       transitions_to_idle_state_when_cancelling_long_break)
{
    timer->set_state(timer->long_break_state());
    timer->cancel();
    EXPECT_EQ(timer->idle_state(), timer->state());
}
