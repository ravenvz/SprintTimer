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
#ifndef SINK_H_LHIQQTBD
#define SINK_H_LHIQQTBD

#include <string>
#include <vector>

namespace sprint_timer::external_io {

class Sink {
public:
    virtual void send(std::vector<std::string>&& data) = 0;

    Sink() = default;
    Sink(const Sink& other) = delete;
    Sink& operator=(const Sink& other) = delete;
    Sink(Sink&& other) noexcept = delete;
    Sink& operator=(Sink&& other) noexcept = delete;
    virtual ~Sink() = default;
};

} // namespace sprint_timer::external_io

#endif /* end of include guard: SINK_H_LHIQQTBD */

