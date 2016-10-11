/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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
#include "core/StatefulTimer.h"
#include <TestHarness.h>

#include <iostream>


TEST_GROUP(StatefulTimerStates)
{

    class TestConfig : public IConfig {

    public:
        TestConfig() {}

        int soundVolume() const override { return mSoundVolume; }

        void setSoundVolume(int soundVolume) override
        {
            TestConfig::mSoundVolume = soundVolume;
        }

        int sprintDuration() const override { return mSprintDuration; }

        void setSprintDuration(int minutes) override
        {
            TestConfig::mSprintDuration = minutes;
        }

        int shortBreakDuration() const override { return mShortBreakDuration; }

        void setShortBreakDuration(int minutes) override
        {
            TestConfig::mShortBreakDuration = minutes;
        }

        int longBreakDuration() const override { return mLongBreakDuration; }

        void setLongBreakDuration(int minutes) override
        {
            TestConfig::mLongBreakDuration = minutes;
        }

        int numSprintsBeforeBreak() const override
        {
            return mTasksBeforeBreak;
        }

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

    private:
        int mSprintDuration = 30;
        int mShortBreakDuration = 10;
        int mLongBreakDuration = 20;
        int mTasksBeforeBreak = 4;
        bool mPlaySound{false};
        int mSoundVolume{0};
    };

    /* Extends StatefulTimer to provide public method to set state, along with
     * method to transition to next state. That allows to test state transitions
     * without having to deal with actual counting timer. */
    class StatefulTimerTest : public StatefulTimer {
    public:
        StatefulTimerTest(
            std::function<void(long long)> tickCallback,
            std::function<void(IStatefulTimer::State)> onStateChangedCallback,
            long tickPeriodInMillisecs,
            const IConfig& applicationSettings)
            : StatefulTimer{tickCallback,
                            onStateChangedCallback,
                            tickPeriodInMillisecs,
                            applicationSettings}
        {
        }

        void setState(IStatefulTimer::State state)
        {
            switch (state) {
            case IStatefulTimer::State::Idle:
                currentState = idleState.get();
                break;
            case IStatefulTimer::State::Task:
                currentState = sprintState.get();
                break;
            case IStatefulTimer::State::Break:
                currentState = shortBreakState.get();
                break;
            case IStatefulTimer::State::LongBreak:
                currentState = longBreakState.get();
                break;
            case IStatefulTimer::State::ZoneEntered:
                currentState = zoneState.get();
                break;
            case IStatefulTimer::State::ZoneLeft:
                currentState = sprintState.get();
                break;
            case IStatefulTimer::State::Finished:
                currentState = finishedState.get();
                break;
            default:
                return;
            }
        }

        void transitionToNext() { currentState->setNextState(); }
    };

    void setup()
    {
        int tick{1000};
        auto onTickCallbackStub = [](long long) {};
        auto onStateChangedCallbackStub = [](IStatefulTimer::State) {};
        timer = std::make_unique<StatefulTimerTest>(
            onTickCallbackStub, onStateChangedCallbackStub, tick, testSettings);
    }

    TestConfig testSettings;
    std::unique_ptr<StatefulTimerTest> timer;
};

TEST(StatefulTimerStates, test_timer_should_initially_be_in_idle_state)
{
    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Idle),
                static_cast<int>(timer->state()));
}

TEST(StatefulTimerStates, test_should_transition_to_task_state_from_idle_state)
{
    timer->transitionToNext();
    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Task),
                static_cast<int>(timer->state()));
}

TEST(StatefulTimerStates, test_should_transition_to_finished_state_after_task)
{
    timer->setState(IStatefulTimer::State::Task);
    timer->transitionToNext();
    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Finished),
                static_cast<int>(timer->state()));
}

TEST(StatefulTimerStates, test_should_transition_to_break_after_finished)
{
    timer->setState(IStatefulTimer::State::Finished);
    timer->setNumFinishedSprints(1);
    timer->transitionToNext();
    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Break),
                static_cast<int>(timer->state()));
}

TEST(StatefulTimerStates,
     test_should_transition_to_long_break_after_finished_when_met_req)
{
    timer->setState(IStatefulTimer::State::Finished);
    int numTaskBeforeLongBreak{4};
    timer->setNumFinishedSprints(numTaskBeforeLongBreak);
    timer->transitionToNext();
    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::LongBreak),
                static_cast<int>(timer->state()));
}

TEST(StatefulTimerStates, test_should_transition_to_idle_from_break)
{
    timer->setState(IStatefulTimer::State::Break);
    timer->transitionToNext();
    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Idle),
                static_cast<int>(timer->state()));
}

TEST(StatefulTimerStates, test_should_transition_to_idle_from_long_break)
{
    timer->setState(IStatefulTimer::State::LongBreak);
    timer->transitionToNext();
    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Idle),
                static_cast<int>(timer->state()));
}

TEST(StatefulTimerStates, test_zone_state_transition)
{
    // Should be ignored in Idle state
    timer->toggleInTheZoneMode();
    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Idle),
                static_cast<int>(timer->state()));

    // Should be ignored in Break state
    timer->setState(IStatefulTimer::State::Break);
    timer->toggleInTheZoneMode();
    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Break),
                static_cast<int>(timer->state()));

    // Should be ignored in LongBreak state
    timer->setState(IStatefulTimer::State::LongBreak);
    timer->toggleInTheZoneMode();
    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::LongBreak),
                static_cast<int>(timer->state()));

    // Should be ignored in Finished state
    timer->setState(IStatefulTimer::State::Finished);
    timer->toggleInTheZoneMode();
    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Finished),
                static_cast<int>(timer->state()));

    // Should transition to Zone if in Task state
    timer->setState(IStatefulTimer::State::Task);
    timer->toggleInTheZoneMode();
    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::ZoneEntered),
                static_cast<int>(timer->state()));

    // Should transition to Task if in Zone state
    timer->setState(IStatefulTimer::State::ZoneEntered);
    timer->toggleInTheZoneMode();
    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Task),
                static_cast<int>(timer->state()));
}

TEST(StatefulTimerStates, test_cancelling_state)
{
//    // Should be ignored in Idle state
//    timer->cancel();
//    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Idle),
//                static_cast<int>(timer->state()));
//
//    // Should be ignored in Zone state
//    timer->setState(IStatefulTimer::State::ZoneEntered);
//    timer->cancel();
//    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::ZoneEntered),
//                static_cast<int>(timer->state()));
//
//    // Should transition to Idle state when cancelling Finished state
//    timer->setState(IStatefulTimer::State::Finished);
//    timer->cancel();
//    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Idle),
//                static_cast<int>(timer->state()));
//
//    // Should transition to Idle state when cancelling Task state
//    timer->setState(IStatefulTimer::State::Task);
//    // timer->start();
//    timer->cancel();
//    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Idle),
//                static_cast<int>(timer->state()));
//
//    // Should transition to Idle state when cancelling Break
//    timer->setState(IStatefulTimer::State::Break);
//    timer->cancel();
//    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Idle),
//                static_cast<int>(timer->state()));
//
//    // Should transition to Idle state when cancelling LongBreak
//    timer->setState(IStatefulTimer::State::LongBreak);
//    timer->cancel();
//    CHECK_EQUAL(static_cast<int>(IStatefulTimer::State::Idle),
//                static_cast<int>(timer->state()));
}
