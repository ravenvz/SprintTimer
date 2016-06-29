#ifndef EDITTASKCOMMAND_H_3FYYCQWP
#define EDITTASKCOMMAND_H_3FYYCQWP

#include "core/ITaskStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {

class EditTask : public RevertableCommand {
public:
    EditTask(ITaskStorageWriter& writer,
             const Task& task,
             const Task& editedTask)
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

    std::string inspect() const final
    {
        return "Edit task '" + task.toString() + " -> " + editedTask.toString()
            + "'";
    }


private:
    ITaskStorageWriter& writer;
    Task task;
    Task editedTask;
};

} /* UseCases */

#endif /* end of include guard: EDITTASKCOMMAND_H_3FYYCQWP */
