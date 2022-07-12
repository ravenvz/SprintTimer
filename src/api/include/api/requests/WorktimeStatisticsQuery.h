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
#ifndef WORKTIMESTATISTICSQUERY_H_MSOKL3J2
#define WORKTIMESTATISTICSQUERY_H_MSOKL3J2

#include "api/dtos/DayPart.h"
#include "date_wrapper/date_wrapper.h"
#include <optional>
#include <vector>

namespace sprint_timer::api {

struct WorktimeStatisticsDTO {
    std::vector<dw::DateTimeRange> timeRanges;
    DayPart bestWorktime;
};

struct WorktimeStatisticsQuery {
    using Result = std::optional<WorktimeStatisticsDTO>;

    size_t numTopTags;
    std::optional<size_t> nthTagFromTop;
    dw::DateRange dateRange;

    friend bool operator==(const WorktimeStatisticsQuery&,
                           const WorktimeStatisticsQuery&) = default;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const WorktimeStatisticsQuery& query)
{
    os << "WorkdayStatisticsQuery{numTopTags: " << query.numTopTags
       << ", filterByTag: "
       << (query.nthTagFromTop ? std::to_string(*query.nthTagFromTop) : "none")
       << ", dateRange: " << query.dateRange << "}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: WORKTIMESTATISTICSQUERY_H_MSOKL3J2 */
