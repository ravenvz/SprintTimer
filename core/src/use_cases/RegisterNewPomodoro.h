#ifndef ADDPOMODOROTRANSACTION_H_YUMZVLHC
#define ADDPOMODOROTRANSACTION_H_YUMZVLHC

#include "core/IPomodoroStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {

class RegisterNewPomodoro : public RevertableCommand {
public:
    RegisterNewPomodoro(IPomodoroStorageWriter& writer,
                        const Pomodoro& pomodoro)
        : writer{writer}
        , pomodoro{pomodoro}
    {
    }

    void executeAction() final { writer.save(pomodoro); }

    void undoAction() final { writer.remove(pomodoro); }

    std::string inspect() const final
    {
        return "Register new pomodoro '" + pomodoro.toString() + "'";
    }

private:
    IPomodoroStorageWriter& writer;
    const Pomodoro pomodoro;
};

} /* UseCases */


#endif /* end of include guard: ADDPOMODOROTRANSACTION_H_YUMZVLHC */
