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
#ifndef COMMANDHANDLERDECORATOR_H_GF3JAIOA
#define COMMANDHANDLERDECORATOR_H_GF3JAIOA

#include "CacheAwareCommandHandler.h"
#include "ErrorReportingCommandHandler.h"
#include "ProfilingRequestHandler.h"
#include "VerboseRequestHandler.h"

namespace sprint_timer::compose {

template <asp::Command CommandT>
std::unique_ptr<asp::RequestHandler<CommandT>>
decorate_command(std::unique_ptr<asp::RequestHandler<CommandT>> wrapped,
                 std::ostream& os,
                 ui::Mediator<ui::Invalidatable>& cacheInvalidationMediator)
{
    auto cacheAwareHandler =
        std::make_unique<CacheAwareCommandHandler<CommandT>>(
            std::move(wrapped), cacheInvalidationMediator);
    auto verboseHandler = std::make_unique<VerboseRequestHandler<CommandT>>(
        std::move(cacheAwareHandler), os);
    auto errorReportingHandler =
        std::make_unique<ErrorReportingCommandHandler<CommandT>>(
            std::move(verboseHandler));
    return errorReportingHandler;
}

// This overload is useful for commands that do not mutate internal state (so no
// synchronization is needed).
template <asp::Command CommandT>
std::unique_ptr<asp::RequestHandler<CommandT>> decorate_immutable_command(
    std::unique_ptr<asp::RequestHandler<CommandT>> wrapped, std::ostream& os)
{
    auto verboseHandler = std::make_unique<VerboseRequestHandler<CommandT>>(
        std::move(wrapped), os);
    auto errorReportingHandler =
        std::make_unique<ErrorReportingCommandHandler<CommandT>>(
            std::move(verboseHandler));
    return errorReportingHandler;
}

// template <asp::Request CommandT>
// std::unique_ptr<asp::RequestHandler<CommandT>>
// decorate_com_handler(std::unique_ptr<asp::RequestHandler<CommandT>> wrapped,
//                      std::ostream& os)
// {
//     return std::make_unique<ErrorReportingRequestHandler<CommandT>>(
//         std::make_unique<sprint_timer::VerboseRequestHandler<CommandT>>(
//             std::move(wrapped), os));
//     // return std::make_unique<ErrorReportingCommandHandler<CommandT>>(
//     //
//     std::make_unique<ProfilingCommandHandler<CommandT>>(std::move(wrapped),
//     //                                                         os));
// }
//
// template <typename CommandT>
// std::unique_ptr<asp::RequestHandler<CommandT>>
// decorate_com_handler(std::unique_ptr<asp::RequestHandler<CommandT>> wrapped,
//                      ui::Mediator<ui::Invalidatable>&
//                      cacheInvalidationMediator)
// {
//     return std::make_unique<CacheAwareCommandHandler<CommandT>>(
//         std::move(wrapped), cacheInvalidationMediator);
// }

} // namespace sprint_timer::compose

#endif /* end of include guard: COMMANDHANDLERDECORATOR_H_GF3JAIOA */
