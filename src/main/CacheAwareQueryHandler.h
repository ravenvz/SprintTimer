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
#include "qt_gui/presentation/Invalidatable.h"
#include "qt_gui/presentation/Mediator.h"
#include <memory>
#include <mutex>
#include <optional>

#include <iostream>

namespace sprint_timer::compose {

template <typename QueryT>
class CacheAwareQueryHandler : public QueryHandler<QueryT>,
                               public ui::Invalidatable {
public:
    using WrappedType = sprint_timer::QueryHandler<QueryT>;
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

    // Note, call to this method is not thread-safe; should be only called from
    // main thread
    void invalidate() override { cachedResult = std::nullopt; }

    typename QueryT::result_t handle(QueryT&& query) override
    {
        {
            std::lock_guard lock{mtx};
            if (!cachedResult) {
                cachedQuery = query;
                cachedResult = wrapped->handle(std::move(query));
            }
            else {
                std::cout << "Cache hit" << std::endl;
            }
        }
        return *cachedResult;
    }

private:
    std::unique_ptr<WrappedType> wrapped;
    MediatorType& cacheInvalidationMediator;
    std::optional<QueryT> cachedQuery;
    std::optional<typename QueryT::result_t> cachedResult;
    std::mutex mtx;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: CACHEAWAREQUERYHANDLER_H_JGYKWEAB */
