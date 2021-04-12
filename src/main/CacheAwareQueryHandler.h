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
#ifndef CACHEAWAREQUERYHANDLER_H_JGYKWEAB
#define CACHEAWAREQUERYHANDLER_H_JGYKWEAB

#include "core/QueryHandler.h"
#include <memory>
#include "qt_gui/presentation/Invalidatable.h"
#include "qt_gui/presentation/Mediator.h"

#include <iostream>

namespace sprint_timer::compose {

template <typename QueryT, typename ResultT>
class CacheAwareQueryHandler : public QueryHandler<QueryT, ResultT>,
                               public ui::Invalidatable {
public:
    using WrappedType = sprint_timer::QueryHandler<QueryT, ResultT>;
    using MediatorType =
        sprint_timer::ui::Mediator<sprint_timer::ui::Invalidatable>;

    CacheAwareQueryHandler(std::unique_ptr<WrappedType> wrapped_,
                           MediatorType& cacheInvalidationMediator_)
        : wrapped{std::move(wrapped_)}
        , cacheInvalidationMediator{cacheInvalidationMediator_}
    {
        cacheInvalidationMediator.addColleague(this);
    }

    ~CacheAwareQueryHandler() override
    {
        cacheInvalidationMediator.removeColleague(this);
    }

    void invalidate() override { cachedResult = std::nullopt; }

    ResultT handle(QueryT&& query) override
    {
        if (!cachedResult) {
            cachedQuery = query;
            cachedResult = wrapped->handle(std::move(query));
        }
        else {
            std::cout << "Cache hit" << std::endl;
        }
        return *cachedResult;
    }

private:
    std::unique_ptr<WrappedType> wrapped;
    MediatorType& cacheInvalidationMediator;
    std::optional<QueryT> cachedQuery;
    std::optional<ResultT> cachedResult;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: CACHEAWAREQUERYHANDLER_H_JGYKWEAB */
