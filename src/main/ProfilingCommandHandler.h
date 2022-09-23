/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#ifndef PROFILINGCOMMANDHANDLER_H_PNRMO94Z
#define PROFILINGCOMMANDHANDLER_H_PNRMO94Z

#include "api/com_query/Command.h"
#include "api/com_query/RequestHandlerDecorator.h"
#include <mutex>

namespace sprint_timer::compose {

template <asp::Command CommandT>
class ProfilingCommandHandler : public asp::RequestHandlerDecorator<CommandT> {
public:
    ProfilingCommandHandler(
        std::unique_ptr<asp::RequestHandler<CommandT>> wrapped_,
        std::ostream& outputStream_)
        : asp::RequestHandlerDecorator<CommandT>{std::move(wrapped_)}
        , os{outputStream_}
    {
    }

    CommandT::Result handle(const CommandT& command) override
    {
        const auto start = std::chrono::steady_clock::now();
        asp::RequestHandlerDecorator<CommandT>::handle(command);
        const auto end = std::chrono::steady_clock::now();

        {
            std::lock_guard lock{mtx};
            os << "Handled command: " << command << " in ";
            os << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                        start)
                      .count();
            os << "ms\n";
        }
    }

private:
    std::ostream& os;
    std::mutex mtx;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: PROFILINGCOMMANDHANDLER_H_PNRMO94Z */
