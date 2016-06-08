#ifndef IMACROTRANSACTION_H_GKJDX3V4
#define IMACROTRANSACTION_H_GKJDX3V4

#include "core/RevertableCommand.h"
#include <memory>
#include <vector>

class MacroTransaction : public RevertableCommand {
public:
    MacroTransaction(std::vector<std::unique_ptr<RevertableCommand>>&& commands)
        : commands{std::move(commands)}
    {
    }

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

    std::string inspect() const final
    {
        std::vector<std::string> descriptions{"Macro command"};
        std::transform(commands.cbegin(),
                       commands.cend(),
                       std::back_inserter(descriptions),
                       [](const auto& command) { return command->inspect(); });
        return StringUtils::join(
            descriptions.cbegin(), descriptions.cend(), " ");
    }

    void addCommand(std::unique_ptr<RevertableCommand> command)
    {
        commands.push_back(std::move(command));
    }

private:
    std::vector<std::unique_ptr<RevertableCommand>> commands;
};

#endif /* end of include guard: IMACROTRANSACTION_H_GKJDX3V4 */
