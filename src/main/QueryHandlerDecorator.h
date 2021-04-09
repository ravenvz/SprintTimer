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

namespace sprint_timer::compose {

template <typename QueryT, typename ResultT>
std::unique_ptr<QueryHandler<QueryT, ResultT>>
decorate(std::unique_ptr<QueryHandler<QueryT, ResultT>> wrapped)
{
    return std::make_unique<sprint_timer::VerboseQueryHandler<QueryT, ResultT>>(
        std::move(wrapped));
}

template <typename QueryT, typename ResultT>
std::unique_ptr<sprint_timer::QueryHandler<QueryT, ResultT>>
decorate(std::unique_ptr<sprint_timer::QueryHandler<QueryT, ResultT>> wrapped,
         ui::Mediator<ui::Invalidatable>& cacheInvalidationMediator)
{
    return std::make_unique<CacheAwareQueryHandler<QueryT, ResultT>>(
        std::move(wrapped), cacheInvalidationMediator);
}

} // namespace sprint_timer::compose

#endif /* end of include guard: QUERYHANDLERDECORATOR_H_PDP6G0WS */
