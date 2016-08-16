/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef IPOMODOROSERVICE_H_XVOMGAES
#define IPOMODOROSERVICE_H_XVOMGAES

#include "core/Distribution.h"
#include "core/TimeSpan.h"
#include "core/entities/Sprint.h"
#include <functional>
#include <string>


class IPomodoroService {
public:
    using TaskResultHandler = std::function<void(const std::vector<Task>&)>;
    using PomodoroResultHandler
        = std::function<void(const std::vector<Sprint>&)>;
    using TagResultHandler
        = std::function<void(const std::vector<std::string>&)>;

    virtual ~IPomodoroService() = default;

    virtual void registerTask(const Task& task) = 0;

    virtual void removeTask(const Task& task) = 0;

    virtual void editTask(const Task& task, const Task& editedTask) = 0;

    virtual void toggleTaskCompletionStatus(const Task& task) = 0;

    virtual void registerTaskPriorities(
        std::vector<std::pair<std::string, int>>&& priorities)
        = 0;

    virtual void
    requestFinishedTasks(const TimeSpan& timeSpan,
                         TaskResultHandler onResultsReceivedCallback)
        = 0;

    virtual void
    requestUnfinishedTasks(TaskResultHandler onResultsReceivedCallback)
        = 0;

    virtual void registerPomodoro(const TimeSpan& timeSpan,
                                  const std::string& taskUuid)
        = 0;

    virtual void removePomodoro(const Sprint& pomodoro) = 0;

    virtual void
    pomodorosInTimeRange(const TimeSpan& timeSpan,
                         PomodoroResultHandler onResultsReceivedCallback)
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

    virtual void requestAllTags(TagResultHandler onResultsReceivedCallback) = 0;

    virtual void editTag(const std::string& oldName, const std::string& newName)
        = 0;

    virtual std::string lastCommandDescription() const = 0;

    virtual size_t numRevertableCommands() const = 0;

    virtual void undoLast() = 0;
};

#endif /* end of include guard: IPOMODOROSERVICE_H_XVOMGAES */
