#ifndef IPOMODOROSERVICE_H_XVOMGAES
#define IPOMODOROSERVICE_H_XVOMGAES

#include "core/Distribution.h"
#include "core/TimeSpan.h"
#include "core/entities/Pomodoro.h"
#include <functional>
#include <string>


class IPomodoroService {
public:
    virtual ~IPomodoroService() = default;

    virtual void registerTask(const TodoItem& task) = 0;

    virtual void removeTask(const TodoItem& task) = 0;

    virtual void editTask(const TodoItem& task, const TodoItem& editedTask) = 0;

    virtual void toggleTaskCompletionStatus(const TodoItem& task) = 0;

    virtual void registerTaskPriorities(
        std::vector<std::pair<std::string, int>>&& priorities)
        = 0;

    virtual void
    requestFinishedTasks(const TimeSpan& timeSpan,
                         std::function<void(const std::vector<TodoItem>&)>
                             onResultsReceivedCallback)
        = 0;

    virtual void
    requestUnfinishedTasks(std::function<void(const std::vector<TodoItem>&)>
                               onResultsReceivedCallback)
        = 0;

    virtual void registerPomodoro(const TimeSpan& timeSpan,
                                  const std::string& taskUuid)
        = 0;

    virtual void removePomodoro(const Pomodoro& pomodoro) = 0;

    virtual void
    pomodorosInTimeRange(const TimeSpan& timeSpan,
                         std::function<void(const std::vector<Pomodoro>&)>
                             onResultsReceivedCallback)
        = 0;

    virtual void
    pomodoroYearRange(std::function<void(const std::vector<std::string>&)>
                          onResultsReceivedCallback)
        = 0;

    virtual void requestPomodoroDailyDistribution(
        const TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        = 0;

    virtual void requestPomodoroWeeklyDistribution(
        const TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        = 0;

    virtual void requestPomodoroMonthlyDistribution(
        const TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        = 0;

    virtual void undoLast() = 0;
};

#endif /* end of include guard: IPOMODOROSERVICE_H_XVOMGAES */
