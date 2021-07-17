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
#ifndef QUERYHANDLERDECORATOR_H_PDP6G0WS
#define QUERYHANDLERDECORATOR_H_PDP6G0WS

#include "CacheAwareQueryHandler.h"
#include "VerboseQueryHandler.h"

#include "ProfilingQueryHandler.h"

namespace sprint_timer::compose {

template <typename QueryT>
std::unique_ptr<QueryHandler<QueryT>>
decorate(std::unique_ptr<QueryHandler<QueryT>> wrapped, std::ostream& os)
{
    return std::make_unique<ProfilingQueryHandler<QueryT>>(std::move(wrapped),
                                                           os);
    // return std::make_unique<sprint_timer::VerboseQueryHandler<QueryT>>(
    //     std::move(wrapped), os);
}

template <typename QueryT>
std::unique_ptr<QueryHandler<QueryT>>
decorate(std::unique_ptr<QueryHandler<QueryT>> wrapped,
         ui::Mediator<ui::Invalidatable>& cacheInvalidationMediator)
{
    return std::make_unique<CacheAwareQueryHandler<QueryT>>(
        std::move(wrapped), cacheInvalidationMediator);
}

} // namespace sprint_timer::compose

#endif /* end of include guard: QUERYHANDLERDECORATOR_H_PDP6G0WS */
