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
#include "core/CoreService.h"
#include "core/use_cases/AddNewTask.h"
#include "core/use_cases/DecrementTaskSprints.h"
#include "core/use_cases/DeleteTask.h"
#include "core/use_cases/EditTask.h"
#include "core/use_cases/IncrementTaskSprints.h"
#include "core/use_cases/RegisterNewSprint.h"
#include "core/use_cases/RemoveSprintTransaction.h"
#include "core/use_cases/RenameTag.h"
#include "core/use_cases/RequestAllTags.h"
#include "core/use_cases/RequestFinishedTasks.h"
#include "core/use_cases/RequestMinMaxYear.h"
#include "core/use_cases/RequestSprintDistribution.h"
#include "core/use_cases/RequestSprints.h"
#include "core/use_cases/RequestTasks.h"
#include "core/use_cases/RequestUnfinishedTasks.h"
#include "core/use_cases/StoreUnfinishedTasksOrder.h"
#include "core/use_cases/ToggleTaskCompletionStatus.h"

using dw::DateTime;
using dw::TimeSpan;

using namespace core;
using namespace core::use_cases; // TODO remove when all this is inside that
                                 // namespace

namespace Core {

CoreService::CoreService(
    ISprintStorageReader& sprintStorageReader,
    ISprintStorageWriter& sprintStorageWriter,
    IYearRangeReader& yearRangeReader,
    ITaskStorageReader& taskStorageReader,
    ITaskStorageWriter& taskStorageWriter,
    ISprintDistributionReader& sprintDailyDistributionReader,
    ISprintDistributionReader& sprintWeeklyDistributionReader,
    ISprintDistributionReader& sprintMonthlyDistributionReader)
    : sprintReader{sprintStorageReader}
    , sprintWriter{sprintStorageWriter}
    , yearRangeReader{yearRangeReader}
    , taskReader{taskStorageReader}
    , taskWriter{taskStorageWriter}
    , sprintDailyDistributionReader{sprintDailyDistributionReader}
    , sprintWeeklyDistributionReader{sprintWeeklyDistributionReader}
    , sprintMonthlyDistributionReader{sprintMonthlyDistributionReader}
{
}

void CoreService::registerTask(const Task& task)
{
    auto addTask = std::make_unique<AddNewTask>(taskWriter, task);
    invoker.executeCommand(std::move(addTask));
}

void CoreService::removeTask(const Task& task)
{
    auto deleteTask = std::make_unique<DeleteTask>(taskWriter, task);
    invoker.executeCommand(std::move(deleteTask));
}

void CoreService::editTask(const Task& task, const Task& editedTask)
{
    auto editTask = std::make_unique<EditTask>(taskWriter, task, editedTask);
    invoker.executeCommand(std::move(editTask));
}

void CoreService::toggleTaskCompletionStatus(const Task& task)
{
    auto toggleTaskCommand
        = std::make_unique<ToggleTaskCompletionStatus>(taskWriter, task);
    invoker.executeCommand(std::move(toggleTaskCommand));
}

void CoreService::registerTaskPriorities(TaskOrder&& old_order,
                                         TaskOrder&& new_order)
{
    auto registerPrioritiesCommand
        = std::make_unique<StoreUnfinishedTasksOrder>(
            taskWriter, std::move(old_order), std::move(new_order));
    invoker.executeCommand(std::move(registerPrioritiesCommand));
}

void CoreService::requestFinishedTasks(
    const TimeSpan& timeSpan,
    std::function<void(const std::vector<Task>&)> onResultsReceivedCallback)
{
    std::unique_ptr<Query> requestItems
        = std::make_unique<RequestFinishedTasks>(
            taskReader, timeSpan, onResultsReceivedCallback);
    query_invoker.executeQuery(std::move(requestItems));
}

void CoreService::requestUnfinishedTasks(
    std::function<void(const std::vector<Task>&)> onResultsReceivedCallback)
{
    auto requestItems = std::make_unique<RequestUnfinishedTasks>(
        taskReader, onResultsReceivedCallback);
    query_invoker.executeQuery(std::move(requestItems));
}

void CoreService::exportTasks(const TimeSpan& timeSpan,
                              std::shared_ptr<ExternalIO::ISink> sink,
                              ICoreService::TaskEncodingFunc func)
{
    auto requestItems = std::make_unique<RequestTasks>(
        taskReader, timeSpan, [sink, func](const auto& tasks) {
            sink->send(func(tasks));
        });
    query_invoker.executeQuery(std::move(requestItems));
}

void CoreService::sprintsInTimeRange(
    const TimeSpan& timeSpan,
    std::function<void(const std::vector<Sprint>&)> onResultsReceivedCallback)
{
    auto requestItems = std::make_unique<RequestSprints>(
        sprintReader, timeSpan, onResultsReceivedCallback);
    query_invoker.executeQuery(std::move(requestItems));
}

void CoreService::registerSprint(const TimeSpan& timeSpan,
                                 const std::string& taskUuid)
{
    Sprint sprint{taskUuid, timeSpan};
    registerSprint(sprint);
}

void CoreService::registerSprint(const Sprint& sprint)
{
    auto registerNewSprint
        = std::make_unique<RegisterNewSprint>(sprintWriter, sprint);
    auto incrementTaskSprints
        = std::make_unique<IncrementTaskSprints>(taskWriter, sprint.taskUuid());
    std::vector<std::unique_ptr<core::Command>> commands;
    commands.push_back(std::move(registerNewSprint));
    commands.push_back(std::move(incrementTaskSprints));
    auto addSprintTransaction
        = std::make_unique<MacroTransaction>(std::move(commands));
    invoker.executeCommand(std::move(addSprintTransaction));
}

void CoreService::removeSprint(const Sprint& sprint)
{
    const std::string& taskUuid = sprint.taskUuid();
    auto removeSprint
        = std::make_unique<RemoveSprintTransaction>(sprintWriter, sprint);
    auto decrementTaskSprints
        = std::make_unique<DecrementTaskSprints>(taskWriter, taskUuid);
    std::vector<std::unique_ptr<core::Command>> commands;
    commands.push_back(std::move(removeSprint));
    commands.push_back(std::move(decrementTaskSprints));
    auto removeSprintTransaction
        = std::make_unique<MacroTransaction>(std::move(commands));
    invoker.executeCommand(std::move(removeSprintTransaction));
}

void CoreService::exportSprints(const TimeSpan& timeSpan,
                                std::shared_ptr<ExternalIO::ISink> sink,
                                ICoreService::SprintEncodingFunc func)
{
    auto requestItems = std::make_unique<RequestSprints>(
        sprintReader, timeSpan, [sink, func](const auto& sprints) {
            sink->send(func(sprints));
        });
    query_invoker.executeQuery(std::move(requestItems));
}

void CoreService::yearRange(std::function<void(const std::vector<std::string>&)>
                                onResultsReceivedCallback)
{
    auto requestYearRange = std::make_unique<RequestMinMaxYear>(
        yearRangeReader, onResultsReceivedCallback);
    query_invoker.executeQuery(std::move(requestYearRange));
}

void CoreService::requestSprintDailyDistribution(
    const TimeSpan& timeSpan,
    std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
{
    auto requestDistribution = std::make_unique<RequestSprintDistribution>(
        sprintDailyDistributionReader, timeSpan, onResultsReceivedCallback);
    query_invoker.executeQuery(std::move(requestDistribution));
}

void CoreService::requestSprintWeeklyDistribution(
    const TimeSpan& timeSpan,
    std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
{
    auto requestDistribution = std::make_unique<RequestSprintDistribution>(
        sprintWeeklyDistributionReader, timeSpan, onResultsReceivedCallback);
    query_invoker.executeQuery(std::move(requestDistribution));
}

void CoreService::requestSprintMonthlyDistribution(
    const TimeSpan& timeSpan,
    std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
{
    auto requestDistribution = std::make_unique<RequestSprintDistribution>(
        sprintMonthlyDistributionReader, timeSpan, onResultsReceivedCallback);
    query_invoker.executeQuery(std::move(requestDistribution));
}

void CoreService::requestAllTags(TagResultHandler onResultsReceivedCallback)
{
    auto requestTags = std::make_unique<RequestAllTags>(
        taskReader, onResultsReceivedCallback);
    query_invoker.executeQuery(std::move(requestTags));
}

void CoreService::editTag(const std::string& oldName,
                          const std::string& newName)
{
    auto editTag = std::make_unique<RenameTag>(taskWriter, oldName, newName);
    invoker.executeCommand(std::move(editTag));
}

std::string CoreService::lastCommandDescription() const
{
    return invoker.lastCommandDescription();
}

uint64_t CoreService::numRevertableCommands() const
{
    return invoker.stackSize();
}

void CoreService::undoLast() { invoker.undo(); }

} // namespace Core
