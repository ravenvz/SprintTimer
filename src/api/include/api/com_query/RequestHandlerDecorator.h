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
#ifndef REQUESTHANDLERDECORATOR_H_OXUHHM83
#define REQUESTHANDLERDECORATOR_H_OXUHHM83

#include "api/com_query/RequestHandler.h"
#include <memory>

namespace asp {

template <Request RequestT>
class RequestHandlerDecorator : public RequestHandler<RequestT> {
public:
    using wrapped_t = RequestHandler<RequestT>;

    explicit RequestHandlerDecorator(std::unique_ptr<wrapped_t> wrapped_)
        : wrapped{std::move(wrapped_)}
    {
    }

    RequestT::Result handle(const RequestT& request) override
    {
        return wrapped->handle(request);
    }

private:
    std::unique_ptr<wrapped_t> wrapped;
};

} // namespace asp

#endif /* end of include guard: REQUESTHANDLERDECORATOR_H_OXUHHM83 */
