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
#ifndef WORKDAYTRACKER_H_DPJU1VL0
#define WORKDAYTRACKER_H_DPJU1VL0

#include "core/WeekSchedule.h"
#include "core/utils/WeekdaySelection.h"
#include <iostream>
#include <map>
#include <unordered_map>

namespace sprint_timer {

class WorkdayTracker {
public:
    using ScheduleData = std::pair<dw::Date, WeekSchedule>;
    using ScheduleRoaster = std::vector<ScheduleData>;
    using DayData = std::pair<dw::Date, int>;

    WorkdayTracker() = default;

    void addWeekSchedule(const dw::Date& sinceDate,
                         const WeekSchedule& schedule);

    bool isWorkday(const dw::Date& date) const;

    int goal(const dw::Date& date) const;

    void addExceptionalDay(const dw::Date& date, int goal);

    void addExtraWorkday(const dw::Date& date, int goal);

    void addExtraHoliday(const dw::Date& date);

    WeekSchedule scheduleFor(const dw::Date& date) const;

    WeekSchedule currentSchedule() const;

    ScheduleRoaster scheduleRoaster() const;

    std::vector<dw::Date> extraWorkdays() const;

    std::vector<dw::Date> extraHolidays() const;

    std::vector<DayData> exceptionalDays() const;

    friend bool operator==(const WorkdayTracker& lhs,
                           const WorkdayTracker& rhs);

private:
    struct DateHash {
        size_t operator()(const dw::Date& date) const;
    };

    std::unordered_map<dw::Date, int, DateHash> extraDays;
    std::map<dw::Date, WeekSchedule> schedules;

    bool isExtraWorkday(const dw::Date& date) const;

    bool isExtraHoliday(const dw::Date& date) const;

    // TODO this operator overload was added for debugging purposes; it is
    // rather expensive to call - best hide it in tests or remove
    template <class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>&
    operator<<(std::basic_ostream<CharT, Traits>& os,
               const WorkdayTracker& tracker);
};


int numWorkdays(const WorkdayTracker& workdayTracker,
                const dw::DateRange& dateSpan);

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const WorkdayTracker& tracker)
{
    os << "WorkdayTracker {\n";
    os << "Schedules: \n";
    for (const auto& entry : tracker.schedules) {
        os << '\t' << entry.first << " " << entry.second << "\n";
    }
    os << "\n\tExtra holidays: ";
    for (const auto& day : tracker.extraHolidays())
        os << dw::to_string(day, "dd.MM.yyyy") << " ";
    os << "\n";
    os << "\n\tExtra workdays: ";
    for (const auto& day : tracker.extraWorkdays())
        os << dw::to_string(day, "dd.MM.yyyy") << " ";
    os << "\n}";

    return os;
}

} // namespace sprint_timer

#endif /* end of include guard: WORKDAYTRACKER_H_DPJU1VL0 */
