#ifndef TOGGLETASKCOMPLETEDCOMMAND_H_CAT08NRT
#define TOGGLETASKCOMPLETEDCOMMAND_H_CAT08NRT

#include "core/ITaskStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {

class ToggleTaskCompletionStatus : public RevertableCommand {
public:
    ToggleTaskCompletionStatus(ITaskStorageWriter& taskStorageWriter,
                               const TodoItem& task)
        : writer{taskStorageWriter}
        , uuid{task.uuid()}
        , oldTimeStamp{task.lastModified()}
    {
    }

    void executeAction() final
    {
        writer.toggleTaskCompletionStatus(uuid,
                                          DateTime::currentDateTimeLocal());
    }

    void undoAction() final
    {
        writer.toggleTaskCompletionStatus(uuid, oldTimeStamp);
    }

    std::string inspect() const final
    {
        return "Toggle task completion for " + uuid;
    }

private:
    ITaskStorageWriter& writer;
    const std::string uuid;
    const DateTime oldTimeStamp;
};

} /* UseCases */

#endif /* end of include guard: TOGGLETASKCOMPLETEDCOMMAND_H_CAT08NRT */
