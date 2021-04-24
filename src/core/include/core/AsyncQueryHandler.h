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
#ifndef ASYNCQUERYHANDLER_H_NUMQHJYM
#define ASYNCQUERYHANDLER_H_NUMQHJYM

#include "core/QueryHandler.h"
#include <functional>
#include <future>

#include <iostream>

namespace sprint_timer {

template <typename QueryT> class AsyncQueryHandler {
public:
    using Result = typename QueryT::result_t;
    using Callback = std::function<void(Result)>;

    virtual ~AsyncQueryHandler() = default;

    std::future<Result> handle(QueryT&& query)
    {
        return handleAwaitImpl(std::move(query));
    }

    void handle(QueryT&& query, Callback callback)
    {
        handleCallbackImpl(std::move(query), std::forward<Callback>(callback));
    }

    Result handleSync(QueryT&& query)
    {
        return handleSyncImp(std::move(query));
    }

private:
    virtual std::future<Result> handleAwaitImpl(QueryT&& query) = 0;

    virtual Result handleSyncImp(QueryT&& query) = 0;

    virtual void handleCallbackImpl(QueryT&& query, Callback callback) = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: ASYNCQUERYHANDLER_H_NUMQHJYM */
