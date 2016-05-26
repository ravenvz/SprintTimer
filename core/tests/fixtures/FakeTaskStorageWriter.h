#ifndef FAKETASKSTORAGEWRITER_H_L9TXWRHX
#define FAKETASKSTORAGEWRITER_H_L9TXWRHX

#include "core/ITaskStorageWriter.h"
#include "fixtures/FakeStorage.h"

class FakeTaskStorageWriter : public ITaskStorageWriter {
public:
    FakeTaskStorageWriter(FakeStorage<TodoItem>& storage)
        : storage{storage}
    {
    }

    void save(const TodoItem& task) final { storage.store(task); }

    void remove(const TodoItem& task) final { storage.remove(task.uuid()); }

    void edit(const TodoItem& task, const TodoItem& editedTask) final {}

    void incrementSpentPomodoros(const std::string& uuid) final
    {
        TodoItem& item = storage.itemRef(uuid);
        item.setSpentPomodoros(item.spentPomodoros() + 1);
    }

    void decrementSpentPomodoros(const std::string& uuid) final
    {
        TodoItem& item = storage.itemRef(uuid);
        item.setSpentPomodoros(item.spentPomodoros() - 1);
    }

    FakeStorage<TodoItem>& storage;
};

#endif /* end of include guard: FAKETASKSTORAGEWRITER_H_L9TXWRHX */
