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
#ifndef CACHEAWARECOMMANDHANDLER_H_DVXZS5E1
#define CACHEAWARECOMMANDHANDLER_H_DVXZS5E1

#include "api/com_query/Command.h"
#include "api/com_query/CommandHandler.h"
#include "api/com_query/RequestHandlerDecorator.h"
#include "qt_gui/presentation/Invalidatable.h"
#include "qt_gui/presentation/Mediator.h"
#include <memory>

namespace sprint_timer::compose {

template <asp::Command CommandT>
class CacheAwareCommandHandler : public asp::RequestHandlerDecorator<CommandT> {
public:
    using WrappedType = asp::CommandHandler<CommandT>;

    CacheAwareCommandHandler(
        std::unique_ptr<WrappedType> wrapped_,
        ui::Mediator<ui::Invalidatable>& cacheInvalidationMediator_)
        : asp::RequestHandlerDecorator<CommandT>{std::move(wrapped_)}
        , cacheInvalidationMediator{cacheInvalidationMediator_}
    {
    }

    void handle(const CommandT& command) override
    {
        // Calling notify first to reset caches
        cacheInvalidationMediator.notifyAll(
            [](auto* colleague) { colleague->invalidate(); });
        // notify();
        // Then SyncronizingActionInvoker should launch update of all caches
        asp::RequestHandlerDecorator<CommandT>::handle(command);
    }

private:
    ui::Mediator<ui::Invalidatable>& cacheInvalidationMediator;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: CACHEAWARECOMMANDHANDLER_H_DVXZS5E1 */
