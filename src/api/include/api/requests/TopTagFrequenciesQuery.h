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
#ifndef TOPTAGFREQUENCIESQUERY_H_MF8TUTOP
#define TOPTAGFREQUENCIESQUERY_H_MF8TUTOP

#include "date_wrapper/date_wrapper.h"
#include <optional>
#include <vector>

namespace sprint_timer::api {

struct TagFrequencyDTO {
    double frequency;
    std::string tag;
};

struct TopTagFrequenciesQuery {
    using Result = std::optional<std::vector<TagFrequencyDTO>>;

    size_t numTopTags;
    dw::DateRange dateRange;

    friend bool operator==(const TopTagFrequenciesQuery&,
                           const TopTagFrequenciesQuery&) = default;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const TopTagFrequenciesQuery& query)
{
    os << "TopTagFrequenciesQuery{numTopTags: " << query.numTopTags
       << ", dateRange: " << query.dateRange << "}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: TOPTAGFREQUENCIESQUERY_H_MF8TUTOP */
