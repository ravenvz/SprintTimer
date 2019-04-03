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
#include "core/WorkdayTracker.h"

namespace {

template <typename T, typename H>
std::vector<T> fromSet(const std::unordered_set<T, H>& set);

} // namespace

namespace sprint_timer {

WorkdayTracker::WorkdayTracker(utils::WeekdaySelection workdays)
    : workdays_{workdays}
{
}

bool WorkdayTracker::isWorkday(const dw::Date& date) const
{
    if (isExtraHoliday(date))
        return false;
    if (isExtraWorkday(date))
        return true;
    return isWorkday(weekday(date));
}

bool WorkdayTracker::isWorkday(const dw::Weekday& weekday) const
{
    return workdays_.isSelected(weekday);
}

void WorkdayTracker::addExtraWorkday(const dw::Date& date)
{
    extraWorkdays_.insert(date);
}

void WorkdayTracker::addExtraHoliday(const dw::Date& date)
{
    extraHolidays_.insert(date);
}

std::vector<dw::Date> WorkdayTracker::extraWorkdays() const
{
    return fromSet(extraWorkdays_);
}

std::vector<dw::Date> WorkdayTracker::extraHolidays() const
{
    return fromSet(extraHolidays_);
}

bool WorkdayTracker::isExtraWorkday(const dw::Date& date) const
{
    return extraWorkdays_.find(date) != extraWorkdays_.cend();
}

bool WorkdayTracker::isExtraHoliday(const dw::Date& date) const
{
    return extraHolidays_.find(date) != extraHolidays_.cend();
}

size_t WorkdayTracker::DateHash::operator()(const dw::Date& date) const
{
    return std::hash<int>()(dw::sys_days(date).time_since_epoch().count());
}

int numWorkdays(const WorkdayTracker& workdayTracker,
                const dw::DateRange& dateRange)
{
    int counter{0};

    for (auto day = dateRange.start(); day <= dateRange.finish();
         day = day + dw::Days{1}) {
        if (workdayTracker.isWorkday(day))
            ++counter;
    }

    return counter;
}

bool operator==(const WorkdayTracker& lhs, const WorkdayTracker& rhs)
{
    return lhs.workdays_.selectionMask() == rhs.workdays_.selectionMask()
        && lhs.extraHolidays_ == rhs.extraHolidays_
        && lhs.extraWorkdays_ == rhs.extraWorkdays_;
}

} // namespace sprint_timer

namespace {

template <typename T, typename H>
std::vector<T> fromSet(const std::unordered_set<T, H>& set)
{
    std::vector<T> result;
    result.reserve(set.size());
    std::copy(cbegin(set), cend(set), std::back_inserter(result));
    return result;
}

} // namespace

