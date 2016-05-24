#ifndef USE_CASES_H_FWYXSOEZ
#define USE_CASES_H_FWYXSOEZ

// #include "core/IPomodoroStorageWriter.h"
// #include "core/entities/Pomodoro.h"
// #include "core/entities/TodoItem.h"
// #include <experimental/optional>
// #include <functional>
// #include <vector>

// #include "core/ICommand.h"
// #include "core/ITransaction.h"
#include "core/IPomodoroStorageReader.h"
#include "core/MacroTransaction.h"
// #include "core/RevertableCommand.h"
#include "core/IPomodoroDistributionReader.h"
#include "core/IPomodoroYearRangeReader.h"
#include "core/ITaskStorageWriter.h"
#include "core/entities/Pomodoro.h"
#include "core/use_cases/AddPomodoroTransaction.h"
#include <memory>

namespace CoreApi {

class PomodoroCoreFacade {
public:
    PomodoroCoreFacade(
        IPomodoroStorageReader& pomodoroStorageReader,
        IPomodoroStorageWriter& pomodoroStorageWriter,
        IPomodoroYearRangeReader& pomodoroYearRangeReader,
        ITaskStorageWriter& taskStorageWriter,
        IPomodoroDistributionReader& pomoDailyDistributionReader,
        IPomodoroDistributionReader& pomoWeeklyDistributionReader,
        IPomodoroDistributionReader& pomoMonthlyDistributionReader);

    void addPomodoro(const TimeSpan& timeSpan, const std::string& taskUuid);

    void removePomodoro(const Pomodoro& pomodoro);

    void pomodorosInTimeRange(const TimeSpan& timeSpan,
                              std::function<void(const std::vector<Pomodoro>&)>
                                  onResultsReceivedCallback);

    void pomodoroYearRange(std::function<void(const std::vector<std::string>&)>
                               onResultsReceivedCallback);

    void requestPomodoroDailyDistribution(
        const TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)>
            onResultsReceivedCallback);

    void requestPomodoroWeeklyDistribution(
        const TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)>
            onResultsReceivedCallback);

    void requestPomodoroMonthlyDistribution(
        const TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)>
            onResultsReceivedCallback);

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

#endif /* end of include guard: USE_CASES_H_FWYXSOEZ */
