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

#include "api/com_query/QueryHandler.h"
#include <functional>
#include <future>

namespace com_query {

template <query QueryT> class AsyncQueryHandler {
public:
    using result_t = QueryT::Result;
    using callback_t = std::function<void(result_t)>;

    virtual ~AsyncQueryHandler() = default;

    std::future<result_t> handle(const QueryT& query)
    {
        return handleAwaitImpl(query);
    }

    void handle(const QueryT& query, callback_t callback)
    {
        handleCallbackImpl(query, std::forward<callback_t>(callback));
    }

    QueryT::Result handleSync(const QueryT& query)
    {
        return handleSyncImpl(query);
    }

private:
    virtual std::future<result_t> handleAwaitImpl(const QueryT& query) = 0;

    virtual QueryT::Result handleSyncImpl(const QueryT& query) = 0;

    virtual void handleCallbackImpl(const QueryT& query,
                                    callback_t callback) = 0;
};

} // namespace com_query

#endif /* end of include guard: ASYNCQUERYHANDLER_H_NUMQHJYM */
