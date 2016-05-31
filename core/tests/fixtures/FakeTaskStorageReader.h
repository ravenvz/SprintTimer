#ifndef FAKETASKSTORAGEREADER_H_EDPTGODR
#define FAKETASKSTORAGEREADER_H_EDPTGODR

#include "FakeStorage.h"
#include "core/ITaskStorageReader.h"

class FakeTaskStorageReader : public ITaskStorageReader {
public:
    FakeTaskStorageReader(FakeStorage<TodoItem>& storage)
        : storage{storage}
    {
    }

    void requestTasks(const TimeSpan& timeSpan, Handler handler) final
    {
        storage.itemsInTimeRange(timeSpan, handler);
    }

    void requestUnfinishedTasks(Handler handler) final
    {
        std::vector<TodoItem> emptyResult;
        handler(emptyResult);
    }

    void requestFinishedTasks(const TimeSpan& timeSpan, Handler handler) final
    {
        storage.itemsInTimeRange(timeSpan, handler);
    }

private:
    FakeStorage<TodoItem>& storage;
};

#endif /* end of include guard: FAKETASKSTORAGEREADER_H_EDPTGODR */
