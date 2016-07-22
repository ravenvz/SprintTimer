/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "core/PomodoroTimer.h"
#include <TestHarness.h>

#include <iostream>


TEST_GROUP(PomodoroTimerStates)
{

    class TestConfig : public IConfig {

    public:
        TestConfig() {}

        int soundVolume() const override { return mSoundVolume; }

        void setSoundVolume(int soundVolume) override
        {
            TestConfig::mSoundVolume = soundVolume;
        }

        int pomodoroDuration() const override { return mPomodoroDuration; }

        void setPomodoroDuration(int pomodoroDuration) override
        {
            TestConfig::mPomodoroDuration = pomodoroDuration;
        }

        int shortBreakDuration() const override { return mShortBreakDuration; }

        void setShortBreakDuration(int shortBreakDuration) override
        {
            TestConfig::mShortBreakDuration = shortBreakDuration;
        }

        int longBreakDuration() const override { return mLongBreakDuration; }

        void setLongBreakDuration(int longBreakDuration) override
        {
            TestConfig::mLongBreakDuration = longBreakDuration;
        }

        int numPomodorosBeforeBreak() const override
        {
            return mTasksBeforeBreak;
        }

        void setPomodorosBeforeBreak(int tasksBeforeBreak) override
        {
            TestConfig::mTasksBeforeBreak = tasksBeforeBreak;
        }

        bool soundIsEnabled() const override { return mPlaySound; }

        void setPlaySound(bool playSound) override
        {
            TestConfig::mPlaySound = playSound;
        }

        int dailyPomodorosGoal() const override { return 0; }

        void setDailyPomodorosGoal(int dailyPomodorosGoal) override {}

        int weeklyPomodorosGoal() const override { return 0; }

        void setWeeklyPomodorosGoal(int weeklyPomodorosGoal) override {}

        int monthlyPomodorosGoal() const override { return 0; }

        void setMonthlyPomodorosGoal(int monthlyPomodorosGoal) override {}

        std::string soundFilePath() const override { return ""; }

        void setSoundFilePath(const std::string& filePath) override {}

    private:
        int mPomodoroDuration = 30;
        int mShortBreakDuration = 10;
        int mLongBreakDuration = 20;
        int mTasksBeforeBreak = 4;
        bool mPlaySound{false};
        int mSoundVolume{0};
    };

    /* Extends PomodoroTimer to provide public method to set state, along with
     * method to transition to next state. That allows to test state transitions
     * without having to deal with actual counting timer. */
    class PomodoroTimerTest : public PomodoroTimer {
    public:
        PomodoroTimerTest(
            std::function<void(long)> tickCallback,
            std::function<void(IPomodoroTimer::State)> onStateChangedCallback,
            long tickPeriodInMillisecs,
            const IConfig& applicationSettings)
            : PomodoroTimer{tickCallback,
                            onStateChangedCallback,
                            tickPeriodInMillisecs,
                            applicationSettings}
        {
        }

        void setState(IPomodoroTimer::State state)
        {
            switch (state) {
            case IPomodoroTimer::State::Idle:
                currentState = idleState.get();
                break;
            case IPomodoroTimer::State::Task:
                currentState = pomodoroState.get();
                break;
            case IPomodoroTimer::State::Break:
                currentState = shortBreakState.get();
                break;
            case IPomodoroTimer::State::LongBreak:
                currentState = longBreakState.get();
                break;
            case IPomodoroTimer::State::ZoneEntered:
                currentState = zoneState.get();
                break;
            case IPomodoroTimer::State::ZoneLeft:
                currentState = pomodoroState.get();
                break;
            case IPomodoroTimer::State::Finished:
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
        auto onTickCallbackStub = [](long) {};
        auto onStateChangedCallbackStub = [](IPomodoroTimer::State) {};
        timer = std::make_unique<PomodoroTimerTest>(
            onTickCallbackStub, onStateChangedCallbackStub, tick, testSettings);
    }

    TestConfig testSettings;
    std::unique_ptr<PomodoroTimerTest> timer;
};

TEST(PomodoroTimerStates, test_timer_should_initially_be_in_idle_state)
{
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Idle),
                static_cast<int>(timer->state()));
}

TEST(PomodoroTimerStates, test_should_transition_to_task_state_from_idle_state)
{
    timer->transitionToNext();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Task),
                static_cast<int>(timer->state()));
}

TEST(PomodoroTimerStates, test_should_transition_to_finished_state_after_task)
{
    timer->setState(IPomodoroTimer::State::Task);
    timer->transitionToNext();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Finished),
                static_cast<int>(timer->state()));
}

TEST(PomodoroTimerStates, test_should_transition_to_break_after_finished)
{
    timer->setState(IPomodoroTimer::State::Finished);
    timer->setNumCompletedPomodoros(1);
    timer->transitionToNext();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Break),
                static_cast<int>(timer->state()));
}

TEST(PomodoroTimerStates,
     test_should_transition_to_long_break_after_finished_when_met_req)
{
    timer->setState(IPomodoroTimer::State::Finished);
    int numTaskBeforeLongBreak{4};
    timer->setNumCompletedPomodoros(numTaskBeforeLongBreak);
    timer->transitionToNext();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::LongBreak),
                static_cast<int>(timer->state()));
}

TEST(PomodoroTimerStates, test_should_transition_to_idle_from_break)
{
    timer->setState(IPomodoroTimer::State::Break);
    timer->transitionToNext();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Idle),
                static_cast<int>(timer->state()));
}

TEST(PomodoroTimerStates, test_should_transition_to_idle_from_long_break)
{
    timer->setState(IPomodoroTimer::State::LongBreak);
    timer->transitionToNext();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Idle),
                static_cast<int>(timer->state()));
}

TEST(PomodoroTimerStates, test_zone_state_transition)
{
    // Should be ignored in Idle state
    timer->toggleInTheZoneMode();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Idle),
                static_cast<int>(timer->state()));

    // Should be ignored in Break state
    timer->setState(IPomodoroTimer::State::Break);
    timer->toggleInTheZoneMode();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Break),
                static_cast<int>(timer->state()));

    // Should be ignored in LongBreak state
    timer->setState(IPomodoroTimer::State::LongBreak);
    timer->toggleInTheZoneMode();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::LongBreak),
                static_cast<int>(timer->state()));

    // Should be ignored in Finished state
    timer->setState(IPomodoroTimer::State::Finished);
    timer->toggleInTheZoneMode();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Finished),
                static_cast<int>(timer->state()));

    // Should transition to Zone if in Task state
    timer->setState(IPomodoroTimer::State::Task);
    timer->toggleInTheZoneMode();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::ZoneEntered),
                static_cast<int>(timer->state()));

    // Should transition to Task if in Zone state
    timer->setState(IPomodoroTimer::State::ZoneEntered);
    timer->toggleInTheZoneMode();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Task),
                static_cast<int>(timer->state()));
}

TEST(PomodoroTimerStates, test_cancelling_state)
{
    // Should be ignored in Idle state
    timer->cancel();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Idle),
                static_cast<int>(timer->state()));

    // Should be ignored in Zone state
    timer->setState(IPomodoroTimer::State::ZoneEntered);
    timer->cancel();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::ZoneEntered),
                static_cast<int>(timer->state()));

    // Should transition to Idle state when cancelling Finished state
    timer->setState(IPomodoroTimer::State::Finished);
    timer->cancel();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Idle),
                static_cast<int>(timer->state()));

    // Should transition to Idle state when cancelling Task state
    timer->setState(IPomodoroTimer::State::Task);
    // timer->start();
    timer->cancel();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Idle),
                static_cast<int>(timer->state()));

    // Should transition to Idle state when cancelling Break
    timer->setState(IPomodoroTimer::State::Break);
    timer->cancel();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Idle),
                static_cast<int>(timer->state()));

    // Should transition to Idle state when cancelling LongBreak
    timer->setState(IPomodoroTimer::State::LongBreak);
    timer->cancel();
    CHECK_EQUAL(static_cast<int>(IPomodoroTimer::State::Idle),
                static_cast<int>(timer->state()));
}
