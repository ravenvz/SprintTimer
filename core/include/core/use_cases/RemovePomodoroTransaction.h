#ifndef REMOVEPOMODOROTRANSACTION_H_GAR8SZLM
#define REMOVEPOMODOROTRANSACTION_H_GAR8SZLM

#include "core/IPomodoroStorageWriter.h"
#include "core/ITransaction.h"

namespace UseCases {

class RemovePomodoroTransaction : public ITransaction {
public:
    RemovePomodoroTransaction(IPomodoroStorageWriter& writer,
                              const Pomodoro& pomodoro)
        : writer{writer}
        , pomodoro{pomodoro}
    {
    }

    bool execute() final
    {
        writer.remove(pomodoro);
        wasExecuted = true;
        return true;
    }

    bool undo() final
    {
        if (wasExecuted) {
            writer.save(pomodoro);
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

} // namespace UseCases

#endif /* end of include guard: REMOVEPOMODOROTRANSACTION_H_GAR8SZLM */
