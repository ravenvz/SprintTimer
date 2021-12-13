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
#include "core/use_cases/request_statistics/WorktimeStatisticsHandler.h"
#include "core/Distribution.h"

namespace {

// TODO that depends on DayPart enum member count
constexpr size_t numDayParts{6};

} // namespace

namespace sprint_timer::use_cases {

WorktimeStatisticsHandler::WorktimeStatisticsHandler(
    sprint_statistics_handler_t& sprintStatisticsHandler_)
    : sprintStatisticsHandler{sprintStatisticsHandler_}
{
}

WorktimeStatisticsHandler::result_t
WorktimeStatisticsHandler::handle(WorktimeStatisticsQuery&& query)
{
    const auto statistics = sprintStatisticsHandler.handle(
        SprintStatisticsQuery{query.numTopTags, query.dateRange});

    if (statistics.allSprints.empty()) {
        return std::nullopt;
    }

    const auto& sprints = query.nthTagFromTop
                              ? statistics.data[*query.nthTagFromTop].sprints
                              : statistics.allSprints;

    std::vector<double> sprintsPerDayPart(numDayParts, 0);
    auto updateCount = [&sprintsPerDayPart](const auto& timeSpan) {
        ++sprintsPerDayPart[static_cast<size_t>(dayPart(timeSpan))];
    };
    std::ranges::for_each(sprints, updateCount);
    Distribution<double> distribution{std::move(sprintsPerDayPart)};

    return WorktimeStatisticsDTO{
        sprints, static_cast<DayPart>(distribution.getMaxValueBin())};
}

} // namespace sprint_timer::use_cases
