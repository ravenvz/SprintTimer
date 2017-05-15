/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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

constexpr int sizeInDays(const TimeSpan& timeSpan) noexcept
{
    return timeSpan.start().discreteDaysTo(timeSpan.finish()) + 1;
}

int discreteDaysBetween(const TimeSpan& lhs, const TimeSpan& rhs)
{
    return std::abs(lhs.start().discreteDaysTo(rhs.start()));
}

} // namespace

SprintStatItem::SprintStatItem(const std::vector<Sprint>& sprints,
                               const TimeSpan& timeInterval)
    : timeSpan(timeInterval)
    , mSprints(sprints)
{
}


Distribution<double> SprintStatItem::dailyDistribution() const
{
    return Distribution<double>{computeDailyDistribution()};
}


Distribution<double> SprintStatItem::weekdayDistribution() const
{
    return Distribution<double>{computeWeekdayDistribution(), countWeekdays()};
}


Distribution<double> SprintStatItem::worktimeDistribution() const
{
    return Distribution<double>{computeWorkTimeDistribution()};
}


std::vector<Sprint> SprintStatItem::sprints() const { return mSprints; }


std::vector<double> SprintStatItem::computeDailyDistribution() const
{
    if (mSprints.empty())
        return std::vector<double>(0, 0);
    int spanInCalendarDays = sizeInDays(timeSpan);
    std::vector<double> distribution(spanInCalendarDays, 0);
    for (const Sprint& sprint : mSprints) {
        distribution[discreteDaysBetween(timeSpan, sprint.timeSpan())]++;
    }
    return distribution;
}


std::vector<double> SprintStatItem::computeWeekdayDistribution() const
{
    std::vector<double> distribution(7, 0);
    if (mSprints.empty())
        return distribution;
    for (const Sprint& sprint : mSprints) {
        distribution[static_cast<size_t>(sprint.startTime().dayOfWeek())]++;
    }
    return distribution;
}


std::vector<double> SprintStatItem::computeWorkTimeDistribution() const
{
    std::vector<double> distribution(DayPart::numParts, 0);
    for (const Sprint& sprint : mSprints) {
        distribution[static_cast<size_t>(
            DayPart::dayPart(sprint.timeSpan()))]++;
    }
    return distribution;
}


std::vector<int> SprintStatItem::countWeekdays() const
{
    std::vector<int> result(7, 0);
    for (int dayNum = 0; dayNum < sizeInDays(timeSpan); ++dayNum) {
        auto date = timeSpan.start().add(DateTime::Days{dayNum});
        result[static_cast<unsigned>(date.dayOfWeek())]++;
    }
    return result;
}

namespace DayPart {


DayPart dayPart(const TimeSpan& timeSpan)
{
    auto hour = timeSpan.start().hour();

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

// TODO OCP violation
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

// TODO OCP violation
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
