/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
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
