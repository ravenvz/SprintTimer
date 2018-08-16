/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#ifndef ICOMMANDINVOKER_H_EQKZS68F
#define ICOMMANDINVOKER_H_EQKZS68F

#include "core/Command.h"
#include <memory>
#include <string>

namespace sprint_timer {

class ICommandInvoker {
public:
    virtual ~ICommandInvoker() = default;
    virtual void executeCommand(std::unique_ptr<Command> command) = 0;
    virtual void undo() = 0;
    virtual std::string lastCommandDescription() const = 0;
    virtual std::size_t stackSize() const = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: ICOMMANDINVOKER_H_EQKZS68F */
