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
#ifndef VERBOSEQUERYHANDLER_H_PLEQQBNM
#define VERBOSEQUERYHANDLER_H_PLEQQBNM

#include "core/QueryHandler.h"
#include <iostream>
#include <memory>

namespace sprint_timer {

template <typename QueryT>
class VerboseQueryHandler : public QueryHandler<QueryT> {
public:
    explicit VerboseQueryHandler(std::unique_ptr<QueryHandler<QueryT>> wraped);

    typename QueryT::result_t handle(QueryT&& query) override;

private:
    std::unique_ptr<QueryHandler<QueryT>> wrapped;
};

template <typename QueryT>
VerboseQueryHandler<QueryT>::VerboseQueryHandler(
    std::unique_ptr<QueryHandler<QueryT>> wrapped_)
    : wrapped{std::move(wrapped_)}
{
}

template <typename QueryT>
typename QueryT::result_t VerboseQueryHandler<QueryT>::handle(QueryT&& query)
{
    std::cout << "Handling query: " << query << '\n';
    return wrapped->handle(std::move(query));
}

} // namespace sprint_timer

#endif /* end of include guard: VERBOSEQUERYHANDLER_H_PLEQQBNM */
