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
#include "core/use_cases/request_statistics/SprintStatisticsHandler.h"
#include <algorithm>
#include <ranges>

namespace sprint_timer::use_cases {

SprintStatisticsHandler::SprintStatisticsHandler(SprintStorageReader& reader_)
    : reader{reader_}
{
}

typename SprintStatisticsQuery::result_t
SprintStatisticsHandler::handle(SprintStatisticsQuery&& query)
{
    const auto sprints = reader.findByDateRange(query.dateRange);
    if (sprints.empty()) {
        return std::nullopt;
    }
    const TagTop tagTop{sprints, query.numTopTags};
    const auto& frequencies = tagTop.tagFrequencies();
    std::vector<TaggedSprintsDTO> taggedSprints(frequencies.size());
    std::ranges::transform(frequencies,
                           begin(taggedSprints),
                           [&, pos = 0](const auto& entry) mutable {
                               return TaggedSprintsDTO{
                                   entry.first.name(),
                                   entry.second,
                                   tagTop.sprintsForTagAt(pos++)};
                           });
    return SprintStatisticsDTO{query.numTopTags,
                               tagTop.sprintsForTagAt(std::nullopt),
                               std::move(taggedSprints)};
}

} // namespace sprint_timer::use_cases
