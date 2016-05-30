#ifndef REMOVETASKTRANSACTION_H_9P4V02DM
#define REMOVETASKTRANSACTION_H_9P4V02DM

#include "core/ITaskStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {

class RemoveTaskTransaction : public RevertableCommand {
public:
    RemoveTaskTransaction(ITaskStorageWriter& taskStorageWriter,
                          const TodoItem& taskToRemove)
        : writer{taskStorageWriter}
        , task{taskToRemove}
    {
    }

    void executeAction() final { writer.remove(task); }

    void undoAction() final { writer.save(task); }

private:
    ITaskStorageWriter& writer;
    const TodoItem& task;
};
} /* UseCases */

#endif /* end of include guard: REMOVETASKTRANSACTION_H_9P4V02DM */
