#ifndef TASKRUNNER_H_9VSDY5UR
#define TASKRUNNER_H_9VSDY5UR

#include "TaskScheduler.h"
#include "Timer.h"
#include "config.h"
#include "date/date.h"
#include "TimeSpan.h"
#include <chrono>
#include <memory>

/* Responsible for running stopwatch by managing timer.
 *
 * Stopwatch initial time depends on type */
class TaskRunner {
public:
    /* Construct TaskRunner given callback function, update
     * timeSpan and reference to application settings.
     *
     * When task is running, callback function will be executed after
     * each update timeSpan until time for the task runs out or task
     * is cancelled. */
    TaskRunner(std::function<void(long timeLeft)> tickCallback,
               long updateIntervalInMilliseconds,
               const IConfig& applicationSettings);

    ~TaskRunner() = default;

    TaskRunner(TaskRunner&&) = delete;
    TaskRunner& operator=(TaskRunner&&) = delete;

    TaskRunner(const TaskRunner&) = delete;
    TaskRunner& operator=(const TaskRunner&) = delete;

    /* Start task.
     *
     * tickCallback function will be called each tick until time for the task
     * runs out or cancelTask() method is called. */
    void startTask();

    /* Cancel currently running task and stop timer. */
    void cancelTask();

    /* Return TimeSpan for last task that had been completed normally
     * (had not been cancelled).
     *
     * Note that timeSpan's finish time will be recorded only after this method
     * is called. */
    TimeSpan finishTask();

    /* Return task duration in minutes.
     *
     * Duration is obtained from application config. */
    int taskDuration();

    /* Return true if current task type is TaskScheduler::ShortBrake or
     * TaskScheduler::LongBreak and false otherwise. */
    bool isBreak() const;

    /* Toggles the TaskScheduler mode when it stops interleaving tasks with
     * breaks and tasks are scheduled one after other without breaks. */
    void toggleInTheZoneMode();

private:
    const IConfig& applicationSettings;
    TaskScheduler taskScheduler;
    const std::chrono::milliseconds tickInterval;
    std::function<void(long timeLeft)> onTickCallback;
    std::unique_ptr<Timer> timerPtr;
    DateTime start;
    DateTime finish;
    std::chrono::milliseconds currentTaskDuration;
    const int millisecondsInMinute{60000};

    void onTimerTick();
};

#endif /* end of include guard: TASKRUNNER_H_9VSDY5UR */
