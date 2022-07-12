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

#include "api/com_query/RequestHandlerDecorator.h"
#include "api/com_query/Query.h"
#include "qt_gui/presentation/Invalidatable.h"
#include "qt_gui/presentation/Mediator.h"
#include <memory>
#include <mutex>
#include <optional>

namespace sprint_timer::compose {

template <asp::Query QueryT>
class CacheAwareQueryHandler : public asp::RequestHandlerDecorator<QueryT>,
                               public ui::Invalidatable {
public:
    using WrappedType = asp::QueryHandler<QueryT>;
    using MediatorType =
        sprint_timer::ui::Mediator<sprint_timer::ui::Invalidatable>;

    CacheAwareQueryHandler(std::unique_ptr<WrappedType> wrapped_,
                           MediatorType& cacheInvalidationMediator_)
        : asp::RequestHandlerDecorator<QueryT>{std::move(wrapped_)}
        , cacheInvalidationMediator{cacheInvalidationMediator_}
    {
        cacheInvalidationMediator.addColleague(this);
    }

    ~CacheAwareQueryHandler() override
    {
        cacheInvalidationMediator.removeColleague(this);
    }

    // Note, call to this method is not thread-safe; should be only called from
    // main thread
    void invalidate() override { cachedResult = std::nullopt; }

    QueryT::Result handle(const QueryT& query) override
    {
        {
            std::lock_guard lock{mtx};
            if (!cachedResult || cachedQuery != query) {
                cachedQuery = query;
                cachedResult = asp::RequestHandlerDecorator<QueryT>::handle(std::move(query));
            }
            // else {
            //     std::cout << "Cache hit\n";
            // }
        }
        return *cachedResult;
    }

private:
    MediatorType& cacheInvalidationMediator;
    std::optional<QueryT> cachedQuery;
    std::optional<typename QueryT::Result> cachedResult;
    std::mutex mtx;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: CACHEAWAREQUERYHANDLER_H_JGYKWEAB */
