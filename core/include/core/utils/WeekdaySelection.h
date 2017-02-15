/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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

#include "date_wrapper/DateTime.h"
#include <array>

namespace {
constexpr std::array<unsigned, 7> weekdayBits = {1, 2, 4, 8, 16, 32, 64};
}

struct WeekdaySelection {

    WeekdaySelection(unsigned mask = 0)
        : mask{mask}
    {
    }

    unsigned selectionMask() const { return mask; }

    void selectDay(DateTime::Weekday weekday)
    {
        mask |= weekdayBits[static_cast<unsigned>(weekday)];
    }

    void unselectDay(DateTime::Weekday weekday)
    {
        mask ^= weekdayBits[static_cast<unsigned>(weekday)];
    }

    bool isSelected(DateTime::Weekday weekday) const
    {
        return (mask & weekdayBits[static_cast<unsigned>(weekday)]) != 0;
    }

private:
    unsigned mask{0};
};
