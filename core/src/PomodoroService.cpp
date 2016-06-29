#include "core/PomodoroService.h"
#include "use_cases/AddNewTask.h"
#include "use_cases/DecrementSpentPomodoros.h"
#include "use_cases/DeleteTask.h"
#include "use_cases/EditTag.h"
#include "use_cases/EditTask.h"
#include "use_cases/IncrementSpentPomodoros.h"
#include "use_cases/RegisterNewPomodoro.h"
#include "use_cases/RemovePomodoroTransaction.h"
#include "use_cases/RequestAllTags.h"
#include "use_cases/RequestFinishedTasks.h"
#include "use_cases/RequestMinMaxYear.h"
#include "use_cases/RequestPomoDistribution.h"
#include "use_cases/RequestPomodoros.h"
#include "use_cases/RequestUnfinishedTasks.h"
#include "use_cases/StoreUnfinishedTasksOrder.h"
#include "use_cases/ToggleTaskCompletionStatus.h"

namespace CoreApi {

PomodoroService::PomodoroService(
    IPomodoroStorageReader& pomodoroStorageReader,
    IPomodoroStorageWriter& pomodoroStorageWriter,
    IPomodoroYearRangeReader& pomodoroYearRangeReader,
    ITaskStorageReader& taskStorageReader,
    ITaskStorageWriter& taskStorageWriter,
    IPomodoroDistributionReader& pomoDailyDistributionReader,
    IPomodoroDistributionReader& pomoWeeklyDistributionReader,
    IPomodoroDistributionReader& pomoMonthlyDistributionReader)
    : pomodoroReader{pomodoroStorageReader}
    , pomodoroWriter{pomodoroStorageWriter}
    , pomodoroYearRangeReader{pomodoroYearRangeReader}
    , taskReader{taskStorageReader}
    , taskWriter{taskStorageWriter}
    , pomoDailyDistributionReader{pomoDailyDistributionReader}
    , pomoWeeklyDistributionReader{pomoWeeklyDistributionReader}
    , pomoMonthlyDistributionReader{pomoMonthlyDistributionReader}
{
}

void PomodoroService::registerTask(const Task& task)
{
    auto addTask = std::make_unique<UseCases::AddNewTask>(taskWriter, task);
    invoker.executeCommand(std::move(addTask));
}

void PomodoroService::removeTask(const Task& task)
{
    auto deleteTask = std::make_unique<UseCases::DeleteTask>(taskWriter, task);
    invoker.executeCommand(std::move(deleteTask));
}

void PomodoroService::editTask(const Task& task, const Task& editedTask)
{
    auto editTask
        = std::make_unique<UseCases::EditTask>(taskWriter, task, editedTask);
    invoker.executeCommand(std::move(editTask));
}

void PomodoroService::toggleTaskCompletionStatus(const Task& task)
{
    auto toggleTaskCommand
        = std::make_unique<UseCases::ToggleTaskCompletionStatus>(taskWriter,
                                                                 task);
    invoker.executeCommand(std::move(toggleTaskCommand));
}

void PomodoroService::registerTaskPriorities(
    std::vector<std::pair<std::string, int>>&& priorities)
{
    auto registerPrioritiesCommand
        = std::make_unique<UseCases::StoreUnfinishedTasksOrder>(
            taskWriter, std::move(priorities));
    invoker.executeCommand(std::move(registerPrioritiesCommand));
}

void PomodoroService::requestFinishedTasks(
    const TimeSpan& timeSpan,
    std::function<void(const std::vector<Task>&)> onResultsReceivedCallback)
{
    std::unique_ptr<Command> requestItems
        = std::make_unique<UseCases::RequestFinishedTasks>(
            taskReader, timeSpan, onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestItems));
}

void PomodoroService::requestUnfinishedTasks(
    std::function<void(const std::vector<Task>&)> onResultsReceivedCallback)
{
    std::unique_ptr<Command> requestItems
        = std::make_unique<UseCases::RequestUnfinishedTasks>(
            taskReader, onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestItems));
}

void PomodoroService::pomodorosInTimeRange(
    const TimeSpan& timeSpan,
    std::function<void(const std::vector<Pomodoro>&)> onResultsReceivedCallback)
{
    std::unique_ptr<Command> requestItems
        = std::make_unique<UseCases::RequestPomodoros>(
            pomodoroReader, timeSpan, onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestItems));
}

void PomodoroService::registerPomodoro(const TimeSpan& timeSpan,
                                       const std::string& taskUuid)
{
    Pomodoro pomodoro{taskUuid, timeSpan};
    auto registerNewPomodoro = std::make_unique<UseCases::RegisterNewPomodoro>(
        pomodoroWriter, pomodoro);
    auto incrementSpentPomodoros
        = std::make_unique<UseCases::IncrementSpentPomodoros>(taskWriter,
                                                              taskUuid);
    std::vector<std::unique_ptr<RevertableCommand>> commands;
    commands.push_back(std::move(registerNewPomodoro));
    commands.push_back(std::move(incrementSpentPomodoros));
    auto addPomodoroTransaction
        = std::make_unique<MacroTransaction>(std::move(commands));
    invoker.executeCommand(std::move(addPomodoroTransaction));
}

void PomodoroService::removePomodoro(const Pomodoro& pomodoro)
{
    const std::string& taskUuid = pomodoro.taskUuid();
    auto removePomodoro = std::make_unique<UseCases::RemovePomodoroTransaction>(
        pomodoroWriter, pomodoro);
    auto decrementSpentPomodoros
        = std::make_unique<UseCases::DecrementSpentPomodoros>(taskWriter,
                                                              taskUuid);
    std::vector<std::unique_ptr<RevertableCommand>> commands;
    commands.push_back(std::move(removePomodoro));
    commands.push_back(std::move(decrementSpentPomodoros));
    auto removePomodoroTransaction
        = std::make_unique<MacroTransaction>(std::move(commands));
    invoker.executeCommand(std::move(removePomodoroTransaction));
}

void PomodoroService::pomodoroYearRange(
    std::function<void(const std::vector<std::string>&)>
        onResultsReceivedCallback)
{
    auto requestYearRange = std::make_unique<UseCases::RequestMinMaxYear>(
        pomodoroYearRangeReader, onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestYearRange));
}

void PomodoroService::requestPomodoroDailyDistribution(
    const TimeSpan& timeSpan,
    std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
{
    auto requestDistribution
        = std::make_unique<UseCases::RequestPomoDistribution>(
            pomoDailyDistributionReader, timeSpan, onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestDistribution));
}

void PomodoroService::requestPomodoroWeeklyDistribution(
    const TimeSpan& timeSpan,
    std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
{
    auto requestDistribution
        = std::make_unique<UseCases::RequestPomoDistribution>(
            pomoWeeklyDistributionReader, timeSpan, onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestDistribution));
}

void PomodoroService::requestPomodoroMonthlyDistribution(
    const TimeSpan& timeSpan,
    std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
{
    auto requestDistribution
        = std::make_unique<UseCases::RequestPomoDistribution>(
            pomoMonthlyDistributionReader, timeSpan, onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestDistribution));
}

void PomodoroService::requestAllTags(TagResultHandler onResultsReceivedCallback)
{
    auto requestTags = std::make_unique<UseCases::RequestAllTags>(
        taskReader, onResultsReceivedCallback);
    invoker.executeCommand(std::move(requestTags));
}

void PomodoroService::editTag(const std::string& oldName,
                              const std::string& newName)
{
    auto editTag
        = std::make_unique<UseCases::EditTag>(taskWriter, oldName, newName);
    editTag->execute();
    invoker.executeCommand(std::move(editTag));
}

std::string PomodoroService::lastCommandDescription() const {
    return invoker.lastCommandDescription();
}

size_t PomodoroService::numRevertableCommands() const {
    return invoker.stackSize();
}

void PomodoroService::undoLast() { invoker.undo(); }

} /* CoreApi */
