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
#ifndef THREADPOOLQUERYHANDLER_H_RXOVZ1EY
#define THREADPOOLQUERYHANDLER_H_RXOVZ1EY

#include "core/AsyncQueryHandler.h"
#include "riften/thiefpool.hpp"

namespace sprint_timer {

template <typename QueryT>
class ThreadPoolQueryHandler : public AsyncQueryHandler<QueryT> {
public:
    ThreadPoolQueryHandler(riften::Thiefpool& threadPool_,
                           QueryHandler<QueryT>& syncHandler_)
        : threadPool{threadPool_}
        , syncHandler{syncHandler_}
    {
    }

    virtual ~ThreadPoolQueryHandler() = default;

    ThreadPoolQueryHandler(const ThreadPoolQueryHandler&) = delete;
    ThreadPoolQueryHandler& operator=(const ThreadPoolQueryHandler&) = delete;

private:
    riften::Thiefpool& threadPool;
    QueryHandler<QueryT>& syncHandler;

    std::future<typename QueryT::result_t>
    handleAwaitImpl(QueryT&& query) override
    {
        return threadPool.enqueue([this, q = std::move(query)]() mutable {
            return handleSyncImpl(std::move(q));
        });
    }

    void handleCallbackImpl(
        QueryT&& query,
        std::function<void(typename QueryT::result_t)> callback) override
    {
        threadPool.enqueue_detach(
            [&](auto clb, auto q) { clb(handleSyncImpl(std::move(q))); },
            callback,
            std::move(query));
    }

    typename QueryT::result_t handleSyncImpl(QueryT&& query) override
    {
        return syncHandler.handle(std::move(query));
    }
};

} // namespace sprint_timer

#endif /* end of include guard: THREADPOOLQUERYHANDLER_H_RXOVZ1EY */
