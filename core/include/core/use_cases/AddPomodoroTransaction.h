#ifndef ADDPOMODOROTRANSACTION_H_YUMZVLHC
#define ADDPOMODOROTRANSACTION_H_YUMZVLHC

#include "core/IPomodoroStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {

class AddPomodoroTransaction : public RevertableCommand {
public:
    AddPomodoroTransaction(IPomodoroStorageWriter& writer,
                           const Pomodoro& pomodoro)
        : writer{writer}
        , pomodoro{pomodoro}
    {
    }

    void executeAction() final { writer.save(pomodoro); }

    void undoAction() final { writer.remove(pomodoro); }

private:
    IPomodoroStorageWriter& writer;
    const Pomodoro& pomodoro;
};

} /* UseCases */


#endif /* end of include guard: ADDPOMODOROTRANSACTION_H_YUMZVLHC */
