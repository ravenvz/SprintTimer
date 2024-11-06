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
#include "core/RequestForMonthsBack.h"

namespace sprint_timer {

RequestForMonthsBack::RequestForMonthsBack(int numMonths_)
    : numMonths{numMonths_}
{
}

auto RequestForMonthsBack::dateRange(dw::Date periodEnd) const -> dw::DateRange
{
    using namespace dw;
    const auto months_back = periodEnd - Months{numMonths - 1};
    const auto to = last_day_of_month(periodEnd);
    const auto from = Date{months_back.year(), months_back.month(), Day{1}};
    return DateRange{from, to};
}

} // namespace sprint_timer

