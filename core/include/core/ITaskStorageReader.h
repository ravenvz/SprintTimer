#ifndef ITASKSTORAGEREADER_H_RMTKEREJ
#define ITASKSTORAGEREADER_H_RMTKEREJ

#include "core/TimeSpan.h"
#include "core/entities/TodoItem.h"
#include <functional>

class ITaskStorageReader {
public:
    using Items = std::vector<TodoItem>;

    using Handler = std::function<void(const Items&)>;

    virtual ~ITaskStorageReader() = default;

    virtual void requestUnfinishedTasks(Handler handler) = 0;

    virtual void requestFinishedTasks(const TimeSpan& timeSpan, Handler handler)
        = 0;
};

#endif /* end of include guard: ITASKSTORAGEREADER_H_RMTKEREJ */
