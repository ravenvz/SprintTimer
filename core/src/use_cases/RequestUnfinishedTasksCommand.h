#ifndef REQUESTUNFINISHEDTASKSCOMMAND_H_AGF8KFHK
#define REQUESTUNFINISHEDTASKSCOMMAND_H_AGF8KFHK

#include "core/ICommand.h"

namespace UseCases {

class RequestUnfinishedTasksCommand : public ICommand {
public:
    RequestUnfinishedTasksCommand(ITaskStorageReader& taskStorageReader,
                                  ITaskStorageReader::Handler handler)
        : reader{taskStorageReader}
        , handler{handler}
    {
    }

    void execute() final { reader.requestUnfinishedTasks(handler); }

private:
    ITaskStorageReader& reader;
    ITaskStorageReader::Handler handler;
};

} /* UseCases */

#endif /* end of include guard: REQUESTUNFINISHEDTASKSCOMMAND_H_AGF8KFHK */
