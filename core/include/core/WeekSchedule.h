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
#ifndef WEEKSCHEDULE_H_NTSVHHA0
#define WEEKSCHEDULE_H_NTSVHHA0

#include <array>
#include <date_wrapper/date_wrapper.h>

namespace sprint_timer {

class WeekSchedule {
public:
    int targetGoal(const dw::Weekday& weekday) const;

    void setTargetGoal(const dw::Weekday& weekday, int goal);

    bool isWorkday(const dw::Weekday& weekday) const;

private:
    std::array<int, 7> goals_{0, 0, 0, 0, 0, 0, 0};
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const WeekSchedule& schedule)
{
    os << "WeekSchedule {\n\t\t";
    os << "Mon: " << schedule.targetGoal(dw::Weekday::Monday) << ' ';
    os << "Tue: " << schedule.targetGoal(dw::Weekday::Tuesday) << ' ';
    os << "Wed: " << schedule.targetGoal(dw::Weekday::Wednesday) << ' ';
    os << "Thu: " << schedule.targetGoal(dw::Weekday::Thursday) << ' ';
    os << "Fri: " << schedule.targetGoal(dw::Weekday::Friday) << ' ';
    os << "Sat: " << schedule.targetGoal(dw::Weekday::Saturday) << ' ';
    os << "Sun: " << schedule.targetGoal(dw::Weekday::Sunday);
    os << "\n\t}";
    return os;
}

bool operator==(const WeekSchedule& lhs, const WeekSchedule& rhs);

bool operator!=(const WeekSchedule& lhs, const WeekSchedule& rhs);

} // namespace sprint_timer

#endif /* end of include guard: WEEKSCHEDULE_H_NTSVHHA0 */
