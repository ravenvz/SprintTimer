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
#include "core/CoreService.h"
#include "core/StringUtils.h"
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
#include "core/external_io/Marshaller.h"

namespace Core {

namespace {

    using namespace ExternalIO;

    void onTasksPreparedForExport(std::shared_ptr<Marshaller> marshaller,
                                  const std::vector<Task>& tasks)
    {
        auto task_to_string = [](const Task& task) {
            std::vector<std::string> str;
            auto tags = task.tags();
            str.emplace_back(StringUtils::join(cbegin(tags), cend(tags), ", "));
            str.emplace_back(task.name());
            str.emplace_back(task.uuid());
            str.emplace_back(std::to_string(task.isCompleted()));
            str.emplace_back(std::to_string(task.actualCost()));
            str.emplace_back(std::to_string(task.estimatedCost()));
            return str;
        };
        marshaller->marshall(tasks, task_to_string);
    }

    void onSprintsPreparedForExport(std::shared_ptr<Marshaller> marshaller,
                                    const std::vector<Sprint>& sprints)
    {
        auto sprint_to_str = [](const Sprint& sprint) {
            std::vector<std::string> str;
            auto tags = sprint.tags();
            str.emplace_back(StringUtils::join(cbegin(tags), cend(tags), ", "));
            str.emplace_back(sprint.timeSpan().toString("hh:MM"));
            str.emplace_back(sprint.name());
            str.emplace_back(sprint.taskUuid());
            str.emplace_back(sprint.uuid());
            return str;
        };
        marshaller->marshall(sprints, sprint_to_str);
    }

} // namespace

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
    auto addTask = std::make_unique<UseCases::AddNewTask>(taskWriter, task);
    invoker.executeCommand(std::move(addTask));
}

void CoreService::removeTask(const Task& task)
{
    auto deleteTask = std::make_unique<UseCases::DeleteTask>(taskWriter, task);
    invoker.executeCommand(std::move(deleteTask));
}

void CoreService::editTask(const Task& task, const Task& editedTask)
{
    auto editTask
        = std::make_unique<UseCases::EditTask>(taskWriter, task, editedTask);
    invoker.executeCommand(std::move(editTask));
}

void CoreService::toggleTaskCompletionStatus(const Task& task)
{
    auto toggleTaskCommand
        = std::make_unique<UseCases::ToggleTaskCompletionStatus>(taskWriter,
                                                                 task);
    invoker.executeCommand(std::move(toggleTaskCommand));
}

void CoreService::registerTaskPriorities(
    std::vector<std::pair<std::string, int>>&& priorities)
{
    auto registerPrioritiesCommand
        = std::make_unique<UseCases::StoreUnfinishedTasksOrder>(
            taskWriter, std::move(priorities));
    invoker.executeCommand(std::move(registerPrioritiesCommand));
}

void CoreService::requestFinishedTasks(
    const TimeSpan& timeSpan,
    std::function<void(const std::vector<Task>&)> onResultsReceivedCallback)
{
    std::unique_ptr<Command> requestItems
        = std::make_unique<UseCases::RequestFinishedTasks>(
            taskReader, timeSpan, onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestItems));
}

void CoreService::requestUnfinishedTasks(
    std::function<void(const std::vector<Task>&)> onResultsReceivedCallback)
{
    std::unique_ptr<Command> requestItems
        = std::make_unique<UseCases::RequestUnfinishedTasks>(
            taskReader, onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestItems));
}

void CoreService::exportTasks(std::shared_ptr<Marshaller> marshaller,
                              const TimeSpan& timeSpan)
{
    std::unique_ptr<Command> requestItems
        = std::make_unique<UseCases::RequestTasks>(
            taskReader, timeSpan, [marshaller](const auto& tasks) {
                onTasksPreparedForExport(marshaller, tasks);
            });
    invoker.executeCommand(std::move(requestItems));
}

void CoreService::sprintsInTimeRange(
    const TimeSpan& timeSpan,
    std::function<void(const std::vector<Sprint>&)> onResultsReceivedCallback)
{
    std::unique_ptr<Command> requestItems
        = std::make_unique<UseCases::RequestSprints>(
            sprintReader, timeSpan, onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestItems));
}

void CoreService::registerSprint(const TimeSpan& timeSpan,
                                 const std::string& taskUuid)
{
    Sprint sprint{taskUuid, timeSpan};
    auto registerNewSprint
        = std::make_unique<UseCases::RegisterNewSprint>(sprintWriter, sprint);
    auto incrementTaskSprints
        = std::make_unique<UseCases::IncrementTaskSprints>(taskWriter,
                                                           taskUuid);
    std::vector<std::unique_ptr<RevertableCommand>> commands;
    commands.push_back(std::move(registerNewSprint));
    commands.push_back(std::move(incrementTaskSprints));
    auto addSprintTransaction
        = std::make_unique<MacroTransaction>(std::move(commands));
    invoker.executeCommand(std::move(addSprintTransaction));
}

void CoreService::removeSprint(const Sprint& sprint)
{
    const std::string& taskUuid = sprint.taskUuid();
    auto removeSprint = std::make_unique<UseCases::RemoveSprintTransaction>(
        sprintWriter, sprint);
    auto decrementTaskSprints
        = std::make_unique<UseCases::DecrementTaskSprints>(taskWriter,
                                                           taskUuid);
    std::vector<std::unique_ptr<RevertableCommand>> commands;
    commands.push_back(std::move(removeSprint));
    commands.push_back(std::move(decrementTaskSprints));
    auto removeSprintTransaction
        = std::make_unique<MacroTransaction>(std::move(commands));
    invoker.executeCommand(std::move(removeSprintTransaction));
}

void CoreService::exportSprints(std::shared_ptr<Marshaller> marshaller,
                                const TimeSpan& timeSpan)
{
    std::unique_ptr<Command> requestItems
        = std::make_unique<UseCases::RequestSprints>(
            sprintReader, timeSpan, [marshaller](const auto& sprints) {
                onSprintsPreparedForExport(marshaller, sprints);
            });
    invoker.executeCommand(std::move(requestItems));
}

void CoreService::yearRange(std::function<void(const std::vector<std::string>&)>
                                onResultsReceivedCallback)
{
    auto requestYearRange = std::make_unique<UseCases::RequestMinMaxYear>(
        yearRangeReader, onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestYearRange));
}

void CoreService::requestSprintDailyDistribution(
    const TimeSpan& timeSpan,
    std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
{
    auto requestDistribution
        = std::make_unique<UseCases::RequestSprintDistribution>(
            sprintDailyDistributionReader, timeSpan, onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestDistribution));
}

void CoreService::requestSprintWeeklyDistribution(
    const TimeSpan& timeSpan,
    std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
{
    auto requestDistribution
        = std::make_unique<UseCases::RequestSprintDistribution>(
            sprintWeeklyDistributionReader,
            timeSpan,
            onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestDistribution));
}

void CoreService::requestSprintMonthlyDistribution(
    const TimeSpan& timeSpan,
    std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
{
    auto requestDistribution
        = std::make_unique<UseCases::RequestSprintDistribution>(
            sprintMonthlyDistributionReader,
            timeSpan,
            onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestDistribution));
}

void CoreService::requestAllTags(TagResultHandler onResultsReceivedCallback)
{
    auto requestTags = std::make_unique<UseCases::RequestAllTags>(
        taskReader, onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestTags));
}

void CoreService::editTag(const std::string& oldName,
                          const std::string& newName)
{
    auto editTag
        = std::make_unique<UseCases::RenameTag>(taskWriter, oldName, newName);
    editTag->execute();
    invoker.executeCommand(std::move(editTag));
}

std::string CoreService::lastCommandDescription() const
{
    return invoker.lastCommandDescription();
}

unsigned long long CoreService::numRevertableCommands() const
{
    return invoker.stackSize();
}

void CoreService::undoLast() { invoker.undo(); }

} /* Core */
