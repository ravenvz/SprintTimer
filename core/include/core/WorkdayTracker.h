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

#include "core/utils/WeekdaySelection.h"
#include <iostream>
#include <unordered_set>

namespace sprint_timer {


class WorkdayTracker {
public:
    WorkdayTracker() = default;

    explicit WorkdayTracker(utils::WeekdaySelection workdays);

    bool isWorkday(const dw::Date& date) const;

    bool isWorkday(const dw::Weekday& weekday) const;

    void addExtraWorkday(const dw::Date& date);

    void addExtraHoliday(const dw::Date& date);

    std::vector<dw::Date> extraWorkdays() const;

    std::vector<dw::Date> extraHolidays() const;

    friend bool operator==(const WorkdayTracker& lhs,
                           const WorkdayTracker& rhs);

private:
    struct DateHash {
        size_t operator()(const dw::Date& date) const;
    };

    utils::WeekdaySelection workdays_{0};
    std::unordered_set<dw::Date, DateHash> extraHolidays_;
    std::unordered_set<dw::Date, DateHash> extraWorkdays_;

    bool isExtraWorkday(const dw::Date& date) const;

    bool isExtraHoliday(const dw::Date& date) const;
};


int numWorkdays(const WorkdayTracker& workdayTracker,
                const dw::DateRange& dateSpan);

// TODO this operator overload was added for debugging purposes; it is rather
// expensive to call - best hide it in tests or remove
template <class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const WorkdayTracker& tracker)
{
    os << "WorkdayTracker {\n";
    os << "\tworking days: ";
    if (tracker.isWorkday(dw::Weekday::Monday))
        os << "Monday ";
    if (tracker.isWorkday(dw::Weekday::Tuesday))
        os << "Tuesday ";
    if (tracker.isWorkday(dw::Weekday::Wednesday))
        os << "Wednesday ";
    if (tracker.isWorkday(dw::Weekday::Thursday))
        os << "Thursday ";
    if (tracker.isWorkday(dw::Weekday::Friday))
        os << "Friday ";
    if (tracker.isWorkday(dw::Weekday::Saturday))
        os << "Saturday ";
    if (tracker.isWorkday(dw::Weekday::Sunday))
        os << "Sunday ";
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
