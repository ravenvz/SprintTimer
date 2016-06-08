#ifndef REQUESTFINISHEDTASKSCOMMAND_H_AF6E0NUL
#define REQUESTFINISHEDTASKSCOMMAND_H_AF6E0NUL

#include "core/Command.h"
#include "core/ITaskStorageReader.h"

namespace UseCases {

class RequestFinishedTasks : public Command {
public:
    RequestFinishedTasks(ITaskStorageReader& taskStorageReader,
                         const TimeSpan& timeSpan,
                         ITaskStorageReader::Handler handler)
        : reader{taskStorageReader}
        , timeSpan{timeSpan}
        , handler{handler}
    {
    }

    void execute() final { reader.requestFinishedTasks(timeSpan, handler); }

    std::string inspect() const final
    {
        return "Request finished tasks in 'placeholder'";
    }

private:
    ITaskStorageReader& reader;
    const TimeSpan timeSpan;
    ITaskStorageReader::Handler handler;
};

} /* UseCases */

#endif /* end of include guard: REQUESTFINISHEDTASKSCOMMAND_H_AF6E0NUL */
