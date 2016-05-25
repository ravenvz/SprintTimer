#ifndef ADDTASKTRANSACTION_H_LEMDIQ17
#define ADDTASKTRANSACTION_H_LEMDIQ17

#include "core/ITaskStorageWriter.h"
#include "core/ITransaction.h"
#include "core/entities/TodoItem.h"

namespace UseCases {

class AddTaskTransaction : public ITransaction {
public:
    AddTaskTransaction(ITaskStorageWriter& taskStorageWriter,
                       const TodoItem& task)
        : writer{taskStorageWriter}
        , task{task}
    {
    }

    bool execute() final
    {
        writer.save(task);
        wasExecuted = true;
        return true;
    }

    bool undo() final
    {
        if (wasExecuted) {
            writer.remove(task);
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


#endif /* end of include guard: ADDTASKTRANSACTION_H_LEMDIQ17 */
