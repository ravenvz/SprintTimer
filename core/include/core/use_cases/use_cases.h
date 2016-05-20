#ifndef USE_CASES_H_FWYXSOEZ
#define USE_CASES_H_FWYXSOEZ

#include "core/IPomodoroStorageReader.h"
#include "core/IPomodoroStorageWriter.h"
#include "core/entities/Pomodoro.h"
#include "core/entities/TodoItem.h"
#include <experimental/optional>
#include <functional>
#include <vector>

#include "core/ICommand.h"
#include "core/ITransaction.h"

namespace UseCases {

using std::experimental::optional;
using std::experimental::make_optional;

/* Pomodoro use cases */


// bool addPomodoro(const Pomodoro& pomodoro);
//


// std::vector<Pomodoro> pomodorosInInterval(IPomodoroStorageReader&
// gateway,
//                                           const TimeSpan& timeSpan)
// {
//     return gateway.items(timeSpan);
// }
//
// std::vector<Pomodoro> todayPomodoros(IPomodoroStorageReader& gateway);
//
// std::vector<std::string> pomodoroYearRange(IPomodoroStorageReader&
// gateway)
// {
//     return gateway.requestYearRange();
// }


/* Task use cases */

bool addTask(const TodoItem& task);

bool removeTask(const TodoItem& task);

bool completeTask(const TodoItem& task);

bool editTask(const TodoItem& task);

std::vector<TodoItem> taskInInterval(const TimeSpan& timeSpan);

std::vector<TodoItem> nonCompletedTasks();


} // namespace UseCases

#endif /* end of include guard: USE_CASES_H_FWYXSOEZ */
