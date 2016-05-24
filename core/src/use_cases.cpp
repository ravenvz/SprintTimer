#include "core/use_cases/use_cases.h"
#include "core/use_cases/AddPomodoroTransaction.h"
#include "core/use_cases/DecrementSpentPomodoros.h"
#include "core/use_cases/IncrementSpentPomodoros.h"
#include "core/use_cases/RemovePomodoroTransaction.h"
#include "core/use_cases/RequestPomodorosInTimeRangeCommand.h"

namespace CoreApi {

void addPomodoro(IPomodoroStorageWriter& pomodoroStorageWriter,
                 ITaskStorageWriter& taskStorageWriter,
                 const TimeSpan& timeSpan,
                 const std::string& taskUuid)
{
    MacroTransaction addPomodoroTransaction;
    Pomodoro pomodoro{taskUuid, timeSpan};
    std::unique_ptr<RevertableCommand> addPomodoro
        = std::make_unique<UseCases::AddPomodoroTransaction>(
            pomodoroStorageWriter, pomodoro);
    std::unique_ptr<RevertableCommand> incrementSpentPomodoros
        = std::make_unique<UseCases::IncrementSpentPomodoros>(taskStorageWriter,
                                                              taskUuid);
    addPomodoroTransaction.addCommand(std::move(addPomodoro));
    addPomodoroTransaction.addCommand(std::move(incrementSpentPomodoros));
    addPomodoroTransaction.execute();
}


void removePomodoro(IPomodoroStorageWriter& pomodoroStorageWriter,
                    ITaskStorageWriter& taskStorageWriter,
                    const Pomodoro& pomodoro)
{
    const std::string& taskUuid = pomodoro.taskUuid();
    MacroTransaction removePomodoroTransaction;
    std::unique_ptr<RevertableCommand> removePomodoro
        = std::make_unique<UseCases::RemovePomodoroTransaction>(
            pomodoroStorageWriter, pomodoro);
    std::unique_ptr<RevertableCommand> decrementSpentPomodoros
        = std::make_unique<UseCases::DecrementSpentPomodoros>(taskStorageWriter,
                                                              taskUuid);
    removePomodoroTransaction.addCommand(std::move(removePomodoro));
    removePomodoroTransaction.addCommand(std::move(decrementSpentPomodoros));
    removePomodoroTransaction.execute();
}

void pomodorosInTimeRange(
    IPomodoroStorageReader& pomodoroStorageReader,
    const TimeSpan& timeSpan,
    std::function<void(const std::vector<Pomodoro>&)> onResultsReceivedCallback)
{
    std::unique_ptr<ICommand> requestItems
        = std::make_unique<UseCases::RequestPomodorosInTimeRangeCommand>(
            pomodoroStorageReader, timeSpan, onResultsReceivedCallback);
    requestItems->execute();
}

} /* CoreApi */
