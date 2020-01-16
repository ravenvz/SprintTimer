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
#include "qt_gui/RequestForWeeksBack.h"

namespace {

dw::DateRange nWeeksBackTillNow(int numWeeks, dw::Weekday firstDayOfWeek);

} // namespace

namespace sprint_timer::ui::qt_gui {

RequestForWeeksBack::RequestForWeeksBack(int numWeeks_,
                                         dw::Weekday firstDayOfWeek_)
    : numWeeks{numWeeks_}
    , firstDayOfWeek{std::move(firstDayOfWeek_)}
{
}

dw::DateRange RequestForWeeksBack::dateRange() const
{
    return nWeeksBackTillNow(numWeeks, firstDayOfWeek);
}

} // namespace sprint_timer::ui::qt_gui

namespace {

dw::DateRange nWeeksBackTillNow(int numWeeks, dw::Weekday firstDayOfWeek)
{
    using namespace dw;
    auto now = current_date_local();
    const auto from = prev_weekday(now - Weeks{numWeeks - 1},
                                   static_cast<dw::Weekday>(firstDayOfWeek));
    const auto lastDayOfWeek{firstDayOfWeek == dw::Weekday::Monday
                                 ? dw::Weekday::Sunday
                                 : dw::Weekday::Saturday};
    const auto to = next_weekday(now, lastDayOfWeek);
    return {from, to};
}

} // namespace
