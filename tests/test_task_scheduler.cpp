#include "core/TaskScheduler.h"
#include <TestHarness.h>


TEST_GROUP(TaskSchedulerGroup) {

    class TestConfig : public IConfig
    {

        public:

            TestConfig() {

            }

            int soundVolume() const override {
                return mSoundVolume;
            }

            void setSoundVolume(int soundVolume) override {
                TestConfig::mSoundVolume = soundVolume;
            }

            int pomodoroDuration() const override {
                return mPomodoroDuration;
            }

            void setPomodoroDuration(int pomodoroDuration) override {
                TestConfig::mPomodoroDuration = pomodoroDuration;
            }

            int shortBreakDuration() const override {
                return mShortBreakDuration;
            }

            void setShortBreakDuration(int shortBreakDuration) override {
                TestConfig::mShortBreakDuration = shortBreakDuration;
            }

            int longBreakDuration() const override {
                return mLongBreakDuration;
            }

            void setLongBreakDuration(int longBreakDuration) override {
                TestConfig::mLongBreakDuration = longBreakDuration;
            }

            int numTasksBeforeBreak() const override {
                return mTasksBeforeBreak;
            }

            void setTasksBeforeBreak(int tasksBeforeBreak) override {
                TestConfig::mTasksBeforeBreak = tasksBeforeBreak;
            }

            bool soundIsEnabled() const override {
                return mPlaySound;
            }

            void setPlaySound(bool playSound) override {
                TestConfig::mPlaySound = playSound;
            }

            int dailyPomodorosGoal() const override {
                return 0;
            }

            void setDailyPomodorosGoal(int dailyPomodorosGoal) override {
            }

            int weeklyPomodorosGoal() const override {
                return 0;
            }

            void setWeeklyPomodorosGoal(int weeklyPomodorosGoal) override {
            }

            int monthlyPomodorosGoal() const override {
                return 0;
            }

            void setMonthlyPomodorosGoal(int monthlyPomodorosGoal) override {
            }

        private:
            int mPomodoroDuration = 25;
            int mShortBreakDuration = 5;
            int mLongBreakDuration = 15;
            int mTasksBeforeBreak = 4;
            bool mPlaySound;
            int mSoundVolume;
    };

};

TEST(TaskSchedulerGroup, should_set_state_to_break_when_finishing_task) {
    TestConfig testConfig;
    TaskScheduler scheduler {testConfig};
    scheduler.startTask();
    scheduler.finishTask();

    CHECK(scheduler.isBreak());
    CHECK_EQUAL(testConfig.shortBreakDuration(), scheduler.taskDurationInMinutes());
}

TEST(TaskSchedulerGroup, should_set_state_to_long_break_when_finishing_task_cycle) {
    TestConfig testConfig;
    TaskScheduler scheduler {testConfig};
    scheduler.setNumCompletedTasks(3);

    scheduler.startTask();
    scheduler.finishTask();

    CHECK_EQUAL(testConfig.longBreakDuration(), scheduler.taskDurationInMinutes());
}

TEST(TaskSchedulerGroup, should_set_schedulable_to_task_when_finishing_break) {
    TestConfig testConfig;
    TaskScheduler scheduler {testConfig};
    scheduler.setNumCompletedTasks(3);

    scheduler.startTask();
    scheduler.finishTask();
    scheduler.startTask();
    scheduler.finishTask();

    CHECK_EQUAL(testConfig.pomodoroDuration(), scheduler.taskDurationInMinutes());
}

TEST(TaskSchedulerGroup, should_set_schedulable_to_task_when_cancelling_task) {
    TestConfig testConfig;
    TaskScheduler scheduler {testConfig};

    scheduler.startTask();
    scheduler.cancelTask();

    CHECK_EQUAL(testConfig.pomodoroDuration(), scheduler.taskDurationInMinutes());
}

TEST(TaskSchedulerGroup, should_set_schedulable_to_task_when_cancelling_break) {
    TestConfig testConfig;
    TaskScheduler scheduler {testConfig};

    scheduler.startTask();
    scheduler.finishTask();
    // Task completed, should start break
    scheduler.startTask();
    scheduler.cancelTask();

    CHECK_EQUAL(testConfig.pomodoroDuration(), scheduler.taskDurationInMinutes());
}
TEST(TaskSchedulerGroup, should_increment_completed_tasks_counter_when_finishing_task) {
    TestConfig testConfig;
    TaskScheduler scheduler {testConfig};
    scheduler.setNumCompletedTasks(2);

    scheduler.startTask();
    scheduler.finishTask();

    CHECK_EQUAL(3, scheduler.numCompletedTasks());
}

TEST(TaskSchedulerGroup, should_not_increment_completed_tasks_counter_when_finishing_break) {
    TestConfig testConfig;
    TaskScheduler scheduler {testConfig};
    scheduler.setNumCompletedTasks(2);

    scheduler.startTask();
    scheduler.finishTask();
    // Task completed numCompletedTasks should increment, should start break
    scheduler.startTask();
    scheduler.finishTask();
    // Break finished, numCompletedTasks should not increment

    CHECK_EQUAL(3, scheduler.numCompletedTasks());
}

TEST(TaskSchedulerGroup, should_only_schedule_tasks_if_in_the_zone_mode_active) {
    TestConfig testConfig;
    TaskScheduler scheduler {testConfig};

    scheduler.toggleInTheZoneMode();

    for (size_t i = 0; i < 20; ++i) {
        scheduler.startTask();
        scheduler.finishTask();
        CHECK(!scheduler.isBreak());
    }
}
