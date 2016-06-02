#ifndef FAKETASKSTORAGEWRITER_H_L9TXWRHX
#define FAKETASKSTORAGEWRITER_H_L9TXWRHX

#include "FakeStorage.h"
#include "core/ITaskStorageWriter.h"

class FakeTaskStorageWriter : public ITaskStorageWriter {
public:
    FakeTaskStorageWriter(FakeStorage<TodoItem>& storage)
        : storage{storage}
    {
    }

    void save(const TodoItem& task) final { storage.store(task); }

    void remove(const TodoItem& task) final { storage.remove(task.uuid()); }

    void edit(const TodoItem& task, const TodoItem& editedTask) final
    {
        storage.itemRef(task.uuid()) = editedTask;
    }

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

    void toggleTaskCompletionStatus(const std::string& uuid,
                                    const DateTime& timeStamp) final
    {
        TodoItem& taskRef = storage.itemRef(uuid);
        taskRef.setCompleted(!taskRef.isCompleted());
        taskRef.setModifiedTimeStamp(timeStamp);
    }

    FakeStorage<TodoItem>& storage;
};

#endif /* end of include guard: FAKETASKSTORAGEWRITER_H_L9TXWRHX */
