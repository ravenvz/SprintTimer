#ifndef REQUESTUNFINISHEDTASKSCOMMAND_H_AGF8KFHK
#define REQUESTUNFINISHEDTASKSCOMMAND_H_AGF8KFHK

#include "core/Command.h"

namespace UseCases {

class RequestUnfinishedTasks : public Command {
public:
    RequestUnfinishedTasks(ITaskStorageReader& taskStorageReader,
                           ITaskStorageReader::Handler handler)
        : reader{taskStorageReader}
        , handler{handler}
    {
    }

    void execute() final { reader.requestUnfinishedTasks(handler); }

    std::string inspect() const final { return "Request unfinished tasks"; }

private:
    ITaskStorageReader& reader;
    ITaskStorageReader::Handler handler;
};

} /* UseCases */

#endif /* end of include guard: REQUESTUNFINISHEDTASKSCOMMAND_H_AGF8KFHK */
