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
#ifndef VERBOSEASYNCQUERYHANDLER_H_NL0LQB9U
#define VERBOSEASYNCQUERYHANDLER_H_NL0LQB9U

#include "core/AsyncQueryHandler.h"
#include <iostream>

namespace sprint_timer::compose {

template <typename QueryT>
class VerboseAsyncQueryHandler : public AsyncQueryHandler<QueryT> {
public:
    VerboseAsyncQueryHandler(
        std::unique_ptr<AsyncQueryHandler<QueryT>> wrapped_)
        : wrapped{std::move(wrapped_)}
    {
    }

private:
    std::unique_ptr<AsyncQueryHandler<QueryT>> wrapped;

    std::future<typename QueryT::result_t>
    handleAwaitImpl(QueryT&& query) override
    {
        std::cout << "Handling query async: " << query << '\n';
        return wrapped->handle(std::move(query));
    }

    void handleCallbackImpl(
        QueryT&& query,
        typename AsyncQueryHandler<QueryT>::callback_t callback) override
    {
        std::cout << "Handling query with callback: " << query << std::endl;
        wrapped->handle(std::move(query), callback);
    }

    typename QueryT::result_t handleSyncImpl(QueryT&& query) override
    {
        std::cout << "Handling query synchroniously: " << query << std::endl;
        return wrapped->handleSync(std::move(query));
    }
};

} // namespace sprint_timer::compose

#endif /* end of include guard: VERBOSEASYNCQUERYHANDLER_H_NL0LQB9U */
