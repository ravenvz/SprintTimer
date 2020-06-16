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
#ifndef SPRINT_TIMER_OSTREAMSINK_H_H
#define SPRINT_TIMER_OSTREAMSINK_H_H

#include "external_io/Sink.h"
#include <iterator>
#include <ostream>

namespace sprint_timer::external_io {

class OstreamSink : public Sink {

public:
    explicit OstreamSink(std::ostream& out)
        : out{out}
    {
    }

    void send(std::vector<std::string>&& data) override
    {
        std::copy(cbegin(data),
                  cend(data),
                  std::ostream_iterator<
                      std::iterator_traits<decltype(cbegin(data))>::value_type>(
                      out, "\n"));
    }

private:
    std::ostream& out;
};

} // namespace sprint_timer::external_io

#endif // SPRINT_TIMER_OSTREAMSINK_H_H
