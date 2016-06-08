#ifndef REGISTERTASKPRIORITIESCOMMAND_H_AGQJD71Y
#define REGISTERTASKPRIORITIESCOMMAND_H_AGQJD71Y

#include "core/Command.h"
#include "core/ITaskStorageWriter.h"

namespace UseCases {

class StoreUnfinishedTasksOrder : public Command {
public:
    StoreUnfinishedTasksOrder(
        ITaskStorageWriter& taskStorageWriter,
        std::vector<std::pair<std::string, int>>&& priorities)
        : writer{taskStorageWriter}
        , priorities{std::move(priorities)}
    {
    }

    void execute() { writer.updatePriorities(std::move(priorities)); }

    std::string inspect() const final { return "Store unfinished tasks order"; }

private:
    ITaskStorageWriter& writer;
    std::vector<std::pair<std::string, int>> priorities;
};
} /* UseCases */

#endif /* end of include guard: REGISTERTASKPRIORITIESCOMMAND_H_AGQJD71Y */
