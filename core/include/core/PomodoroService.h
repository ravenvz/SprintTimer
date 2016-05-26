#ifndef POMODOROSERVICE_H_JXK8PKAI
#define POMODOROSERVICE_H_JXK8PKAI


#include "core/IPomodoroDistributionReader.h"
#include "core/IPomodoroService.h"
#include "core/IPomodoroStorageReader.h"
#include "core/IPomodoroStorageWriter.h"
#include "core/IPomodoroYearRangeReader.h"
#include "core/ITaskStorageWriter.h"
#include "core/MacroTransaction.h"
#include <memory>

namespace CoreApi {

class PomodoroService : public IPomodoroService {
public:
    PomodoroService(IPomodoroStorageReader& pomodoroStorageReader,
                    IPomodoroStorageWriter& pomodoroStorageWriter,
                    IPomodoroYearRangeReader& pomodoroYearRangeReader,
                    ITaskStorageWriter& taskStorageWriter,
                    IPomodoroDistributionReader& pomoDailyDistributionReader,
                    IPomodoroDistributionReader& pomoWeeklyDistributionReader,
                    IPomodoroDistributionReader& pomoMonthlyDistributionReader);

    void registerPomodoro(const TimeSpan& timeSpan,
                          const std::string& taskUuid) final;

    void removePomodoro(const Pomodoro& pomodoro) final;

    void registerTask(const TodoItem& task) final;

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

    void undoLast() final;

private:
    IPomodoroStorageReader& pomodoroReader;
    IPomodoroStorageWriter& pomodoroWriter;
    IPomodoroYearRangeReader& pomodoroYearRangeReader;
    ITaskStorageWriter& taskWriter;
    IPomodoroDistributionReader& pomoDailyDistributionReader;
    IPomodoroDistributionReader& pomoWeeklyDistributionReader;
    IPomodoroDistributionReader& pomoMonthlyDistributionReader;
    std::vector<std::unique_ptr<RevertableCommand>> commandStack;
};


} // namespace CoreApi

#endif /* end of include guard: POMODOROSERVICE_H_JXK8PKAI */
