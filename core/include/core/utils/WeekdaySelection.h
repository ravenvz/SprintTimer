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

#ifndef WEEKDAYSELECTION_H_7HAYMKBJ
#define WEEKDAYSELECTION_H_7HAYMKBJ

#include <array>
#include <date_wrapper/date_wrapper.h>
#include <date_wrapper/date_wrapper.h>
#include <vector>

namespace sprint_timer::utils {

struct WeekdaySelection {

    explicit WeekdaySelection(unsigned mask = 0);

    unsigned selectionMask() const;

    void selectDay(dw::Weekday weekday);

    void unselectDay(dw::Weekday weekday);

    bool isSelected(dw::Weekday weekday) const;

    int numSelected() const;

private:
    unsigned mask{0};
};


/* Return number of workdays in given time range with
 * respect to current selection. */
unsigned numWorkdays(const dw::DateTimeRange& timeSpan,
                     const WeekdaySelection& workdays);

/* Return vector of integers. Elements with value 1 represent workday, elements
 * with value 0 represent non-working day. */
std::vector<int> workday_outline(const dw::DateTimeRange& timeSpan,
                                 const WeekdaySelection& workdays);
} // namespace sprint_timer::utils

#endif /* end of include guard: WEEKDAYSELECTION_H_7HAYMKBJ */
