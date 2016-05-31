#ifndef REQUESTTASKSCOMMAND_H_ROJXTM6V
#define REQUESTTASKSCOMMAND_H_ROJXTM6V

#include "core/ICommand.h"
#include "core/ITaskStorageReader.h"

namespace UseCases {

class RequestTasksCommand : public ICommand {
public:
    RequestTasksCommand(ITaskStorageReader& taskStorageReader,
                        const TimeSpan& timeSpan,
                        ITaskStorageReader::Handler handler)
        : reader{taskStorageReader}
        , timeSpan{timeSpan}
        , handler{handler}
    {
    }

    void execute() final { reader.requestTasks(timeSpan, handler); }

private:
    ITaskStorageReader& reader;
    const TimeSpan timeSpan;
    ITaskStorageReader::Handler handler;
};
} /* UseCases */

#endif /* end of include guard: REQUESTTASKSCOMMAND_H_ROJXTM6V */
