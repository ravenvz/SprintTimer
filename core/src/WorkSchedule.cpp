/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "core/WorkSchedule.h"

namespace {

template <typename Map>
typename Map::const_iterator greatest_less(Map const& m,
                                           typename Map::key_type const& k);

} // namespace

namespace sprint_timer {

/* Add new week schedule.
 *
 * Schedule is 'active' from sinceDate including to the date of the next
 * schedule (excluding) that is later in time if such schedule exists (otherwise
 * it is 'active' to the current date).
 *
 * When new week schedule is the same as previous date schedule (meaning there
 * is no difference in daily goals), insertion is ignored.
 *
 * When allWeekSchedules already contain entry for sinceDate, this entry is
 * ovewritten with new schedule. */
void WorkSchedule::addWeekSchedule(const dw::Date& sinceDate,
                                   const WeekSchedule& schedule)
{
    // Ignore schedule insertion when it has same date or schedule for
    // previous date is the same
    if (const auto it = greatest_less(allWeekSchedules, sinceDate);
        it != allWeekSchedules.cend() && it->second == schedule) {
        return;
    }
    // Merge schedules when the week schedule for next date is same as
    // inserted one
    if (auto it = allWeekSchedules.lower_bound(sinceDate);
        it != allWeekSchedules.end() && it->second == schedule) {
        allWeekSchedules.erase(it->first);
    }
    allWeekSchedules.insert_or_assign(sinceDate, schedule);
}

void WorkSchedule::removeWeekSchedule(const dw::Date& date)
{
    if (auto it = allWeekSchedules.cbegin(); it == allWeekSchedules.cend())
        return;

    auto have_same_schedule = [this](auto before, auto after) {
        return before != allWeekSchedules.cend() &&
               after != allWeekSchedules.cend() &&
               before->second == after->second;
    };

    auto date_before_it = greatest_less(allWeekSchedules, date);
    auto date_after_it = allWeekSchedules.upper_bound(date);

    if (have_same_schedule(date_before_it, date_after_it))
        allWeekSchedules.erase(date_after_it->first);

    allWeekSchedules.erase(date);
}

bool WorkSchedule::isWorkday(const dw::Date& date) const
{
    if (isExtraHoliday(date))
        return false;
    if (isExtraWorkday(date))
        return true;
    return weekScheduleFor(date).isWorkday(weekday(date));
}

int WorkSchedule::goal(const dw::Date& date) const
{
    if (auto it = extraDays.find(date); it != extraDays.cend())
        return it->second;
    return weekScheduleFor(date).targetGoal(dw::weekday(date));
}

void WorkSchedule::addExceptionalDay(const dw::Date& date, int goal)
{
    extraDays.insert_or_assign(date, goal);
}

void WorkSchedule::removeExceptionalDay(const dw::Date& date)
{
    if (auto it = extraDays.find(date); it == extraDays.cend())
        return;
    extraDays.erase(date);
}

WeekSchedule WorkSchedule::weekScheduleFor(const dw::Date& date) const
{
    // there exists schedule that is active since specified date
    if (auto it = allWeekSchedules.lower_bound(date);
        it != allWeekSchedules.cend() && it->first == date)
        return it->second;
    // there exists schedule for date before specified date
    if (auto it = greatest_less(allWeekSchedules, date);
        it != allWeekSchedules.cend())
        return it->second;
    // there is no schedule for date before specified date
    return WeekSchedule{};
}

WeekSchedule WorkSchedule::currentWeekSchedule() const
{
    return weekScheduleFor(dw::current_date_local());
}

WorkSchedule::Roaster WorkSchedule::roaster() const
{
    Roaster output;
    output.reserve(allWeekSchedules.size());
    std::copy(allWeekSchedules.cbegin(),
              allWeekSchedules.cend(),
              std::back_inserter(output));
    return output;
}

std::vector<WorkSchedule::DateGoal> WorkSchedule::exceptionalDays() const
{
    return std::vector<WorkSchedule::DateGoal>(extraDays.cbegin(),
                                               extraDays.cend());
}

bool WorkSchedule::isExtraWorkday(const dw::Date& date) const
{
    if (auto found = extraDays.find(date); found != extraDays.cend())
        return found->second > 0;
    return false;
}

bool WorkSchedule::isExtraHoliday(const dw::Date& date) const
{
    if (auto found = extraDays.find(date); found != extraDays.cend())
        return found->second == 0;
    return false;
}

size_t WorkSchedule::DateHash::operator()(const dw::Date& date) const
{
    return std::hash<int>()(dw::sys_days(date).time_since_epoch().count());
}

int numWorkdays(const WorkSchedule& workSchedule,
                const dw::DateRange& dateRange)
{
    int counter{0};

    for (auto day = dateRange.start(); day <= dateRange.finish();
         day = day + dw::Days{1}) {
        if (workSchedule.isWorkday(day))
            ++counter;
    }

    return counter;
}

int goalFor(const WorkSchedule& workSchedule, const dw::DateRange& dateRange)
{
    int goal{0};

    for (auto day = dateRange.start(); day <= dateRange.finish();
         day = day + dw::Days{1}) {
        goal += workSchedule.goal(day);
    }

    return goal;
}

bool operator==(const WorkSchedule& lhs, const WorkSchedule& rhs)
{
    return lhs.allWeekSchedules == rhs.allWeekSchedules &&
           lhs.extraDays == rhs.extraDays;
}

} // namespace sprint_timer

namespace {

template <typename Map>
typename Map::const_iterator greatest_less(Map const& m,
                                           typename Map::key_type const& k)
{
    typename Map::const_iterator it = m.lower_bound(k);
    if (it != m.begin()) {
        return --it;
    }
    return m.end();
}

} // namespace

