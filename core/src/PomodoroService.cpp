#include "core/PomodoroService.h"
#include "use_cases/AddPomodoroTransaction.h"
#include "use_cases/AddTaskTransaction.h"
#include "use_cases/DecrementSpentPomodoros.h"
#include "use_cases/IncrementSpentPomodoros.h"
#include "use_cases/RemovePomodoroTransaction.h"
#include "use_cases/RemoveTaskTransaction.h"
#include "use_cases/RequestPomoDistribution.h"
#include "use_cases/RequestPomodoroYearRangeCommand.h"
#include "use_cases/RequestPomodorosInTimeRangeCommand.h"

namespace CoreApi {

PomodoroService::PomodoroService(
    IPomodoroStorageReader& pomodoroStorageReader,
    IPomodoroStorageWriter& pomodoroStorageWriter,
    IPomodoroYearRangeReader& pomodoroYearRangeReader,
    ITaskStorageWriter& taskStorageWriter,
    IPomodoroDistributionReader& pomoDailyDistributionReader,
    IPomodoroDistributionReader& pomoWeeklyDistributionReader,
    IPomodoroDistributionReader& pomoMonthlyDistributionReader)
    : pomodoroReader{pomodoroStorageReader}
    , pomodoroWriter{pomodoroStorageWriter}
    , pomodoroYearRangeReader{pomodoroYearRangeReader}
    , taskWriter{taskStorageWriter}
    , pomoDailyDistributionReader{pomoDailyDistributionReader}
    , pomoWeeklyDistributionReader{pomoWeeklyDistributionReader}
    , pomoMonthlyDistributionReader{pomoMonthlyDistributionReader}
{
}

void PomodoroService::registerPomodoro(const TimeSpan& timeSpan,
                                       const std::string& taskUuid)
{
    Pomodoro pomodoro{taskUuid, timeSpan};
    std::unique_ptr<RevertableCommand> addPomodoro
        = std::make_unique<UseCases::AddPomodoroTransaction>(pomodoroWriter,
                                                             pomodoro);
    std::unique_ptr<RevertableCommand> incrementSpentPomodoros
        = std::make_unique<UseCases::IncrementSpentPomodoros>(taskWriter,
                                                              taskUuid);
    std::vector<std::unique_ptr<RevertableCommand>> commands;
    commands.push_back(std::move(addPomodoro));
    commands.push_back(std::move(incrementSpentPomodoros));
    std::unique_ptr<RevertableCommand> addPomodoroTransaction
        = std::make_unique<MacroTransaction>(std::move(commands));
    addPomodoroTransaction->execute();
    commandStack.push_back(std::move(addPomodoroTransaction));
}


void PomodoroService::removePomodoro(const Pomodoro& pomodoro)
{
    const std::string& taskUuid = pomodoro.taskUuid();
    std::unique_ptr<RevertableCommand> removePomodoro
        = std::make_unique<UseCases::RemovePomodoroTransaction>(pomodoroWriter,
                                                                pomodoro);
    std::unique_ptr<RevertableCommand> decrementSpentPomodoros
        = std::make_unique<UseCases::DecrementSpentPomodoros>(taskWriter,
                                                              taskUuid);
    std::vector<std::unique_ptr<RevertableCommand>> commands;
    commands.push_back(std::move(removePomodoro));
    commands.push_back(std::move(decrementSpentPomodoros));
    std::unique_ptr<RevertableCommand> removePomodoroTransaction
        = std::make_unique<MacroTransaction>(std::move(commands));
    removePomodoroTransaction->execute();
    commandStack.push_back(std::move(removePomodoroTransaction));
}

void PomodoroService::registerTask(const TodoItem& task)
{
    std::unique_ptr<RevertableCommand> addTask
        = std::make_unique<UseCases::AddTaskTransaction>(taskWriter, task);
    addTask->execute();
    commandStack.push_back(std::move(addTask));
}

void PomodoroService::removeTask(const TodoItem& task)
{
    std::unique_ptr<RevertableCommand> remove
        = std::make_unique<UseCases::RemoveTaskTransaction>(taskWriter, task);
    remove->execute();
    commandStack.push_back(std::move(remove));
}

void PomodoroService::pomodorosInTimeRange(
    const TimeSpan& timeSpan,
    std::function<void(const std::vector<Pomodoro>&)> onResultsReceivedCallback)
{
    std::unique_ptr<ICommand> requestItems
        = std::make_unique<UseCases::RequestPomodorosInTimeRangeCommand>(
            pomodoroReader, timeSpan, onResultsReceivedCallback);
    requestItems->execute();
}

void PomodoroService::pomodoroYearRange(
    std::function<void(const std::vector<std::string>&)>
        onResultsReceivedCallback)
{
    std::unique_ptr<ICommand> requestYearRange
        = std::make_unique<UseCases::RequestPomodoroYearRangeCommand>(
            pomodoroYearRangeReader, onResultsReceivedCallback);
    requestYearRange->execute();
}

void PomodoroService::requestPomodoroDailyDistribution(
    const TimeSpan& timeSpan,
    std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
{
    std::unique_ptr<ICommand> requestDistribution
        = std::make_unique<UseCases::RequestPomoDistribution>(
            pomoDailyDistributionReader, timeSpan, onResultsReceivedCallback);
    requestDistribution->execute();
}

void PomodoroService::requestPomodoroWeeklyDistribution(
    const TimeSpan& timeSpan,
    std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
{
    std::unique_ptr<ICommand> requestDistribution
        = std::make_unique<UseCases::RequestPomoDistribution>(
            pomoWeeklyDistributionReader, timeSpan, onResultsReceivedCallback);
    requestDistribution->execute();
}

void PomodoroService::requestPomodoroMonthlyDistribution(
    const TimeSpan& timeSpan,
    std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
{
    std::unique_ptr<ICommand> requestDistribution
        = std::make_unique<UseCases::RequestPomoDistribution>(
            pomoMonthlyDistributionReader, timeSpan, onResultsReceivedCallback);
    requestDistribution->execute();
}

void PomodoroService::undoLast()
{
    commandStack.back()->undo();
    commandStack.pop_back();
}

} /* CoreApi */
