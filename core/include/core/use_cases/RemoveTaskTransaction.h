#ifndef REMOVETASKTRANSACTION_H_9P4V02DM
#define REMOVETASKTRANSACTION_H_9P4V02DM

#include "core/ITaskStorageWriter.h"
#include "core/ITransaction.h"

namespace UseCases {

class RemoveTaskTransaction : public ITransaction {
public:
    RemoveTaskTransaction(ITaskStorageWriter& taskStorageWriter,
                          const TodoItem& taskToRemove)
        : writer{taskStorageWriter}
        , task{taskToRemove}
    {
    }

    bool execute() final
    {
        writer.remove(task);
        wasExecuted = true;
        return true;
    }

    bool undo() final
    {
        if (wasExecuted) {
            writer.save(task);
            wasExecuted = false;
            return true;
        }
        return false;
    }

private:
    ITaskStorageWriter& writer;
    const TodoItem& task;
    bool wasExecuted{false};
};
} /* UseCases */

#endif /* end of include guard: REMOVETASKTRANSACTION_H_9P4V02DM */
