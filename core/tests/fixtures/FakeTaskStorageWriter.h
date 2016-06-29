#ifndef FAKETASKSTORAGEWRITER_H_L9TXWRHX
#define FAKETASKSTORAGEWRITER_H_L9TXWRHX

#include "FakeStorage.h"
#include "core/ITaskStorageWriter.h"

class FakeTaskStorageWriter : public ITaskStorageWriter {
public:
    FakeTaskStorageWriter(FakeStorage<Task>& storage)
        : storage{storage}
    {
    }

    void save(const Task& task) final { storage.store(task); }

    void remove(const Task& task) final { storage.remove(task.uuid()); }

    void edit(const Task& task, const Task& editedTask) final
    {
        storage.itemRef(task.uuid()) = editedTask;
    }

    void incrementSpentPomodoros(const std::string& uuid) final
    {
        Task& item = storage.itemRef(uuid);
        item.setSpentPomodoros(item.spentPomodoros() + 1);
    }

    void decrementSpentPomodoros(const std::string& uuid) final
    {
        Task& item = storage.itemRef(uuid);
        item.setSpentPomodoros(item.spentPomodoros() - 1);
    }

    void toggleTaskCompletionStatus(const std::string& uuid,
                                    const DateTime& timeStamp) final
    {
        Task& taskRef = storage.itemRef(uuid);
        taskRef.setCompleted(!taskRef.isCompleted());
        taskRef.setModifiedTimeStamp(timeStamp);
    }

    void updatePriorities(std::vector<std::pair<std::string, int>>&& priorities)
    {
    }

    void editTag(const std::string& oldName, const std::string& newName)
    {
        storage.editTag(oldName, newName);
    }

    FakeStorage<Task>& storage;
};

#endif /* end of include guard: FAKETASKSTORAGEWRITER_H_L9TXWRHX */
