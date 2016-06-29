#ifndef COMMANDINVOKER_H_WGTA1XLU
#define COMMANDINVOKER_H_WGTA1XLU

#include "core/RevertableCommand.h"
#include <iostream>
#include <memory>
#include <vector>

class CommandInvoker {
public:
    void executeCommand(std::unique_ptr<Command>&& command)
    {
        command->execute();
        std::cout << "Executing " << command->inspect() << " command"
                  << std::endl;
        if (command->supportUndo())
            commandStack.push_back(std::move(command));
    }

    void undo()
    {
        if (commandStack.empty())
            return;
        std::cout << "Undoing " << commandStack.back()->inspect() << " command"
                  << std::endl;
        commandStack.back()->undo();
        commandStack.pop_back();
    }

    size_t stackSize() const { return commandStack.size(); }

    std::string lastCommandDescription() const {
        if (commandStack.empty())
            return "";
        return commandStack.back()->inspect(); }

private:
    std::vector<std::unique_ptr<Command>> commandStack;
};

#endif /* end of include guard: COMMANDINVOKER_H_WGTA1XLU */
