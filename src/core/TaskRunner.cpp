#include "TaskRunner.h"

TaskRunner::TaskRunner(std::function<void(long timeLeft)> tickCallback,
                       long updateIntervalInMilliseconds,
                       const IConfig& applicationSettings)
    : applicationSettings{applicationSettings}
    , taskScheduler{}
    , tickInterval{updateIntervalInMilliseconds}
    , onTickCallback{tickCallback}
    , start{std::chrono::system_clock::time_point{}}
    , finish{std::chrono::system_clock::time_point{}}
{
}

void TaskRunner::startTask()
{
    using namespace date;
    start = DateTime::currentDateTime();
    timerPtr = std::make_unique<Timer>(
        std::bind(&TaskRunner::onTimerTick, this), tickInterval);
    timerPtr->start();
    currentTaskDuration
        = std::chrono::milliseconds{taskDuration() * millisecondsInMinute};
}

void TaskRunner::cancelTask()
{
    timerPtr->stop();
    taskScheduler.cancelState();
}

TimeInterval TaskRunner::finishTask()
{
    taskScheduler.setNextState();
    return TimeInterval{start, finish};
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
    if (currentTaskDuration.count() == 0) {
        finish = DateTime::currentDateTime();
        timerPtr->stop();
    }
    onTickCallback(currentTaskDuration.count());
}

void TaskRunner::toggleInTheZoneMode() { taskScheduler.toggleInTheZoneMode(); }
