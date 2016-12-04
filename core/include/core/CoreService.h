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
#ifndef CORESERVICE_H_JXK8PKAI
#define CORESERVICE_H_JXK8PKAI


#include "core/CommandInvoker.h"
#include "core/ISprintDistributionReader.h"
#include "core/ICoreService.h"
#include "core/ISprintStorageReader.h"
#include "core/ISprintStorageWriter.h"
#include "core/IYearRangeReader.h"
#include "core/ITaskStorageReader.h"
#include "core/ITaskStorageWriter.h"
#include "core/MacroTransaction.h"
#include <memory>

namespace Core {

class CoreService : public ICoreService {
public:
    CoreService(ISprintStorageReader& sprintStorageReader,
                    ISprintStorageWriter& sprintStorageWriter,
                    IYearRangeReader& yearRangeReader,
                    ITaskStorageReader& taskStorageReader,
                    ITaskStorageWriter& taskStorageWriter,
                    ISprintDistributionReader& sprintDailyDistributionReader,
                    ISprintDistributionReader& sprintWeeklyDistributionReader,
                    ISprintDistributionReader& sprintMonthlyDistributionReader);

    void registerTask(const Task& task) final;

    void removeTask(const Task& task) final;

    void editTask(const Task& task, const Task& editedTask) final;

    void toggleTaskCompletionStatus(const Task& task) final;

    void registerTaskPriorities(
        std::vector<std::pair<std::string, int>>&& priorities) final;

    void requestFinishedTasks(const TimeSpan& timeSpan,
                              std::function<void(const std::vector<Task>&)>
                                  onResultsReceivedCallback) final;

    void
    requestUnfinishedTasks(std::function<void(const std::vector<Task>&)>
                               onResultsReceivedCallback) final;

    void registerSprint(const TimeSpan& timeSpan,
                        const std::string& taskUuid) final;

    void registerSprint(const Sprint& sprint) final;

    void removeSprint(const Sprint& sprint) final;

    void sprintsInTimeRange(const TimeSpan& timeSpan,
                            std::function<void(const std::vector<Sprint>&)>
                            onResultsReceivedCallback) final;

    void yearRange(std::function<void(const std::vector<std::string>&)>
                   onResultsReceivedCallback) final;

    void requestSprintDailyDistribution(
            const TimeSpan& timeSpan,
            std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        final;

    void requestSprintWeeklyDistribution(
            const TimeSpan& timeSpan,
            std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        final;

    void requestSprintMonthlyDistribution(
            const TimeSpan& timeSpan,
            std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        final;

    void requestAllTags(TagResultHandler onResultsReceivedCallback) final;

    void editTag(const std::string& oldName, const std::string& newName) final;

    virtual std::string lastCommandDescription() const final;

    virtual unsigned long long numRevertableCommands() const final;

    void undoLast() final;

private:
    ISprintStorageReader& sprintReader;
    ISprintStorageWriter& sprintWriter;
    IYearRangeReader& yearRangeReader;
    ITaskStorageReader& taskReader;
    ITaskStorageWriter& taskWriter;
    ISprintDistributionReader& sprintDailyDistributionReader;
    ISprintDistributionReader& sprintWeeklyDistributionReader;
    ISprintDistributionReader& sprintMonthlyDistributionReader;
    CommandInvoker invoker;
};


} // namespace Core

#endif /* end of include guard: CORESERVICE_H_JXK8PKAI */
