#ifndef REMOVEPOMODOROTRANSACTION_H_GAR8SZLM
#define REMOVEPOMODOROTRANSACTION_H_GAR8SZLM

#include "core/IPomodoroStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {

class RemovePomodoroTransaction : public RevertableCommand {
public:
    RemovePomodoroTransaction(IPomodoroStorageWriter& writer,
                              const Pomodoro& pomodoro)
        : writer{writer}
        , pomodoro{pomodoro}
    {
    }

    void executeAction() final { writer.remove(pomodoro); }

    void undoAction() final { writer.save(pomodoro); }

    std::string inspect() const final
    {
        return "Remove Pomodoro '" + pomodoro.toString() + "'";
    }

private:
    IPomodoroStorageWriter& writer;
    const Pomodoro& pomodoro;
};

} // namespace UseCases

#endif /* end of include guard: REMOVEPOMODOROTRANSACTION_H_GAR8SZLM */
