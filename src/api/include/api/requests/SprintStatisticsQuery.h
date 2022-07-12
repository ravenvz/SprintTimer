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
#ifndef SPRINTSTATISTICSQUERY_H
#define SPRINTSTATISTICSQUERY_H

#include "api/dtos/SprintStatisticsDTO.h"
#include "core/TagTop.h"
#include <iostream>

namespace sprint_timer::api {

struct SprintStatisticsQuery {
    using Result = std::optional<SprintStatisticsDTO>;

    size_t numTopTags;
    dw::DateRange dateRange;

    friend bool operator==(const SprintStatisticsQuery&,
                           const SprintStatisticsQuery&) = default;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const SprintStatisticsQuery& query)
{
    os << "SprintStatisticsQuery {" << query.dateRange << ", "
       << query.numTopTags << "}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: SPRINTSTATISTICSQUERY_H */
