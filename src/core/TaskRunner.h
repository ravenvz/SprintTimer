#ifndef TASKRUNNER_H_9VSDY5UR
#define TASKRUNNER_H_9VSDY5UR

#include <chrono>
#include <memory>
#include "config.h"
#include "Timer.h"
#include "TaskScheduler.h"
#include "timeinterval.h"
#include "date/date.h"

class TaskRunner {
public:
    using SystemTime = std::chrono::time_point<std::chrono::system_clock>;

    TaskRunner(std::function<void(long timeLeft)> tickCallback,
               long updateIntervalInMilliseconds,
               const IConfig& applicationSettings);
    ~TaskRunner() = default;

    TaskRunner(TaskRunner&&) = delete;
    TaskRunner& operator=(TaskRunner&&) = delete;

    TaskRunner(const TaskRunner&) = delete;
    TaskRunner& operator=(const TaskRunner&) = delete;

    void startTask();

    void cancelTask();

    TimeInterval finishTask();

    int taskDuration();

    bool isBreak() const;

    void toggleInTheZoneMode();

private:
    const IConfig& applicationSettings;
    TaskScheduler taskScheduler;
    const std::chrono::milliseconds tickInterval;
    std::function<void(long timeLeft)> onTickCallback;
    std::unique_ptr<Timer> timerPtr;
    SystemTime start;
    SystemTime finish;
    std::chrono::milliseconds currentTaskDuration;
    const int millisecondsInMinute{60000};

    void onTimerTick();
};

#endif /* end of include guard: TASKRUNNER_H_9VSDY5UR */
