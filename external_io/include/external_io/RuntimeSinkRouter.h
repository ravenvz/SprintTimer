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
#ifndef RUNTIMESINKROUTER_H_XANBYPYS
#define RUNTIMESINKROUTER_H_XANBYPYS

#include "core/DataExporter.h"
#include "external_io/Sink.h"
#include "external_io/SinkRoutingException.h"
#include <functional>
#include <unordered_map>

namespace sprint_timer::external_io {

class RuntimeSinkRouter {
public:
    using SinkTypeMapper =
        std::unordered_map<SinkType, std::reference_wrapper<Sink>>;

    explicit RuntimeSinkRouter(SinkTypeMapper&& mapper);

    void route(std::vector<std::string>&& data, SinkType sinkType);

private:
    SinkTypeMapper sinkTypeMapper;
};

} // namespace sprint_timer::external_io

#endif /* end of include guard: RUNTIMESINKROUTER_H_XANBYPYS */
