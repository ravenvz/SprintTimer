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
#ifndef PROFILINGCOMMANDHANDLER_H_F5XGRBQE
#define PROFILINGCOMMANDHANDLER_H_F5XGRBQE

#include "api/com_query/RequestHandlerDecorator.h"
#include <iostream>
#include <mutex>

namespace sprint_timer::compose {

template <asp::Request RequestT>
class ProfilingRequestHandler : public asp::RequestHandlerDecorator<RequestT> {
public:
    ProfilingRequestHandler(
        std::unique_ptr<asp::RequestHandler<RequestT>> wrapped_,
        std::ostream& outputStream_)
        : asp::RequestHandlerDecorator<RequestT>{std::move(wrapped_)}
        , os{outputStream_}
    {
    }

    RequestT::Result handle(const RequestT& request) override
    {
        const auto start = std::chrono::steady_clock::now();
        auto res = asp::RequestHandlerDecorator<RequestT>::handle(request);

        const auto end = std::chrono::steady_clock::now();

        {
            std::lock_guard lock{mtx};
            os << "Handled query: " << request << " in ";
            os << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                        start)
                      .count();
            os << "ms\n";
        }

        return res;
    }

private:
    std::ostream& os;
    std::mutex mtx;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: PROFILINGCOMMANDHANDLER_H_F5XGRBQE */
