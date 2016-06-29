#ifndef FAKETASKSTORAGEREADER_H_EDPTGODR
#define FAKETASKSTORAGEREADER_H_EDPTGODR

#include "FakeStorage.h"
#include "core/ITaskStorageReader.h"

class FakeTaskStorageReader : public ITaskStorageReader {
public:
    FakeTaskStorageReader(FakeStorage<Task>& storage)
        : storage{storage}
    {
    }

    void requestUnfinishedTasks(Handler handler) final
    {
        std::vector<Task> emptyResult;
        handler(emptyResult);
    }

    void requestFinishedTasks(const TimeSpan& timeSpan, Handler handler) final
    {
        storage.itemsInTimeRange(timeSpan, handler);
    }

    void requestAllTags(TagHandler handler)
    {
        const std::vector<std::string> emptyResult;
        handler(emptyResult);
    }

private:
    FakeStorage<Task>& storage;
};

#endif /* end of include guard: FAKETASKSTORAGEREADER_H_EDPTGODR */
