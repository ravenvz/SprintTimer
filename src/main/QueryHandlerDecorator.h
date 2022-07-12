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
#include "ErrorReportingQueryHandler.h"
#include "ProfilingRequestHandler.h"
#include "VerboseRequestHandler.h"

namespace sprint_timer::compose {

template <asp::Query QueryT>
std::unique_ptr<asp::RequestHandler<QueryT>>
decorate_query(std::unique_ptr<asp::RequestHandler<QueryT>> wrapped,
               std::ostream& os,
               ui::Mediator<ui::Invalidatable>& cacheInvalidationMediator)
{
    auto cacheAwareHandler = std::make_unique<CacheAwareQueryHandler<QueryT>>(
        std::move(wrapped), cacheInvalidationMediator);
    auto profilingHandler = std::make_unique<ProfilingRequestHandler<QueryT>>(
        std::move(cacheAwareHandler), os);
    auto errorReportingHandler =
        std::make_unique<ErrorReportingQueryHandler<QueryT>>(
            std::move(cacheAwareHandler));
    return errorReportingHandler;
}

// template <asp::Query QueryT>
// std::unique_ptr<asp::RequestHandler<QueryT>>
// decorate_query(std::unique_ptr<asp::RequestHandler<QueryT>> wrapped,
// std::ostream& os)
//{
//     return std::make_unique<asp::RequestHandler<QueryT>>(
//         std::make_unique<ProfilingRequestHandler<QueryT>>(std::move(wrapped),
//         os), os);
//     // return std::make_unique<sprint_timer::VerboseQueryHandler<QueryT>>(
//     //     std::move(wrapped), os);
// }
//
// template <asp::Query QueryT>
// std::unique_ptr<asp::RequestHandler<QueryT>>
// decorate_query(std::unique_ptr<asp::RequestHandler<QueryT>> wrapped,
//          ui::Mediator<ui::Invalidatable>& cacheInvalidationMediator)
//{
//     return std::make_unique<CacheAwareQueryHandler<QueryT>>(
//         std::move(wrapped), cacheInvalidationMediator);
// }

} // namespace sprint_timer::compose

#endif /* end of include guard: QUERYHANDLERDECORATOR_H_PDP6G0WS */
