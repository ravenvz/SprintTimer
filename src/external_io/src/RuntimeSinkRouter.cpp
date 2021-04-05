/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "external_io/RuntimeSinkRouter.h"

namespace sprint_timer::external_io {

RuntimeSinkRouter::RuntimeSinkRouter(SinkTypeMapper&& mapper_)
    : sinkTypeMapper{std::move(mapper_)}
{
}

void RuntimeSinkRouter::route(std::vector<std::string>&& data,
                              SinkType sinkType)
{
    if (auto sink = sinkTypeMapper.find(sinkType);
        sink != sinkTypeMapper.cend()) {
        sink->second.get().send(std::move(data));
        return;
    }
    std::string errMsg{"Unable to find sink to route to with SinkType: "};
    errMsg += std::to_string(static_cast<int>(sinkType));
    throw SinkRoutingException{errMsg};
}

} // namespace sprint_timer::external_io

