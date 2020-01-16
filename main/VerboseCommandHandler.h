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
#ifndef VERBOSECOMMANDHANDLER_H_VSD9NQOL
#define VERBOSECOMMANDHANDLER_H_VSD9NQOL

#include <core/CommandHandler.h>
#include <iostream>
#include <memory>

namespace sprint_timer {

template <typename CommandT>
class VerboseCommandHandler : public CommandHandler<CommandT> {
public:
    VerboseCommandHandler(std::unique_ptr<CommandHandler<CommandT>> wrapped);

    void handle(CommandT&& command) override;

private:
    std::unique_ptr<CommandHandler<CommandT>> wrapped;
};

template <typename CommandT>
VerboseCommandHandler<CommandT>::VerboseCommandHandler(
    std::unique_ptr<CommandHandler<CommandT>> wrapped_)
    : wrapped{std::move(wrapped_)}
{
}

template <typename CommandT>
void VerboseCommandHandler<CommandT>::handle(CommandT&& command)
{
    std::cout << "Handling command: " << command << '\n';
    wrapped->handle(std::move(command));
}

} // namespace sprint_timer

#endif /* end of include guard: VERBOSECOMMANDHANDLER_H_VSD9NQOL */
