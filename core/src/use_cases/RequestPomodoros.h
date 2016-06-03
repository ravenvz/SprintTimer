#ifndef REQUESTPOMODOROSINTIMERANGECOMMAND_H_4QUSWCF0
#define REQUESTPOMODOROSINTIMERANGECOMMAND_H_4QUSWCF0


#include "core/ICommand.h"
#include "core/IPomodoroStorageReader.h"

namespace UseCases {

class RequestPomodoros : public ICommand {
public:
    RequestPomodoros(
        IPomodoroStorageReader& reader,
        const TimeSpan& timeSpan,
        std::function<void(const std::vector<Pomodoro>&)> resultHandler)
        : reader{reader}
        , timeSpan{timeSpan}
        , handler{resultHandler}
    {
    }

    void execute() final { reader.requestItems(timeSpan, handler); }

private:
    IPomodoroStorageReader& reader;
    const TimeSpan timeSpan;
    std::function<void(const std::vector<Pomodoro>&)> handler;
};

} /* UseCases */

#endif /* end of include guard: REQUESTPOMODOROSINTIMERANGECOMMAND_H_4QUSWCF0  \
          */
