#ifndef USE_CASES_H_FWYXSOEZ
#define USE_CASES_H_FWYXSOEZ

#include "core/IPomodoroStorageReader.h"
#include "core/entities/Pomodoro.h"
#include "core/entities/TodoItem.h"
#include <functional>
#include <vector>

namespace UseCases {

/* Pomodoro use cases */
class IPomodoroStorageWriter {
public:
    virtual ~IPomodoroStorageWriter() = default;
    virtual bool save(const Pomodoro& pomodoro) = 0;
    virtual bool remove(const Pomodoro& pomodoro) = 0;
};


class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
};


class ITransaction {
public:
    virtual ~ITransaction() = default;
    virtual bool execute() = 0;
    virtual bool undo() = 0;
};


class IPomodoroYearRangeReader {
public:
    virtual ~IPomodoroYearRangeReader() = default;
    virtual void requestYearRange() = 0;
};


class RequestPomodoroYearRangeCommand : public ICommand {
public:
    RequestPomodoroYearRangeCommand(IPomodoroYearRangeReader& reader)
        : reader{reader}
    {
    }

    void execute() final { reader.requestYearRange(); }

private:
    IPomodoroYearRangeReader& reader;
};


class RequestPomodorosInTimeRangeCommand : public ICommand {
public:
    RequestPomodorosInTimeRangeCommand(
        IPomodoroStorageReader& reader,
        const TimeSpan& timeSpan,
        std::function<void(const std::vector<Pomodoro>&)> resultHandler)
        : reader{reader}
        , timeSpan{timeSpan}
        , handler{resultHandler}
    {
    }

    void execute() final { reader.requestItems(timeSpan, handler); }

private:
    IPomodoroStorageReader& reader;
    const TimeSpan timeSpan;
    std::function<void(const std::vector<Pomodoro>&)> handler;
};


class AddPomodoroTransaction : public ITransaction {
public:
    AddPomodoroTransaction(IPomodoroStorageWriter& gateway,
                           const Pomodoro& pomodoro)
        : gateway{gateway}
        , pomodoro{pomodoro}
    {
    }

    bool execute() final
    {
        wasExecuted = gateway.save(pomodoro);
        return wasExecuted;
    }

    bool undo() final
    {
        if (wasExecuted) {
            return gateway.remove(pomodoro);
        }
        return false;
    }

private:
    IPomodoroStorageWriter& gateway;
    const Pomodoro& pomodoro;
    bool wasExecuted{false};
};


// bool addPomodoro(const Pomodoro& pomodoro);
//
class RemovePomodoroTransaction : public ITransaction {
public:
    RemovePomodoroTransaction(IPomodoroStorageWriter& gateway,
                              const Pomodoro& pomodoro)
        : gateway{gateway}
        , pomodoro{pomodoro}
    {
    }

    bool execute() final
    {
        wasExecuted = gateway.remove(pomodoro);
        return wasExecuted;
    }

    bool undo() final
    {
        if (wasExecuted) {
            return gateway.save(pomodoro);
        }
        return false;
    }

private:
    IPomodoroStorageWriter& gateway;
    const Pomodoro& pomodoro;
    bool wasExecuted{false};
};


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


class ITaskStorageWriter {
public:
    virtual ~ITaskStorageWriter() = default;
    virtual bool save(const TodoItem& task) = 0;
    virtual bool remove(const TodoItem& task) = 0;
    virtual bool edit(const TodoItem& task, const TodoItem& editedTodoItem) = 0;
};


class AddTaskTransaction : public ITransaction {
public:
    AddTaskTransaction(ITaskStorageWriter& gateway, const TodoItem& task)
        : gateway{gateway}
        , task{task}
    {
    }

    bool execute() final
    {
        wasExecuted = gateway.save(task);
        return wasExecuted;
    }

    bool undo() final
    {
        if (wasExecuted) {
            return gateway.remove(task);
        }
        return false;
    }

private:
    ITaskStorageWriter& gateway;
    const TodoItem& task;
    bool wasExecuted{false};
};

/* Task use cases */

bool addTask(const TodoItem& task);

bool removeTask(const TodoItem& task);

bool completeTask(const TodoItem& task);

bool editTask(const TodoItem& task);

std::vector<TodoItem> taskInInterval(const TimeSpan& timeSpan);

std::vector<TodoItem> nonCompletedTasks();


} // namespace UseCases

#endif /* end of include guard: USE_CASES_H_FWYXSOEZ */
