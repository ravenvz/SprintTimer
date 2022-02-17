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
#include "core/use_cases/request_statistics/TopTagFrequenciesHandler.h"

namespace sprint_timer::use_cases {

TopTagFrequenciesHandler::TopTagFrequenciesHandler(
    sprint_statistics_handler_t& sprintStatisticsHandler_)
    : sprintStatisticsHandler{sprintStatisticsHandler_}
{
}

TopTagFrequenciesHandler::result_t
TopTagFrequenciesHandler::handle(TopTagFrequenciesQuery&& query)
{
    const auto statistics = sprintStatisticsHandler.handle(
        SprintStatisticsQuery{query.numTopTags, query.dateRange});

    if (!statistics) {
        return std::nullopt;
    }

    std::vector<TagFrequencyDTO> result;
    result.reserve(statistics->data.size());

    std::ranges::transform(
        statistics->data, std::back_inserter(result), [](const auto& entry) {
            return TagFrequencyDTO{entry.frequency, entry.tag};
        });

    return result;
}

} // namespace sprint_timer::use_cases
