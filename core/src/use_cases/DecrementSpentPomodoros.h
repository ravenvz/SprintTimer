#ifndef DECREMENTSPENTPOMODOROS_H_GYPTACBX
#define DECREMENTSPENTPOMODOROS_H_GYPTACBX

#include "core/ITaskStorageWriter.h"

namespace UseCases {


class DecrementSpentPomodoros : public RevertableCommand {
public:
    DecrementSpentPomodoros(ITaskStorageWriter& taskStorageWriter,
                            const std::string& taskUuid)
        : writer{taskStorageWriter}
        , taskUuid{taskUuid}
    {
    }

    void executeAction() final { writer.decrementSpentPomodoros(taskUuid); }

    void undoAction() final { writer.incrementSpentPomodoros(taskUuid); }

    std::string inspect() const final
    {
        return "Decrement spent pomodoros for " + taskUuid;
    }

private:
    ITaskStorageWriter& writer;
    const std::string& taskUuid;
};

} /* UseCases */

#endif /* end of include guard: DECREMENTSPENTPOMODOROS_H_GYPTACBX */
