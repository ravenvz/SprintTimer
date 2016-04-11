#include "core/PomodoroTimerModeScheduler.h"
#include <TestHarness.h>


TEST_GROUP(PomodoroTimerModeSchedulerGroup)
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

        int numPomodorosBeforeBreak() const override { return mTasksBeforeBreak; }

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

    private:
        int mPomodoroDuration = 25;
        int mShortBreakDuration = 5;
        int mLongBreakDuration = 15;
        int mTasksBeforeBreak = 4;
        bool mPlaySound;
        int mSoundVolume;
    };
};

bool stateIsShortBreak(const PomodoroTimerModeScheduler& scheduler)
{
    return scheduler.mode()
        == PomodoroTimerModeScheduler::PomodoroTimerMode::ShortBreak;
}

bool stateIsLongBreak(const PomodoroTimerModeScheduler& scheduler)
{
    return scheduler.mode()
        == PomodoroTimerModeScheduler::PomodoroTimerMode::LongBreak;
}

bool stateIsTask(const PomodoroTimerModeScheduler& scheduler)
{
    return scheduler.mode()
        == PomodoroTimerModeScheduler::PomodoroTimerMode::Task;
}

TEST(PomodoroTimerModeSchedulerGroup,
     should_set_state_to_break_when_finishing_task)
{
    PomodoroTimerModeScheduler scheduler;
    scheduler.setNextMode();

    CHECK(scheduler.isBreak());
    CHECK(stateIsShortBreak(scheduler))
}

TEST(PomodoroTimerModeSchedulerGroup,
     should_set_state_to_long_break_when_finishing_task_cycle)
{
    PomodoroTimerModeScheduler scheduler;
    scheduler.setNumCompletedPomodoros(3);

    scheduler.setNextMode();

    CHECK(scheduler.isBreak())
    CHECK(stateIsLongBreak(scheduler))
}

TEST(PomodoroTimerModeSchedulerGroup,
     should_set_schedulable_to_task_when_finishing_break)
{
    PomodoroTimerModeScheduler scheduler;
    scheduler.setNumCompletedPomodoros(3);

    scheduler.setNextMode();
    scheduler.setNextMode();

    CHECK(!scheduler.isBreak())
    CHECK(stateIsTask(scheduler))
}

TEST(PomodoroTimerModeSchedulerGroup,
     should_set_schedulable_to_task_when_cancelling_task)
{
    PomodoroTimerModeScheduler scheduler;

    scheduler.cancelMode();

    CHECK(!scheduler.isBreak())
    CHECK(stateIsTask(scheduler))
}

TEST(PomodoroTimerModeSchedulerGroup,
     should_set_schedulable_to_task_when_cancelling_break)
{
    PomodoroTimerModeScheduler scheduler;

    scheduler.setNextMode();
    // Task completed, should be in break state
    scheduler.cancelMode();

    CHECK(!scheduler.isBreak())
    CHECK(stateIsTask(scheduler))
}

TEST(PomodoroTimerModeSchedulerGroup,
     should_increment_completed_pomodoros_counter_when_finishing_task)
{
    PomodoroTimerModeScheduler scheduler;
    scheduler.setNumCompletedPomodoros(2);

    scheduler.setNextMode();

    CHECK_EQUAL(3, scheduler.numCompletedPomodoros());
}

TEST(PomodoroTimerModeSchedulerGroup,
     should_not_increment_completed_pomodoros_counter_when_finishing_break)
{
    PomodoroTimerModeScheduler scheduler;
    scheduler.setNumCompletedPomodoros(2);

    scheduler.setNextMode();
    // Task completed numCompletedPomodoros should increment and transition to
    // the break state
    scheduler.setNextMode();
    // Break finished, numCompletedPomodoros should not increment

    CHECK_EQUAL(3, scheduler.numCompletedPomodoros());
}

TEST(PomodoroTimerModeSchedulerGroup,
     should_only_schedule_pomodoros_if_in_the_zone_mode_active)
{
    PomodoroTimerModeScheduler scheduler;

    scheduler.toggleInTheZoneMode();

    for (size_t i = 0; i < 20; ++i) {
        scheduler.setNextMode();
        CHECK(!scheduler.isBreak());
    }

    CHECK_EQUAL(20, scheduler.numCompletedPomodoros())
}
