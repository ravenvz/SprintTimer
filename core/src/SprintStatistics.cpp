/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

namespace sprint_timer {

using dw::DateTime;
using dw::DateTimeRange;
using entities::Sprint;

namespace {

    constexpr size_t daysInWeek{7};

    constexpr size_t sizeInDays(const DateTimeRange& timeSpan) noexcept
    {
        return static_cast<size_t>(timeSpan.duration<dw::Days>().count()) + 1;
    }

    constexpr size_t discreteDaysBetween(const DateTimeRange& lhs,
                                         const DateTimeRange& rhs) noexcept
    {
        return static_cast<size_t>(DateTimeRange{lhs.start(), rhs.start()}
                                       .duration<dw::Days>()
                                       .count());
    }

    std::vector<int> weekdayFrequency(const DateTimeRange& timeSpan)
    {
        std::vector<int> result(daysInWeek, 0);
        for (size_t dayNum = 0; dayNum < sizeInDays(timeSpan); ++dayNum) {
            const auto date = timeSpan.start() + dw::Days{dayNum};
            const auto weekdayNumber = static_cast<unsigned>(date.weekday());
            ++result[weekdayNumber];
        }
        return result;
    }

    constexpr bool sameDate(const DateTime& lhs, const DateTime& rhs) noexcept
    {
        return lhs.year() == rhs.year() && lhs.month() == rhs.month()
            && lhs.day() == rhs.day();
    }

    constexpr bool containsDate(const DateTimeRange& timeSpan,
                                const DateTime& dateTime) noexcept
    {
        return sameDate(timeSpan.start(), dateTime)
            || sameDate(timeSpan.finish(), dateTime)
            || (timeSpan.start() <= dateTime && dateTime <= timeSpan.finish());
    }

} // namespace

SprintStatItem::SprintStatItem(const std::vector<Sprint>& sprints,
                               const DateTimeRange& timeSpan)
{
    std::vector<double> sprintsPerDay(sizeInDays(timeSpan), 0);
    std::vector<double> sprintsPerWeekday(daysInWeek, 0);
    std::vector<double> sprintsPerDayPart(DayPart::numParts, 0);

    for (const Sprint& sprint : sprints) {
        if (!containsDate(timeSpan, sprint.timeSpan().start()))
            continue;
        const auto dayNumber = discreteDaysBetween(timeSpan, sprint.timeSpan());
        ++sprintsPerDay[static_cast<size_t>(dayNumber)];
        ++sprintsPerWeekday[static_cast<size_t>(sprint.startTime().weekday())];
        ++sprintsPerDayPart[static_cast<size_t>(
            DayPart::dayPart(sprint.timeSpan()))];
    }

    dailyDistribution_ = Distribution<double>{std::move(sprintsPerDay)};
    weekdayDistribution_ = Distribution<double>{std::move(sprintsPerWeekday),
                                                weekdayFrequency(timeSpan)};
    worktimeDistribution_ = Distribution<double>{std::move(sprintsPerDayPart)};
}


const Distribution<double>& SprintStatItem::dailyDistribution() const
{
    return dailyDistribution_;
}


const Distribution<double>& SprintStatItem::weekdayDistribution() const
{
    return weekdayDistribution_;
}


const Distribution<double>& SprintStatItem::worktimeDistribution() const
{
    return worktimeDistribution_;
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
