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
#include "qt_gui/RequestForDaysBack.h"


namespace {

dw::DateRange nDaysBackTillNow(int numDays);

} // namespace


namespace sprint_timer::ui::qt_gui {

RequestForDaysBack::RequestForDaysBack(int numDays_)
    : numDays{numDays_}
{
}

dw::DateRange RequestForDaysBack::dateRange() const
{
    return nDaysBackTillNow(numDays);
}

} // namespace sprint_timer::ui::qt_gui


namespace {

dw::DateRange nDaysBackTillNow(int numDays)
{
    using namespace dw;
    auto now = current_date_local();
    auto from = now - Days{numDays - 1};
    return {from, now};
}

} // namespace
