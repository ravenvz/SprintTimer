#ifndef EDITTASKCOMMAND_H_3FYYCQWP
#define EDITTASKCOMMAND_H_3FYYCQWP

#include "core/ITaskStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {

class EditTaskCommand : public RevertableCommand {
public:
    EditTaskCommand(ITaskStorageWriter& writer,
                    const TodoItem& task,
                    const TodoItem& editedTask)
        : writer{writer}
        , task{task}
        , editedTask{editedTask.name(),
                     editedTask.estimatedPomodoros(),
                     task.spentPomodoros(),
                     task.uuid(),
                     editedTask.tags(),
                     task.isCompleted(),
                     DateTime::currentDateTimeLocal()}
    {
    }

    void executeAction() final { writer.edit(task, editedTask); }

    void undoAction() final { writer.edit(editedTask, task); }


private:
    ITaskStorageWriter& writer;
    TodoItem task;
    TodoItem editedTask;
};

} /* UseCases */

#endif /* end of include guard: EDITTASKCOMMAND_H_3FYYCQWP */
