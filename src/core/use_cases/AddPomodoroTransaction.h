#ifndef ADDPOMODOROTRANSACTION_H_YUMZVLHC
#define ADDPOMODOROTRANSACTION_H_YUMZVLHC

#include "core/IPomodoroStorageWriter.h"
#include "core/ITransaction.h"

namespace UseCases {

class AddPomodoroTransaction : public ITransaction {
public:
    AddPomodoroTransaction(IPomodoroStorageWriter& writer,
                           const Pomodoro& pomodoro,
                           long long associatedTaskId)
        : writer{writer}
        , pomodoro{pomodoro}
        , taskId{associatedTaskId}
    {
    }

    bool execute() final
    {
        writer.save(pomodoro, taskId);
        wasExecuted = true;
        return true;
    }

    bool undo() final
    {
        if (wasExecuted) {
            writer.remove(pomodoro);
            wasExecuted = false;
            return true;
        }
        return false;
    }

private:
    IPomodoroStorageWriter& writer;
    const Pomodoro& pomodoro;
    long long taskId;
    bool wasExecuted{false};
};

} /* UseCases */


#endif /* end of include guard: ADDPOMODOROTRANSACTION_H_YUMZVLHC */
