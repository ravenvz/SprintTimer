#ifndef FAKETASKSTORAGEWRITER_H_L9TXWRHX
#define FAKETASKSTORAGEWRITER_H_L9TXWRHX

#include "core/ITaskStorageWriter.h"
#include "fixtures/FakeStorage.h"

class FakeTaskStorageWriter : public ITaskStorageWriter {
public:
    void save(const TodoItem& task) final { storage.store(task); }

    void remove(const TodoItem& task) final { storage.remove(task.uuid()); }

    void edit(const TodoItem& task, const TodoItem& editedTask) final {}

    FakeStorage<TodoItem> storage;
};

#endif /* end of include guard: FAKETASKSTORAGEWRITER_H_L9TXWRHX */
