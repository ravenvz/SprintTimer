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
#include "ProfilingCommandHandler.h"
#include "VerboseCommandHandler.h"

namespace sprint_timer::compose {

template <typename CommandT>
std::unique_ptr<CommandHandler<CommandT>>
decorate(std::unique_ptr<CommandHandler<CommandT>> wrapped, std::ostream& os)
{
    return std::make_unique<sprint_timer::VerboseCommandHandler<CommandT>>(
        std::move(wrapped), os);
    // return std::make_unique<ProfilingCommandHandler<CommandT>>(
    //     std::move(wrapped), os);
}

template <typename CommandT>
std::unique_ptr<sprint_timer::CommandHandler<CommandT>>
decorate(std::unique_ptr<sprint_timer::CommandHandler<CommandT>> wrapped,
         ui::Mediator<ui::Invalidatable>& cacheInvalidationMediator)
{
    return std::make_unique<CacheAwareCommandHandler<CommandT>>(
        std::move(wrapped), cacheInvalidationMediator);
}

} // namespace sprint_timer::compose

#endif /* end of include guard: COMMANDHANDLERDECORATOR_H_GF3JAIOA */
