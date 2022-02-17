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
#include "core/use_cases/request_statistics/WorkdayStatisticsHandler.h"
#include "core/Distribution.h"

namespace {

constexpr size_t daysInWeek{7};

std::optional<sprint_timer::use_cases::WorkdayStatisticsDTO>
computeStatistics(const std::vector<dw::DateTimeRange>& sprintIntervals,
                  const dw::DateRange& dateRange);

sprint_timer::Distribution<double>
weekdayStatistics(const std::vector<dw::DateTimeRange>& sprintIntervals,
                  const dw::DateRange& dateRange);

std::vector<int> weekdayFrequency(const dw::DateRange& dateRange);

std::array<double, daysInWeek>
sprintsByWeekday(const sprint_timer::Distribution<double>& distribution);

std::pair<dw::Weekday, int>
findBestWorkday(const sprint_timer::Distribution<double>& distribution);

} // namespace

namespace sprint_timer::use_cases {

WorkdayStatisticsHandler::WorkdayStatisticsHandler(
    sprint_statistics_handler_t& handler_)
    : handler{handler_}
{
}

WorkdayStatisticsHandler::result_t
WorkdayStatisticsHandler::handle(WorkdayStatisticsQuery&& query)
{
    const auto statistics = handler.handle(
        SprintStatisticsQuery{query.numTopTags, query.dateRange});
    if (!statistics) {
        return std::nullopt;
    }
    if (auto tag = query.nthTagFromTop; tag and *tag >= query.numTopTags) {
        return std::nullopt;
    }
    const auto& sprints = query.nthTagFromTop
                              ? statistics->data[*query.nthTagFromTop].sprints
                              : statistics->allSprints;
    return computeStatistics(sprints, query.dateRange);
}

} // namespace sprint_timer::use_cases

namespace {

std::optional<sprint_timer::use_cases::WorkdayStatisticsDTO>
computeStatistics(const std::vector<dw::DateTimeRange>& sprintIntervals,
                  const dw::DateRange& dateRange)
{
    using sprint_timer::use_cases::WorkdayStatisticsDTO;
    if (sprintIntervals.empty()) {
        return std::nullopt;
    }
    const auto distribution = weekdayStatistics(sprintIntervals, dateRange);
    const auto [bestWorkday, percentageAboveAverage] =
        findBestWorkday(distribution);
    return WorkdayStatisticsDTO{
        sprintsByWeekday(distribution), bestWorkday, percentageAboveAverage};
}

sprint_timer::Distribution<double>
weekdayStatistics(const std::vector<dw::DateTimeRange>& sprintIntervals,
                  const dw::DateRange& dateRange)
{
    std::vector<double> sprintsPerWeekday(daysInWeek, 0);

    for (const auto& interval : sprintIntervals) {
        ++sprintsPerWeekday[static_cast<size_t>(interval.start().weekday())];
    }

    return sprint_timer::Distribution<double>{std::move(sprintsPerWeekday),
                                              weekdayFrequency(dateRange)};
}

std::pair<dw::Weekday, int>
findBestWorkday(const sprint_timer::Distribution<double>& distribution)
{
    // Average should not be zero as distribution is not empty
    // and we had already checked that. But in case of refactorings
    // should ensure that either distribution is checked for emptiness
    // or average is checked for zero.
    const double average = distribution.getAverage();
    const int relativeComparisonInPercent =
        static_cast<int>((distribution.getMax() - average) * 100 / average);
    const dw::Weekday bestWorkday{
        static_cast<dw::Weekday>(distribution.getMaxValueBin())};
    return {bestWorkday, relativeComparisonInPercent};
}

std::array<double, daysInWeek>
sprintsByWeekday(const sprint_timer::Distribution<double>& distribution)
{
    std::array<double, daysInWeek> result{};
    std::ranges::copy(distribution.getDistributionVector(), begin(result));
    return result;
}

std::vector<int> weekdayFrequency(const dw::DateRange& dateRange)
{
    std::vector<int> result(daysInWeek, 0);
    for (auto date = dateRange.start(); date <= dateRange.finish();
         date = date + dw::Days{1}) {
        const auto weekdayNumber = static_cast<unsigned>(dw::weekday(date));
        ++result[weekdayNumber];
    }
    return result;
}

} // namespace
