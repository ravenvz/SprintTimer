#ifndef ADDTASKTRANSACTION_H_LEMDIQ17
#define ADDTASKTRANSACTION_H_LEMDIQ17

#include "core/ITaskStorageWriter.h"
#include "core/RevertableCommand.h"
#include "core/entities/TodoItem.h"

namespace UseCases {

class AddTaskTransaction : public RevertableCommand {
public:
    AddTaskTransaction(ITaskStorageWriter& taskStorageWriter,
                       const TodoItem& task)
        : writer{taskStorageWriter}
        , task{task}
    {
    }

    void executeAction() final { writer.save(task); }

    void undoAction() final { writer.remove(task); }

private:
    ITaskStorageWriter& writer;
    const TodoItem& task;
};


} /* UseCases */


#endif /* end of include guard: ADDTASKTRANSACTION_H_LEMDIQ17 */
