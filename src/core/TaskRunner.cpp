#include "TaskRunner.h"

TaskRunner::TaskRunner(std::function<void(long timeLeft)> tickCallback,
                       long updateIntervalInMilliseconds,
                       const IConfig& applicationSettings)
    : applicationSettings{applicationSettings}
    , taskScheduler{}
    , tickInterval{updateIntervalInMilliseconds}
    , onTickCallback{tickCallback}
{
}

void TaskRunner::startTask()
{
    using namespace date;
    start = std::chrono::system_clock::now();
    std::cout << start << std::endl;
    timerPtr = std::make_unique<Timer>(
        std::bind(&TaskRunner::onTimerTick, this), tickInterval);
    timerPtr->start();
    currentTaskDuration
        = std::chrono::milliseconds{taskDuration() * millisecondsInMinute};
    std::cout << "Current task duration " << currentTaskDuration.count()
              << std::endl;
}

void TaskRunner::cancelTask()
{
    timerPtr->stop();
    taskScheduler.cancelState();
}

TimeInterval TaskRunner::finishTask()
{
    taskScheduler.setNextState();
    std::cout << "Current task is break: " << isBreak() << std::endl;
    return TimeInterval{QDateTime::fromTime_t(static_cast<unsigned>(
                            std::chrono::system_clock::to_time_t(start))),
                        QDateTime::fromTime_t(static_cast<unsigned>(
                            std::chrono::system_clock::to_time_t(finish)))};
}

int TaskRunner::taskDuration()
{
    switch (taskScheduler.state()) {
    case TaskScheduler::TaskState::Task:
        return applicationSettings.pomodoroDuration();
    case TaskScheduler::TaskState::LongBreak:
        return applicationSettings.longBreakDuration();
    case TaskScheduler::TaskState::ShortBreak:
        return applicationSettings.shortBreakDuration();
    default:
        return applicationSettings.shortBreakDuration();
    }
}

bool TaskRunner::isBreak() const { return taskScheduler.isBreak(); }

void TaskRunner::onTimerTick()
{
    currentTaskDuration -= tickInterval;
    std::cout << currentTaskDuration.count() << std::endl;
    if (currentTaskDuration.count() == 0) {
        std::cout << "Timer stopped normally" << std::endl;
        finish = std::chrono::system_clock::now();
        timerPtr->stop();
    }
    onTickCallback(currentTaskDuration.count());
}
