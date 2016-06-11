#ifndef ITASKSTORAGEWRITER_H_PVAMCJ6G
#define ITASKSTORAGEWRITER_H_PVAMCJ6G

#include "core/entities/TodoItem.h"

class ITaskStorageWriter {
public:
    virtual ~ITaskStorageWriter() = default;

    virtual void save(const TodoItem& task) = 0;

    virtual void remove(const TodoItem& task) = 0;

    virtual void edit(const TodoItem& task, const TodoItem& editedTask) = 0;

    virtual void incrementSpentPomodoros(const std::string& uuid) = 0;

    virtual void decrementSpentPomodoros(const std::string& uuid) = 0;

    virtual void toggleTaskCompletionStatus(const std::string& uuid,
                                            const DateTime& timeStamp)
        = 0;

    virtual void
    updatePriorities(std::vector<std::pair<std::string, int>>&& priorities)
        = 0;

    virtual void editTag(const std::string& oldName, const std::string& newName)
        = 0;
};

#endif /* end of include guard: ITASKSTORAGEWRITER_H_PVAMCJ6G */
