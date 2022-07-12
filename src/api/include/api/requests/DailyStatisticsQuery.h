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
#ifndef DAILYSTATISTICSQUERY_H_KSY2WTKM
#define DAILYSTATISTICSQUERY_H_KSY2WTKM

#include "date_wrapper/date_wrapper.h"
#include <iostream>
#include <optional>
#include <vector>

namespace sprint_timer::api {

struct DailyStatisticsDTO {
    double averageSprintsPerWorkday{0.0};
    double expectedSprintsPerWorkday{0.0};
    int32_t totalSprints{0};
    std::vector<int32_t> distribution;
};

struct DailyStatisticsQuery {

    using Result = DailyStatisticsDTO;

    size_t numTopTags;
    std::optional<size_t> nthTagFromTop;
    dw::DateRange dateRange;

    friend bool operator==(const DailyStatisticsQuery&,
                           const DailyStatisticsQuery&) = default;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const DailyStatisticsQuery& query)
{
    os << "DailyStatisticsQuery{numTopTags: " << query.numTopTags
       << ", filterByTag: "
       << (query.nthTagFromTop ? std::to_string(*query.nthTagFromTop) : "none")
       << ", dateRange: " << query.dateRange << "}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: DAILYSTATISTICSQUERY_H_KSY2WTKM */
