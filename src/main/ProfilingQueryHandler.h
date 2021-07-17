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
#ifndef PROFILINGQUERYHANDLER_H_5ZAK6EDU
#define PROFILINGQUERYHANDLER_H_5ZAK6EDU

#include "core/QueryHandler.h"
#include <iostream>
#include <mutex>

namespace sprint_timer::compose {

template <typename QueryT>
class ProfilingQueryHandler : public QueryHandler<QueryT> {
public:
    ProfilingQueryHandler(std::unique_ptr<QueryHandler<QueryT>> wrapped_,
                          std::ostream& outputStream_)
        : wrapped{std::move(wrapped_)}
        , os{outputStream_}
    {
    }

    typename QueryT::result_t handle(QueryT&& query) override
    {
        const auto start = std::chrono::steady_clock::now();
        auto result = wrapped->handle(std::move(query));
        const auto end = std::chrono::steady_clock::now();
        {
            std::lock_guard lock{mtx};
            os << "Handling query: " << query << " ";
            os << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                        start)
                      .count()
               << "ms" << '\n';
        }
        return result;
    }

private:
    std::unique_ptr<QueryHandler<QueryT>> wrapped;
    std::ostream& os;
    std::mutex mtx;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: PROFILINGQUERYHANDLER_H_5ZAK6EDU */
