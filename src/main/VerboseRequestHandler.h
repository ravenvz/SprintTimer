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
#ifndef VERBOSECOMMANDHANDLER_H_VSD9NQOL
#define VERBOSECOMMANDHANDLER_H_VSD9NQOL

#include "api/com_query/RequestHandlerDecorator.h"
#include <iostream>
#include <memory>
#include <mutex>

namespace sprint_timer {

template <asp::Request RequestT>
class VerboseRequestHandler : public asp::RequestHandlerDecorator<RequestT> {
public:
    VerboseRequestHandler(
        std::unique_ptr<asp::RequestHandler<RequestT>> wrapped,
        std::ostream& outputStream_);

    RequestT::Result handle(const RequestT& request) override;

private:
    std::ostream& os;
    std::mutex mtx;
};

template <asp::Request RequestT>
VerboseRequestHandler<RequestT>::VerboseRequestHandler(
    std::unique_ptr<asp::RequestHandler<RequestT>> wrapped_,
    std::ostream& outputStream_)
    : asp::RequestHandlerDecorator<RequestT>{std::move(wrapped_)}
    , os{outputStream_}
{
}

template <asp::Request RequestT>
RequestT::Result
VerboseRequestHandler<RequestT>::handle(const RequestT& request)
{
    {
        std::lock_guard lock{mtx};
        os << "Handling request: " << request << '\n';
    }
    return asp::RequestHandlerDecorator<RequestT>::handle(request);
}

} // namespace sprint_timer

#endif /* end of include guard: VERBOSECOMMANDHANDLER_H_VSD9NQOL */
