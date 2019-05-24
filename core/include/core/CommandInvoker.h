/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef COMMANDINVOKER_H_WGTA1XLU
#define COMMANDINVOKER_H_WGTA1XLU

#include "core/Command.h"
#include "core/ICommandInvoker.h"
#include "core/Observable.h"
#include <iostream>
#include <memory>
#include <stack>
#include <vector>

namespace sprint_timer {

class CommandInvoker : public ICommandInvoker, public Observable {
public:
    void executeCommand(std::unique_ptr<Command> command) override;

    void undo() override;

    std::string lastCommandDescription() const override;

    bool hasUndoableCommands() const override;

private:
    std::stack<std::unique_ptr<Command>> commandStack;
};

} // namespace sprint_timer

#endif /* end of include guard: COMMANDINVOKER_H_WGTA1XLU */
