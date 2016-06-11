#ifndef INCREMENTSPENTPOMODOROS_H_2OKTXAHD
#define INCREMENTSPENTPOMODOROS_H_2OKTXAHD

#include "core/ITaskStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {
class IncrementSpentPomodoros : public RevertableCommand {
public:
    IncrementSpentPomodoros(ITaskStorageWriter& taskStorageWriter,
                            const std::string& taskUuid)
        : writer{taskStorageWriter}
        , taskUuid{taskUuid}
    {
    }

    std::string inspect() const final
    {
        return "Increment spent pomodoros for " + taskUuid;
    }

protected:
    void executeAction() final { writer.incrementSpentPomodoros(taskUuid); }

    void undoAction() final { writer.decrementSpentPomodoros(taskUuid); }

private:
    ITaskStorageWriter& writer;
    const std::string& taskUuid;
};

} /* UseCases */

#endif /* end of include guard: INCREMENTSPENTPOMODOROS_H_2OKTXAHD */
