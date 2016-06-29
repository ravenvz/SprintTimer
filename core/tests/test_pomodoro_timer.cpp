#include "core/PomodoroTimer.h"
#include <TestHarness.h>


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

        std::string soundFilePath() override { return ""; }

        void setSoundFilePath(const std::string& filePath) {}

    private:
        int mPomodoroDuration = 25;
        int mShortBreakDuration = 5;
        int mLongBreakDuration = 15;
        int mTasksBeforeBreak = 4;
        bool mPlaySound;
        int mSoundVolume;
    };
};

TEST(PomodoroTimerStates, test_should_be_in_short_break_when_finishing_pomodoro)
{
    std::function<void(long)> callbackStub;
    TestConfig testSettings;
    int tick{1000};
    PomodoroTimer timer{callbackStub, tick, testSettings};
    // Should be in PomodoroState with 0 completed Pomodoros

    timer.finish();

    CHECK(timer.isBreak());
    CHECK_EQUAL(testSettings.shortBreakDuration(), timer.taskDuration());
}

TEST(PomodoroTimerStates,
     test_should_be_in_long_break_when_finishing_task_cycle)
{
    std::function<void(long)> callbackStub;
    TestConfig testSettings;
    int tick{1000};
    PomodoroTimer timer{callbackStub, tick, testSettings};
    timer.setNumCompletedPomodoros(3);

    timer.finish();

    CHECK(timer.isBreak());
    CHECK_EQUAL(testSettings.longBreakDuration(), timer.taskDuration());
}

TEST(PomodoroTimerStates, test_should_set_state_to_pomodoro_after_any_break)
{
    std::function<void(long)> callbackStub;
    TestConfig testSettings;
    int tick{1000};
    PomodoroTimer timer{callbackStub, tick, testSettings};
    timer.setNumCompletedPomodoros(3);

    timer.finish();
    timer.finish(); // Long break finished

    CHECK(!timer.isBreak());

    timer.finish();
    timer.finish(); // Short break finished

    CHECK(!timer.isBreak())
}

TEST(PomodoroTimerStates, test_should_be_in_task_state_when_cancelling_task)
{
    std::function<void(long)> callbackStub;
    TestConfig testSettings;
    int tick{1000};
    PomodoroTimer timer{callbackStub, tick, testSettings};
    timer.setNumCompletedPomodoros(3);

    timer.finish(); // Short break
    timer.cancel();

    CHECK(!timer.isBreak());
}

TEST(PomodoroTimerStates, test_should_be_in_task_state_when_cancelling_break)
{
    std::function<void(long)> callbackStub;
    TestConfig testSettings;
    int tick{1000};
    PomodoroTimer timer{callbackStub, tick, testSettings};

    timer.finish();
    timer.cancel();

    CHECK(!timer.isBreak());
}

TEST(PomodoroTimerStates,
     test_should_increment_completed_pomodoros_when_finishing_pomodoro)
{
    std::function<void(long)> callbackStub;
    TestConfig testSettings;
    int tick{1000};
    PomodoroTimer timer{callbackStub, tick, testSettings};
    timer.setNumCompletedPomodoros(2);

    timer.finish();

    CHECK_EQUAL(3, timer.numCompletedPomodoros());
}

TEST(PomodoroTimerStates,
     test_should_not_increment_completed_pomodoros_when_finishing_break)
{
    std::function<void(long)> callbackStub;
    TestConfig testSettings;
    int tick{1000};
    PomodoroTimer timer{callbackStub, tick, testSettings};
    timer.setNumCompletedPomodoros(2);

    timer.finish(); // Increment and transition to ShortBreak
    timer.finish(); // Should not increment and transition to Pomodoro

    CHECK_EQUAL(3, timer.numCompletedPomodoros());
}

TEST(PomodoroTimerStates,
     test_should_not_interleave_with_breaks_when_in_zone_state_active)
{
    std::function<void(long)> callbackStub;
    TestConfig testSettings;
    int tick{1000};
    PomodoroTimer timer{callbackStub, tick, testSettings};

    timer.toggleInTheZoneMode();

    for (size_t i = 0; i < 20; ++i) {
        timer.finish();
        CHECK(!timer.isBreak());
    }

    CHECK_EQUAL(20, timer.numCompletedPomodoros());
}
