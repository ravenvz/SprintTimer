#ifndef REQUESTFINISHEDTASKSCOMMAND_H_AF6E0NUL
#define REQUESTFINISHEDTASKSCOMMAND_H_AF6E0NUL

#include "core/ICommand.h"
#include "core/ITaskStorageReader.h"

namespace UseCases {

class RequestFinishedTasksCommand : public ICommand {
public:
    RequestFinishedTasksCommand(ITaskStorageReader& taskStorageReader,
                                const TimeSpan& timeSpan,
                                ITaskStorageReader::Handler handler)
        : reader{taskStorageReader}
        , timeSpan{timeSpan}
        , handler{handler}
    {
    }

    void execute() final { reader.requestFinishedTasks(timeSpan, handler); }

private:
    ITaskStorageReader& reader;
    const TimeSpan timeSpan;
    ITaskStorageReader::Handler handler;
};

} /* UseCases */

#endif /* end of include guard: REQUESTFINISHEDTASKSCOMMAND_H_AF6E0NUL */
