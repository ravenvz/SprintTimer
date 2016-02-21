#include "core/TaskScheduler.h"
#include <TestHarness.h>


TEST_GROUP(TaskSchedulerGroup) {

    class TestConfig : public IConfig
    {

        public:

            TestConfig() {

            }

            int getSoundVolume() const override {
                return soundVolume;
            }

            void setSoundVolume(int soundVolume) override {
                TestConfig::soundVolume = soundVolume;
            }

            unsigned int getPomodoroDuration() const override {
                return pomodoroDuration;
            }

            void setPomodoroDuration(unsigned int pomodoroDuration) override {
                TestConfig::pomodoroDuration = pomodoroDuration;
            }

            unsigned int getShortBreakDuration() const override {
                return shortBreakDuration;
            }

            void setShortBreakDuration(unsigned int shortBreakDuration) override {
                TestConfig::shortBreakDuration = shortBreakDuration;
            }

            unsigned int getLongBreakDuration() const override {
                return longBreakDuration;
            }

            void setLongBreakDuration(unsigned int longBreakDuration) override {
                TestConfig::longBreakDuration = longBreakDuration;
            }

            unsigned int getTasksBeforeBreak() const override {
                return tasksBeforeBreak;
            }

            void setTasksBeforeBreak(unsigned int tasksBeforeBreak) override {
                TestConfig::tasksBeforeBreak = tasksBeforeBreak;
            }

            bool soundIsEnabled() const override {
                return playSound;
            }

            void setPlaySound(bool playSound) override {
                TestConfig::playSound = playSound;
            }

            unsigned int getDailyPomodorosGoal() const override {
                return 0;
            }

            void setDailyPomodorosGoal(unsigned dailyPomodorosGoal) override {
            }

            unsigned int getWeeklyPomodorosGoal() const override {
                return 0;
            }

            void setWeeklyPomodorosGoal(unsigned weeklyPomodorosGoal) override {
            }

            unsigned int getMonthlyPomodorosGoal() const override {
                return 0;
            }

            void setMonthlyPomodorosGoal(unsigned monthlyPomodorosGoal) override {
            }

        private:
            unsigned pomodoroDuration = 25;
            unsigned shortBreakDuration = 5;
            unsigned longBreakDuration = 15;
            unsigned tasksBeforeBreak = 4;
            bool playSound;
            int soundVolume;
    };

};

TEST(TaskSchedulerGroup, should_set_state_to_break_when_finishing_task) {
    TestConfig testConfig;
    TaskScheduler scheduler {&testConfig};
    scheduler.startTask();
    scheduler.finishTask();

    CHECK(scheduler.isBreak());
    CHECK_EQUAL(testConfig.getShortBreakDuration(), scheduler.getTaskDurationInMinutes());
}

TEST(TaskSchedulerGroup, should_set_state_to_long_break_when_finishing_task_cycle) {
    TestConfig testConfig;
    TaskScheduler scheduler {&testConfig};
    scheduler.setNumCompletedTasks(3);

    scheduler.startTask();
    scheduler.finishTask();

    CHECK_EQUAL(testConfig.getLongBreakDuration(), scheduler.getTaskDurationInMinutes());
}

TEST(TaskSchedulerGroup, should_set_schedulable_to_task_when_finishing_break) {
    TestConfig testConfig;
    TaskScheduler scheduler {&testConfig};
    scheduler.setNumCompletedTasks(3);

    scheduler.startTask();
    scheduler.finishTask();
    scheduler.startTask();
    scheduler.finishTask();

    CHECK_EQUAL(testConfig.getPomodoroDuration(), scheduler.getTaskDurationInMinutes());
}

TEST(TaskSchedulerGroup, should_set_schedulable_to_task_when_cancelling_task) {
    TestConfig testConfig;
    TaskScheduler scheduler {&testConfig};

    scheduler.startTask();
    scheduler.cancelTask();

    CHECK_EQUAL(testConfig.getPomodoroDuration(), scheduler.getTaskDurationInMinutes());
}

TEST(TaskSchedulerGroup, should_set_schedulable_to_task_when_cancelling_break) {
    TestConfig testConfig;
    TaskScheduler scheduler {&testConfig};

    scheduler.startTask();
    scheduler.finishTask();
    // Task completed, should start break
    scheduler.startTask();
    scheduler.cancelTask();

    CHECK_EQUAL(testConfig.getPomodoroDuration(), scheduler.getTaskDurationInMinutes());
}
TEST(TaskSchedulerGroup, should_increment_completed_tasks_counter_when_finishing_task) {
    TestConfig testConfig;
    TaskScheduler scheduler {&testConfig};
    scheduler.setNumCompletedTasks(2);

    scheduler.startTask();
    scheduler.finishTask();

    CHECK_EQUAL(3, scheduler.numCompletedTasks());
}

TEST(TaskSchedulerGroup, should_not_increment_completed_tasks_counter_when_finishing_break) {
    TestConfig testConfig;
    TaskScheduler scheduler {&testConfig};
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
    TaskScheduler scheduler {&testConfig};

    scheduler.toggleInTheZoneMode();

    for (size_t i = 0; i < 20; ++i) {
        scheduler.startTask();
        scheduler.finishTask();
        CHECK(!scheduler.isBreak());
    }
}
