#include "core/TaskScheduler.h"
#include <TestHarness.h>

TEST_GROUP(TaskSchedulerGroup) {
    const unsigned taskDuration = 25;
    const unsigned shortBreakDuration = 5;
    const unsigned longBreakDuration = 15;
    const unsigned tasksBeforeBreak = 4;

    Config defaultConfig {taskDuration, shortBreakDuration, longBreakDuration, tasksBeforeBreak};
};

TEST(TaskSchedulerGroup, should_set_state_to_break_when_finishing_task) {
    unsigned numCompleted = 0;
    TaskScheduler defaultScheduler {defaultConfig, numCompleted};
    defaultScheduler.startTask();
    defaultScheduler.finishTask();

    CHECK(defaultScheduler.isBreak());
    CHECK_EQUAL(shortBreakDuration, defaultScheduler.getTaskDurationInMinutes());
}

TEST(TaskSchedulerGroup, should_set_state_to_long_break_when_finishing_task_cycle) {
    unsigned numCompleted = 3;
    TaskScheduler defaultScheduler {defaultConfig, numCompleted};
    defaultScheduler.startTask();
    defaultScheduler.finishTask();

    CHECK_EQUAL(longBreakDuration, defaultScheduler.getTaskDurationInMinutes());
}

TEST(TaskSchedulerGroup, should_set_schedulable_to_task_when_finishing_break) {
    unsigned numCompleted = 3;
    TaskScheduler defaultScheduler {defaultConfig, numCompleted};
    defaultScheduler.startTask();
    defaultScheduler.finishTask();
    defaultScheduler.startTask();
    defaultScheduler.finishTask();

    CHECK_EQUAL(taskDuration, defaultScheduler.getTaskDurationInMinutes());
}

TEST(TaskSchedulerGroup, should_set_schedulable_to_task_when_cancelling_task) {
    unsigned numCompleted = 0;
    TaskScheduler defaultScheduler {defaultConfig, numCompleted};
    defaultScheduler.startTask();
    defaultScheduler.cancelTask();

    CHECK_EQUAL(taskDuration, defaultScheduler.getTaskDurationInMinutes());
}

TEST(TaskSchedulerGroup, should_set_schedulable_to_task_when_cancelling_break) {
    unsigned numCompleted = 0;
    TaskScheduler defaultScheduler {defaultConfig, numCompleted};
    defaultScheduler.startTask();
    defaultScheduler.finishTask();
    // Task completed, should start break
    defaultScheduler.startTask();
    defaultScheduler.cancelTask();

    CHECK_EQUAL(taskDuration, defaultScheduler.getTaskDurationInMinutes());
}
TEST(TaskSchedulerGroup, should_increment_completed_tasks_counter_when_finishing_task) {
    unsigned numCompleted = 2;
    TaskScheduler defaultScheduler {defaultConfig, numCompleted};
    defaultScheduler.startTask();
    defaultScheduler.finishTask();

    CHECK_EQUAL(3, defaultScheduler.getNumCompletedTasks());
}

TEST(TaskSchedulerGroup, should_not_increment_completed_tasks_counter_when_finishing_break) {
    unsigned numCompleted = 2;
    TaskScheduler defaultScheduler {defaultConfig, numCompleted};
    defaultScheduler.startTask();
    defaultScheduler.finishTask();
    // Task completed numCompletedTasks should increment, should start break
    defaultScheduler.startTask();
    defaultScheduler.finishTask();
    // Break finished, numCompletedTasks should not increment

    CHECK_EQUAL(3, defaultScheduler.getNumCompletedTasks());
}

TEST(TaskSchedulerGroup, should_only_schedule_tasks_if_in_the_zone_mode_active) {
    unsigned numCompleted = 0;
    TaskScheduler defaultScheduler {defaultConfig, numCompleted};
    defaultScheduler.toggleInTheZoneMode();
    for (size_t i = 0; i < 20; ++i) {
        defaultScheduler.startTask();
        defaultScheduler.finishTask();
        CHECK(!defaultScheduler.isBreak());
    }
}
