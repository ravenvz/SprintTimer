#ifndef REGISTERTASKPRIORITIESCOMMAND_H_AGQJD71Y
#define REGISTERTASKPRIORITIESCOMMAND_H_AGQJD71Y

#include "core/ICommand.h"
#include "core/ITaskStorageWriter.h"

namespace UseCases {

class RegisterTaskPrioritiesCommand : public ICommand {
public:
    RegisterTaskPrioritiesCommand(
        ITaskStorageWriter& taskStorageWriter,
        std::vector<std::pair<std::string, int>>&& priorities)
        : writer{taskStorageWriter}
        , priorities{std::move(priorities)}
    {
    }

    void execute() { writer.updatePriorities(std::move(priorities)); }

private:
    ITaskStorageWriter& writer;
    std::vector<std::pair<std::string, int>> priorities;
};
} /* UseCases */

#endif /* end of include guard: REGISTERTASKPRIORITIESCOMMAND_H_AGQJD71Y */
