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
#include "core/WorkdayTracker.h"

namespace {

template <typename In, typename Out, typename UnaryOp, typename Pred>
void transform_if(In first, In last, Out out, UnaryOp unary_op, Pred predicate);

template <typename Map>
typename Map::const_iterator greatest_less(Map const& m,
                                           typename Map::key_type const& k);

} // namespace

namespace sprint_timer {

// TODO word this properly
/* Add new week schedule that is active since sinceDate including.
 * When new week schedule is same as previous date schedule, insertion is
 * ignored. When sinceDate is same as previous date, previous date schedule is
 * ovewritten with new schedule. */
void WorkdayTracker::addWeekSchedule(const dw::Date& sinceDate,
                                     const WeekSchedule& schedule)
{
    // Ignore schedule insertion when it has same date or schedule for
    // previous date is the same
    if (const auto it = greatest_less(schedules, sinceDate);
        it != schedules.cend() && it->second == schedule) {
        return;
    }
    // Merge schedules when the schedule for next date is same as inserted
    // schedule
    if (auto it = schedules.lower_bound(sinceDate);
        it != schedules.end() && it->second == schedule) {
        schedules.erase(it->first);
    }
    schedules.insert_or_assign(sinceDate, schedule);
}

void WorkdayTracker::removeWeekSchedule(const dw::Date& date)
{
    if (auto it = schedules.cbegin(); it == schedules.cend())
        return;

    auto have_same_schedule = [this](auto before, auto after) {
        return before != schedules.cend() && after != schedules.cend()
            && before->second == after->second;
    };

    auto date_before_it = greatest_less(schedules, date);
    auto date_after_it = schedules.upper_bound(date);

    if (have_same_schedule(date_before_it, date_after_it))
        schedules.erase(date_after_it->first);

    schedules.erase(date);
}

bool WorkdayTracker::isWorkday(const dw::Date& date) const
{
    if (isExtraHoliday(date))
        return false;
    if (isExtraWorkday(date))
        return true;
    return scheduleFor(date).isWorkday(weekday(date));
}

int WorkdayTracker::goal(const dw::Date& date) const
{
    if (auto it = extraDays.find(date); it != extraDays.cend())
        return it->second;
    return scheduleFor(date).targetGoal(dw::weekday(date));
}

// bool WorkdayTracker::isWorkday(const dw::Weekday& weekday) const
// {
//     return workdays_.isSelected(weekday);
// }

void WorkdayTracker::addExtraWorkday(const dw::Date& date, int goal)
{
    addExceptionalDay(date, goal);
}

void WorkdayTracker::addExtraHoliday(const dw::Date& date)
{
    addExceptionalDay(date, 0);
}

void WorkdayTracker::addExceptionalDay(const dw::Date& date, int goal)
{
    extraDays.insert_or_assign(date, goal);
}

void WorkdayTracker::removeExceptionalDay(const dw::Date& date)
{
    if (auto it = extraDays.find(date); it == extraDays.cend())
        return;
    extraDays.erase(date);
}

WeekSchedule WorkdayTracker::scheduleFor(const dw::Date& date) const
{
    auto it = schedules.lower_bound(date);
    if (it == schedules.cbegin() && !schedules.empty())
        return (date == it->first) ? it->second : WeekSchedule{};
    if (!schedules.empty())
        return (date == it->first) ? it->second : (--it)->second;
    return WeekSchedule{};
}

WeekSchedule WorkdayTracker::currentSchedule() const
{
    return scheduleFor(dw::current_date_local());
}

WorkdayTracker::ScheduleRoaster WorkdayTracker::scheduleRoaster() const
{
    ScheduleRoaster output;
    output.reserve(schedules.size());
    std::copy(schedules.cbegin(), schedules.cend(), std::back_inserter(output));
    return output;
}

// utils::WeekdaySelection WorkdayTracker::workdays() const { return workdays_;
// }

std::vector<dw::Date> WorkdayTracker::extraWorkdays() const
{
    std::vector<dw::Date> result;
    transform_if(
        extraDays.cbegin(),
        extraDays.cend(),
        std::back_inserter(result),
        [](const auto& entry) { return entry.first; },
        [](const auto& entry) { return entry.second > 0; });
    return result;
}

std::vector<dw::Date> WorkdayTracker::extraHolidays() const
{
    std::vector<dw::Date> result;
    transform_if(
        extraDays.cbegin(),
        extraDays.cend(),
        std::back_inserter(result),
        [](const auto& entry) { return entry.first; },
        [](const auto& entry) { return entry.second == 0; });
    return result;
}

std::vector<WorkdayTracker::DayData> WorkdayTracker::exceptionalDays() const
{
    return std::vector<WorkdayTracker::DayData>(extraDays.cbegin(),
                                                extraDays.cend());
}

bool WorkdayTracker::isExtraWorkday(const dw::Date& date) const
{
    if (auto found = extraDays.find(date); found != extraDays.cend())
        return found->second > 0;
    return false;
}

bool WorkdayTracker::isExtraHoliday(const dw::Date& date) const
{
    if (auto found = extraDays.find(date); found != extraDays.cend())
        return found->second == 0;
    return false;
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

int goalFor(const WorkdayTracker& workdayTracker,
            const dw::DateRange& dateRange)
{
    int goal{0};

    for (auto day = dateRange.start(); day <= dateRange.finish();
         day = day + dw::Days{1}) {
        goal += workdayTracker.goal(day);
    }

    return goal;
}

bool operator==(const WorkdayTracker& lhs, const WorkdayTracker& rhs)
{
    return lhs.schedules == rhs.schedules && lhs.extraDays == rhs.extraDays;
}

} // namespace sprint_timer

namespace {

template <typename In, typename Out, typename UnaryOp, typename Pred>
void transform_if(In first, In last, Out out, UnaryOp unary_op, Pred predicate)
{
    while (first != last) {
        if (predicate(*first)) {
            *out = unary_op(*first);
            ++out;
        }
        ++first;
    }
}

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

