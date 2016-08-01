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