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
#ifndef WORKSCHEDULE_H_IUR90OGC
#define WORKSCHEDULE_H_IUR90OGC

#include "core/WeekSchedule.h"
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

namespace sprint_timer {

class WorkSchedule {
public:
    using ScheduleData = std::pair<dw::Date, WeekSchedule>;
    using Roaster = std::vector<ScheduleData>;
    using DateGoal = std::pair<dw::Date, int>;

    WorkSchedule() = default;

    void addWeekSchedule(const dw::Date& sinceDate,
                         const WeekSchedule& weekSchedule);

    void removeWeekSchedule(const dw::Date& date);

    bool isWorkday(const dw::Date& date) const;

    int goal(const dw::Date& date) const;

    void addExceptionalDay(const dw::Date& date, int goal);

    void removeExceptionalDay(const dw::Date& date);

    WeekSchedule weekScheduleFor(const dw::Date& date) const;

    WeekSchedule currentWeekSchedule() const;

    Roaster roaster() const;

    std::vector<DateGoal> exceptionalDays() const;

    friend bool operator==(const WorkSchedule& lhs, const WorkSchedule& rhs);

private:
    struct DateHash {
        size_t operator()(const dw::Date& date) const;
    };

    std::unordered_map<dw::Date, int, DateHash> extraDays;
    std::map<dw::Date, WeekSchedule> allWeekSchedules;

    bool isExtraWorkday(const dw::Date& date) const;

    bool isExtraHoliday(const dw::Date& date) const;
};

int numWorkdays(const WorkSchedule& workSchedule,
                const dw::DateRange& dateSpan);

int goalFor(const WorkSchedule& workSchedule, const dw::DateRange& dateRange);

// TODO this operator overload was added for debugging purposes; it is
// rather expensive to call - best hide it in tests or remove
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const WorkSchedule& workSchedule);

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const WorkSchedule& workSchedule)
{
    os << "WorkSchedule {\n";
    os << "Schedules: \n";
    for (const auto& entry : workSchedule.roaster()) {
        os << '\t' << entry.first << " " << entry.second << "\n";
    }
    os << "\n\tExceptionalDays: ";
    for (const auto& [day, goal] : workSchedule.exceptionalDays()) {
        os << "[" << dw::to_string(day, "dd.MM.yyyy") << ": " << goal << "]";
    }
    os << "\n";

    return os;
}

} // namespace sprint_timer

#endif /* end of include guard: WORKSCHEDULE_H_IUR90OGC */
