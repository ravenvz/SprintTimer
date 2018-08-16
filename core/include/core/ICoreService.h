/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include "core/Observer.h"
#include "core/entities/Task.h"
#include "core/external_io/ISink.h"
#include "date_wrapper/TimeSpan.h"
#include <functional>
#include <string>

// TODO this interface should be torn apart

namespace sprint_timer {

class ICoreService {
public:
    using TaskResultHandler
        = std::function<void(const std::vector<entities::Task>&)>;
    using SprintResultHandler
        = std::function<void(const std::vector<entities::Sprint>&)>;
    using TagResultHandler
        = std::function<void(const std::vector<std::string>&)>;
    using SprintEncodingFunc = std::function<std::string(
        const std::vector<entities::Sprint>& sprints)>;
    using TaskEncodingFunc
        = std::function<std::string(const std::vector<entities::Task>& task)>;
    using TaskOrder = std::vector<std::string>;

    virtual ~ICoreService() = default;

    virtual void registerTask(const entities::Task& task) = 0;

    virtual void removeTask(const entities::Task& task) = 0;

    virtual void editTask(const entities::Task& task,
                          const entities::Task& editedTask)
        = 0;

    virtual void toggleTaskCompletionStatus(const entities::Task& task) = 0;

    virtual void registerTaskPriorities(TaskOrder&& old_order,
                                        TaskOrder&& new_order)
        = 0;

    virtual void
    requestFinishedTasks(const dw::TimeSpan& timeSpan,
                         TaskResultHandler onResultsReceivedCallback)
        = 0;

    virtual void exportTasks(const dw::TimeSpan& timeSpan,
                             std::shared_ptr<external_io::ISink> sink,
                             TaskEncodingFunc func)
        = 0;

    virtual void registerSprint(const dw::TimeSpan& timeSpan,
                                const std::string& taskUuid)
        = 0;

    virtual void registerSprint(const entities::Sprint& sprint) = 0;

    virtual void removeSprint(const entities::Sprint& sprint) = 0;

    virtual void
    sprintsInTimeRange(const dw::TimeSpan& timeSpan,
                       SprintResultHandler onResultsReceivedCallback)
        = 0;

    virtual void exportSprints(const dw::TimeSpan& timeSpan,
                               std::shared_ptr<external_io::ISink> sink,
                               SprintEncodingFunc func)
        = 0;

    virtual void yearRange(std::function<void(const std::vector<std::string>&)>
                               onResultsReceivedCallback)
        = 0;

    virtual void requestSprintDailyDistribution(
        const dw::TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        = 0;

    virtual void requestSprintWeeklyDistribution(
        const dw::TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        = 0;

    virtual void requestSprintMonthlyDistribution(
        const dw::TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        = 0;

    virtual void requestAllTags(TagResultHandler onResultsReceivedCallback) = 0;

    virtual void editTag(const std::string& oldName, const std::string& newName)
        = 0;

    virtual std::string lastCommandDescription() const = 0;

    virtual void registerUndoObserver(Observer& observer) = 0;

    virtual uint64_t numRevertableCommands() const = 0;

    virtual void undoLast() = 0;

    virtual void requestSprintsForTask(
        const std::string& taskUuid,
        std::function<void(const std::vector<entities::Sprint>&)>
            onResultsReceivedCallback)
        = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: ICORESERVICE_H_XVOMGAES */
