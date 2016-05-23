#ifndef IMACROTRANSACTION_H_GKJDX3V4
#define IMACROTRANSACTION_H_GKJDX3V4

#include "core/RevertableCommand.h"
#include <memory>
#include <vector>

class MacroTransaction : public RevertableCommand {
public:
    void executeAction() final
    {
        for (const auto& command : commands) {
            command->execute();
        }
    }

    void undoAction() final
    {
        for (const auto& command : commands) {
            command->undo();
        }
    }

    void addCommand(std::unique_ptr<RevertableCommand> command)
    {
        commands.push_back(std::move(command));
    }

private:
    std::vector<std::unique_ptr<RevertableCommand>> commands;
};

#endif /* end of include guard: IMACROTRANSACTION_H_GKJDX3V4 */
