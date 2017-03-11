/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef ICORESERVICE_H_XVOMGAES
#define ICORESERVICE_H_XVOMGAES

#include "core/Distribution.h"
#include "date_wrapper/TimeSpan.h"
#include "core/entities/Sprint.h"
#include <functional>
#include <string>
#include "core/external_io/ISink.h"


class ICoreService {
public:
    using TaskResultHandler = std::function<void(const std::vector<Task>&)>;
    using SprintResultHandler = std::function<void(const std::vector<Sprint>&)>;
    using TagResultHandler
        = std::function<void(const std::vector<std::string>&)>;
    using SprintEncodingFunc = std::function<std::string(const std::vector<Sprint>& sprints)>;
    using TaskEncodingFunc = std::function<std::string(const std::vector<Task>& task)>;

    virtual ~ICoreService() = default;

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

    virtual void exportTasks(const TimeSpan& timeSpan,
                             std::shared_ptr<ExternalIO::ISink> sink,
                             TaskEncodingFunc func) = 0;

    virtual void registerSprint(const TimeSpan& timeSpan,
                                const std::string& taskUuid)
        = 0;

    virtual void registerSprint(const Sprint& sprint) = 0;

    virtual void removeSprint(const Sprint& sprint) = 0;

    virtual void
    sprintsInTimeRange(const TimeSpan& timeSpan,
                       SprintResultHandler onResultsReceivedCallback)
        = 0;

    virtual void exportSprints(const TimeSpan& timeSpan,
                               std::shared_ptr<ExternalIO::ISink> sink,
                               SprintEncodingFunc func) = 0;

    virtual void yearRange(std::function<void(const std::vector<std::string>&)>
                               onResultsReceivedCallback)
        = 0;

    virtual void requestSprintDailyDistribution(
        const TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        = 0;

    virtual void requestSprintWeeklyDistribution(
        const TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        = 0;

    virtual void requestSprintMonthlyDistribution(
        const TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        = 0;

    virtual void requestAllTags(TagResultHandler onResultsReceivedCallback) = 0;

    virtual void editTag(const std::string& oldName, const std::string& newName)
        = 0;

    virtual std::string lastCommandDescription() const = 0;

    virtual unsigned long long numRevertableCommands() const = 0;

    virtual void undoLast() = 0;
};

#endif /* end of include guard: ICORESERVICE_H_XVOMGAES */
