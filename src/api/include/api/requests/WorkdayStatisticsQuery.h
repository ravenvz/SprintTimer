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
#ifndef WORKDAYSTATISTICSQUERY_H_R0GZUYZF
#define WORKDAYSTATISTICSQUERY_H_R0GZUYZF

#include "date_wrapper/date_wrapper.h"
#include <array>
#include <cstddef>
#include <optional>

namespace sprint_timer::api {

struct WorkdayStatisticsDTO {
    std::array<double, 7> sprintDistribution{};
    dw::Weekday bestWorkday{dw::Weekday::Monday};
    int percentageAboveAverage{0};
};

struct WorkdayStatisticsQuery {
    using Result = std::optional<WorkdayStatisticsDTO>;

    size_t numTopTags;
    std::optional<size_t> nthTagFromTop;
    dw::DateRange dateRange;

    friend bool operator==(const WorkdayStatisticsQuery&,
                           const WorkdayStatisticsQuery&) = default;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const WorkdayStatisticsQuery& query)
{
    os << "WorkdayStatisticsQuery{numTopTags: " << query.numTopTags
       << ", filterByTag: "
       << (query.nthTagFromTop ? std::to_string(*query.nthTagFromTop) : "none")
       << ", dateRange: " << query.dateRange << "}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: WORKDAYSTATISTICSQUERY_H_R0GZUYZF */
