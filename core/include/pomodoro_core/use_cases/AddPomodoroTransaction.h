#ifndef ADDPOMODOROTRANSACTION_H_YUMZVLHC
#define ADDPOMODOROTRANSACTION_H_YUMZVLHC

#include "core/IPomodoroStorageWriter.h"
#include "core/ITransaction.h"

namespace UseCases {

class AddPomodoroTransaction : public ITransaction {
public:
    AddPomodoroTransaction(IPomodoroStorageWriter& writer,
                           const Pomodoro& pomodoro)
        : writer{writer}
        , pomodoro{pomodoro}
    {
    }

    bool execute() final
    {
        writer.save(pomodoro);
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
    bool wasExecuted{false};
};

} /* UseCases */


#endif /* end of include guard: ADDPOMODOROTRANSACTION_H_YUMZVLHC */
