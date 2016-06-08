#ifndef POMODOROSERVICE_H_JXK8PKAI
#define POMODOROSERVICE_H_JXK8PKAI


#include "core/CommandInvoker.h"
#include "core/IPomodoroDistributionReader.h"
#include "core/IPomodoroService.h"
#include "core/IPomodoroStorageReader.h"
#include "core/IPomodoroStorageWriter.h"
#include "core/IPomodoroYearRangeReader.h"
#include "core/ITaskStorageReader.h"
#include "core/ITaskStorageWriter.h"
#include "core/MacroTransaction.h"
#include <memory>

namespace CoreApi {

class PomodoroService : public IPomodoroService {
public:
    PomodoroService(IPomodoroStorageReader& pomodoroStorageReader,
                    IPomodoroStorageWriter& pomodoroStorageWriter,
                    IPomodoroYearRangeReader& pomodoroYearRangeReader,
                    ITaskStorageReader& taskStorageReader,
                    ITaskStorageWriter& taskStorageWriter,
                    IPomodoroDistributionReader& pomoDailyDistributionReader,
                    IPomodoroDistributionReader& pomoWeeklyDistributionReader,
                    IPomodoroDistributionReader& pomoMonthlyDistributionReader);

    void registerTask(const TodoItem& task) final;

    void removeTask(const TodoItem& task) final;

    void editTask(const TodoItem& task, const TodoItem& editedTask) final;

    void toggleTaskCompletionStatus(const TodoItem& task) final;

    void registerTaskPriorities(
        std::vector<std::pair<std::string, int>>&& priorities) final;

    void requestFinishedTasks(const TimeSpan& timeSpan,
                              std::function<void(const std::vector<TodoItem>&)>
                                  onResultsReceivedCallback) final;

    void
    requestUnfinishedTasks(std::function<void(const std::vector<TodoItem>&)>
                               onResultsReceivedCallback) final;

    void registerPomodoro(const TimeSpan& timeSpan,
                          const std::string& taskUuid) final;

    void removePomodoro(const Pomodoro& pomodoro) final;

    void pomodorosInTimeRange(const TimeSpan& timeSpan,
                              std::function<void(const std::vector<Pomodoro>&)>
                                  onResultsReceivedCallback) final;

    void pomodoroYearRange(std::function<void(const std::vector<std::string>&)>
                               onResultsReceivedCallback) final;

    void requestPomodoroDailyDistribution(
        const TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        final;

    void requestPomodoroWeeklyDistribution(
        const TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        final;

    void requestPomodoroMonthlyDistribution(
        const TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)> onResultsReceivedCallback)
        final;

    void requestAllTags(TagResultHandler onResultsReceivedCallback) final;

    void editTag(const std::string& oldName, const std::string& newName) final;

    void undoLast() final;

private:
    IPomodoroStorageReader& pomodoroReader;
    IPomodoroStorageWriter& pomodoroWriter;
    IPomodoroYearRangeReader& pomodoroYearRangeReader;
    ITaskStorageReader& taskReader;
    ITaskStorageWriter& taskWriter;
    IPomodoroDistributionReader& pomoDailyDistributionReader;
    IPomodoroDistributionReader& pomoWeeklyDistributionReader;
    IPomodoroDistributionReader& pomoMonthlyDistributionReader;
    CommandInvoker invoker;
};


} // namespace CoreApi

#endif /* end of include guard: POMODOROSERVICE_H_JXK8PKAI */
