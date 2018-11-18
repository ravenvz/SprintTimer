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

#include "core/utils/WeekdaySelection.h"

namespace {

constexpr std::array<unsigned, 7> weekdayBits = {1, 2, 4, 8, 16, 32, 64};

int countBits(unsigned mask)
{
    int res{0};
    while (mask) {
        res += mask & 1;
        mask >>= 1;
    }
    return res;
}

} // namespace

namespace sprint_timer::utils {

WeekdaySelection::WeekdaySelection(unsigned mask)
    : mask{mask}
{
}

unsigned WeekdaySelection::selectionMask() const { return mask; }

void WeekdaySelection::selectDay(dw::DateTime::Weekday weekday)
{
    mask |= weekdayBits[static_cast<unsigned>(weekday)];
}

void WeekdaySelection::unselectDay(dw::DateTime::Weekday weekday)
{
    mask ^= weekdayBits[static_cast<unsigned>(weekday)];
}

bool WeekdaySelection::isSelected(dw::DateTime::Weekday weekday) const
{
    return (mask & weekdayBits[static_cast<unsigned>(weekday)]) != 0;
}

int WeekdaySelection::numSelected() const { return countBits(mask); }

unsigned numWorkdays(const dw::TimeSpan& timeSpan,
                     const WeekdaySelection& workdays)
{
    unsigned res{0};

    for (auto day = timeSpan.start(); day <= timeSpan.finish();
         day = day.add(dw::DateTime::Days{1})) {
        if (workdays.isSelected(
                static_cast<dw::DateTime::Weekday>(day.dayOfWeek())))
            ++res;
    }

    return res;
}
} // namespace sprint_timer::utils
