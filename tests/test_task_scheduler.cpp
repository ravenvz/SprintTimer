#include "core/TaskScheduler.h"
#include <TestHarness.h>


TEST_GROUP(TaskSchedulerGroup)
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

        int numTasksBeforeBreak() const override { return mTasksBeforeBreak; }

        void setTasksBeforeBreak(int tasksBeforeBreak) override
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

bool stateIsShortBreak(const TaskScheduler& scheduler)
{
    return scheduler.state() == TaskScheduler::TaskState::ShortBreak;
}

bool stateIsLongBreak(const TaskScheduler& scheduler)
{
    return scheduler.state() == TaskScheduler::TaskState::LongBreak;
}

bool stateIsTask(const TaskScheduler& scheduler)
{
    return scheduler.state() == TaskScheduler::TaskState::Task;
}

TEST(TaskSchedulerGroup, should_set_state_to_break_when_finishing_task)
{
    TaskScheduler scheduler;
    scheduler.setNextState();

    CHECK(scheduler.isBreak());
    CHECK(stateIsShortBreak(scheduler))
}

TEST(TaskSchedulerGroup,
     should_set_state_to_long_break_when_finishing_task_cycle)
{
    TaskScheduler scheduler;
    scheduler.setNumCompletedTasks(3);

    scheduler.setNextState();

    CHECK(scheduler.isBreak())
    CHECK(stateIsLongBreak(scheduler))
}

TEST(TaskSchedulerGroup, should_set_schedulable_to_task_when_finishing_break)
{
    TaskScheduler scheduler;
    scheduler.setNumCompletedTasks(3);

    scheduler.setNextState();
    scheduler.setNextState();

    CHECK(!scheduler.isBreak())
    CHECK(stateIsTask(scheduler))
}

TEST(TaskSchedulerGroup, should_set_schedulable_to_task_when_cancelling_task)
{
    TaskScheduler scheduler;

    scheduler.cancelState();

    CHECK(!scheduler.isBreak())
    CHECK(stateIsTask(scheduler))
}

TEST(TaskSchedulerGroup, should_set_schedulable_to_task_when_cancelling_break)
{
    TaskScheduler scheduler;

    scheduler.setNextState();
    // Task completed, should be in break state
    scheduler.cancelState();

    CHECK(!scheduler.isBreak())
    CHECK(stateIsTask(scheduler))
}

TEST(TaskSchedulerGroup,
     should_increment_completed_tasks_counter_when_finishing_task)
{
    TaskScheduler scheduler;
    scheduler.setNumCompletedTasks(2);

    scheduler.setNextState();

    CHECK_EQUAL(3, scheduler.numCompletedTasks());
}

TEST(TaskSchedulerGroup,
     should_not_increment_completed_tasks_counter_when_finishing_break)
{
    TaskScheduler scheduler;
    scheduler.setNumCompletedTasks(2);

    scheduler.setNextState();
    // Task completed numCompletedTasks should increment and transition to
    // the break state
    scheduler.setNextState();
    // Break finished, numCompletedTasks should not increment

    CHECK_EQUAL(3, scheduler.numCompletedTasks());
}

TEST(TaskSchedulerGroup, should_only_schedule_tasks_if_in_the_zone_mode_active)
{
    TaskScheduler scheduler;

    scheduler.toggleInTheZoneMode();

    for (size_t i = 0; i < 20; ++i) {
        scheduler.setNextState();
        CHECK(!scheduler.isBreak());
    }

    CHECK_EQUAL(20, scheduler.numCompletedTasks())
}
