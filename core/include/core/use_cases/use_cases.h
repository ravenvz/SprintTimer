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
#include "core/ITaskStorageWriter.h"
#include "core/entities/Pomodoro.h"
#include "core/use_cases/AddPomodoroTransaction.h"
#include <memory>

namespace CoreApi {

void addPomodoro(IPomodoroStorageWriter& pomodoroStorageWriter,
                 ITaskStorageWriter& taskStorageWriter,
                 const TimeSpan& timeSpan,
                 const std::string& taskUuid);

void removePomodoro(IPomodoroStorageWriter& pomodoroStorageWriter,
                    ITaskStorageWriter& taskStorageWriter,
                    const Pomodoro& pomodoro);

void pomodorosInTimeRange(IPomodoroStorageReader& pomodoroStorageReader,
                          const TimeSpan& timeSpan,
                          std::function<void(const std::vector<Pomodoro>&)>
                              onResultsReceivedCallback);


} // namespace CoreApi

#endif /* end of include guard: USE_CASES_H_FWYXSOEZ */
