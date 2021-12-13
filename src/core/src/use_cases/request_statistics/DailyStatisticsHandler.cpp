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
#include "core/use_cases/request_statistics/DailyStatisticsHandler.h"
#include "core/utils/Algutils.h"

namespace {

size_t daysBetween(const dw::Date& date, const dw::DateTime& dateTime);

} // namespace

namespace sprint_timer::use_cases {

DailyStatisticsHandler::DailyStatisticsHandler(
    work_schedule_handler_t& workScheduleHandler_,
    request_statistics_handler_t& sprintStatisticsHandler_)
    : workScheduleHandler{workScheduleHandler_}
    , sprintStatisticsHandler{sprintStatisticsHandler_}
{
}

DailyStatisticsHandler::result_t
DailyStatisticsHandler::handle(DailyStatisticsQuery&& query)
{
    const auto statistics = sprintStatisticsHandler.handle(
        SprintStatisticsQuery{query.numTopTags, query.dateRange});

    if (statistics.allSprints.empty()) {
        return std::nullopt;
    }

    const auto schedule = workScheduleHandler.handle(WorkScheduleQuery{});
    const auto& sprints = query.nthTagFromTop
                              ? statistics.data[*query.nthTagFromTop].sprints
                              : statistics.allSprints;

    std::vector<int32_t> sprintsPerDay(
        static_cast<size_t>(query.dateRange.duration().count() + 1), 0);
    auto updateCount = [&sprintsPerDay,
                        start = query.dateRange.start()](const auto& interval) {
        const auto dayNumber = daysBetween(start, interval.start());
        ++sprintsPerDay[dayNumber];
    };
    std::ranges::for_each(sprints, updateCount);

    const auto total = ranges_ext::fold(sprintsPerDay, 0, std::plus<int>{});
    const auto workdays = numWorkdays(schedule, query.dateRange);
    const double actualAverage = workdays > 0 ? total / workdays : 0;
    const double goal{static_cast<double>(goalFor(schedule, query.dateRange))};
    const double expectedAverage = workdays > 0 ? goal / workdays : 0;

    return DailyStatisticsDTO{
        actualAverage, expectedAverage, total, sprintsPerDay};
}

} // namespace sprint_timer::use_cases

namespace {

size_t daysBetween(const dw::Date& date, const dw::DateTime& dateTime)
{
    const auto range = dw::DateRange{
        date, dw::Date{dateTime.year(), dateTime.month(), dateTime.day()}};
    return static_cast<size_t>(range.duration().count());
}

} // namespace
