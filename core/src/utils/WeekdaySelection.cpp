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

#include "core/utils/WeekdaySelection.h"

namespace {

constexpr std::array<unsigned, 7> weekdayBits = {1, 2, 4, 8, 16, 32, 64};

int countBits(unsigned mask)
{
    int res{0};
    while (mask) {
        res += mask & 1u;
        mask >>= 1u;
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

void WeekdaySelection::selectDay(dw::Weekday weekday)
{
    mask |= weekdayBits[static_cast<unsigned>(weekday)];
}

void WeekdaySelection::unselectDay(dw::Weekday weekday)
{
    mask ^= weekdayBits[static_cast<unsigned>(weekday)];
}

bool WeekdaySelection::isSelected(dw::Weekday weekday) const
{
    return (mask & weekdayBits[static_cast<unsigned>(weekday)]) != 0;
}

int WeekdaySelection::numSelected() const { return countBits(mask); }

unsigned numWorkdays(const dw::DateTimeRange& timeSpan,
                     const WeekdaySelection& workdays)
{
    unsigned res{0};

    for (auto day = timeSpan.start(); day <= timeSpan.finish();
         day = day + dw::Days{1}) {
        if (workdays.isSelected(static_cast<dw::Weekday>(day.weekday())))
            ++res;
    }

    return res;
}

std::vector<int> workday_outline(const dw::DateTimeRange& timeSpan,
                                 const WeekdaySelection& workdays)
{
    std::vector<int> outline;
    outline.reserve(static_cast<size_t>(timeSpan.duration<dw::Days>().count()));

    for (auto day = timeSpan.start(); day <= timeSpan.finish();
         day = day + dw::Days{1}) {
        if (workdays.isSelected(static_cast<dw::Weekday>(day.weekday())))
            outline.push_back(1);
        else
            outline.push_back(0);
    }

    return outline;
}

} // namespace sprint_timer::utils
