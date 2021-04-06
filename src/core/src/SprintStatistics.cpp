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
#include "core/SprintStatistics.h"
#include <numeric>

namespace {

constexpr size_t daysInWeek{7};

constexpr size_t sizeInDays(const dw::DateRange& dateRange) noexcept
{
    return static_cast<size_t>(dateRange.duration().count()) + 1;
}

constexpr size_t daysBetween(const dw::Date& date,
                             const dw::DateTime& dateTime) noexcept
{
    const auto range = dw::DateRange{
        date, dw::Date{dateTime.year(), dateTime.month(), dateTime.day()}};
    return static_cast<size_t>(range.duration().count());
}

// TODO remove when all calls redirected to handler
std::vector<int> weekdayFrequency(const dw::DateRange& dateRange)
{
    std::vector<int> result(daysInWeek, 0);
    for (size_t dayNum = 0; dayNum < sizeInDays(dateRange); ++dayNum) {
        const auto date = dateRange.start() + dw::Days{dayNum};
        const auto weekdayNumber = static_cast<unsigned>(dw::weekday(date));
        ++result[weekdayNumber];
    }
    return result;
}

// TODO remove when all calls redirected to handler
constexpr bool containsDate(const dw::DateRange& dateRange,
                            const dw::Date& date) noexcept
{
    return dateRange.start() <= date && date <= dateRange.finish();
}

} // namespace

namespace sprint_timer {

using dw::DateTime;
using dw::DateTimeRange;
using entities::Sprint;

// TODO remove when all calls redirected to handler
Distribution<double> weekdayStatistics(const std::vector<Sprint>& sprints,
                                       const dw::DateRange& dateRange)
{
    std::vector<double> sprintsPerWeekday(daysInWeek, 0);

    for (const Sprint& sprint : sprints) {
        if (!containsDate(dateRange, sprint.startTime().date()))
            continue;
        ++sprintsPerWeekday[static_cast<size_t>(sprint.startTime().weekday())];
    }

    return Distribution<double>{std::move(sprintsPerWeekday),
                                weekdayFrequency(dateRange)};
}

Distribution<double>
workingHoursStatistics(const std::vector<entities::Sprint>& sprints)
{
    std::vector<double> sprintsPerDayPart(DayPart::numParts, 0);

    for (const Sprint& sprint : sprints) {
        ++sprintsPerDayPart[static_cast<size_t>(
            DayPart::dayPart(sprint.timeSpan()))];
    }

    return Distribution<double>{std::move(sprintsPerDayPart)};
}

Distribution<double> dailyStatistics(const std::vector<Sprint>& sprints,
                                     const dw::DateRange& dateRange)
{
    std::vector<double> sprintsPerDay(
        static_cast<size_t>(dateRange.duration().count() + 1), 0);

    for (const Sprint& sprint : sprints) {
        if (!containsDate(dateRange, sprint.startTime().date()))
            continue;
        const auto dayNumber =
            daysBetween(dateRange.start(), sprint.timeSpan().start());
        ++sprintsPerDay[static_cast<size_t>(dayNumber)];
    }

    return Distribution<double>{std::move(sprintsPerDay)};
}

namespace DayPart {

DayPart dayPart(const DateTimeRange& timeSpan)
{
    const auto hour = timeSpan.start().hour().count();

    if (22 < hour || hour <= 2) {
        return DayPart::Midnight;
    }
    else if (2 < hour && hour <= 6) {
        return DayPart::Night;
    }
    else if (6 < hour && hour <= 10) {
        return DayPart::Morning;
    }
    else if (10 < hour && hour <= 14) {
        return DayPart::Noon;
    }
    else if (14 < hour && hour <= 18) {
        return DayPart::Afternoon;
    }
    else {
        return DayPart::Evening;
    }
}

std::string dayPartName(unsigned dayPart)
{
    return dayPartName(static_cast<DayPart>(dayPart));
}

std::string dayPartName(DayPart dayPart)
{
    switch (dayPart) {
    case DayPart::Midnight:
        return "Midnight";
    case DayPart::Night:
        return "Night";
    case DayPart::Morning:
        return "Morning";
    case DayPart::Noon:
        return "Noon";
    case DayPart::Afternoon:
        return "Afternoon";
    case DayPart::Evening:
        return "Evening";
    }
    return "Invalid";
}

std::string dayPartHours(DayPart dayPart)
{
    switch (dayPart) {
    case DayPart::Midnight:
        return "22:00 - 2:00";
    case DayPart::Night:
        return "2:00 - 6:00";
    case DayPart::Morning:
        return "6:00 - 10:00";
    case DayPart::Noon:
        return "10:00 - 14:00";
    case DayPart::Afternoon:
        return "14:00 - 18:00";
    case DayPart::Evening:
        return "18:00 - 22:00";
    }
    return "Invalid";
}

std::string dayPartHours(unsigned dayPart)
{
    return dayPartHours(static_cast<DayPart>(dayPart));
}

} // namespace DayPart

} // namespace sprint_timer
