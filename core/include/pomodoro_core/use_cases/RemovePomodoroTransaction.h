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
        return true;
    }

    bool undo() final
    {
        // if (pomoData) {
        //     long long taskId = (*pomoData).first;
        //     TimeSpan timeSpan = (*pomoData).second;
        //     pomoData = optional<std::pair<long long, TimeSpan>>();
        //     return bool(writer.save(taskId, timeSpan));
        // }
        return false;
    }

private:
    IPomodoroStorageWriter& writer;
    const Pomodoro& pomodoro;
};

} // namespace UseCases

#endif /* end of include guard: REMOVEPOMODOROTRANSACTION_H_GAR8SZLM */
