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
#include "qt_gui/presentation/DateRangeSelectorPresenter.h"
#include <numeric>

namespace sprint_timer::ui {

DateRangeSelectorPresenter::DateRangeSelectorPresenter(
    QueryHandler<use_cases::OperationalRangeQuery, dw::DateRange>& handler_,
    DateRangeChangeListener& dateRangeChangeListener_,
    dw::Weekday firstDayOfWeek_)
    : handler{handler_}
    , dateRangeChangeListener{dateRangeChangeListener_}
    , firstDayOfWeek{firstDayOfWeek_}
{
}

void DateRangeSelectorPresenter::onSelectedRangeChanged(
    const dw::DateRange& dateRange)
{
    dateRangeChangeListener.get().onRangeChanged(dateRange);
}

void DateRangeSelectorPresenter::updateViewImpl()
{
    if (auto v = view(); v) {
        const auto range =
            handler.get().handle(use_cases::OperationalRangeQuery{});
        const int startYear = static_cast<int>(range.start().year());
        const int lastYear = static_cast<int>(range.finish().year());
        std::vector<int> years(static_cast<size_t>(lastYear - startYear + 1),
                               0);
        std::iota(begin(years), end(years), startYear);
        v.value()->setFirstDayOfWeek(firstDayOfWeek);
        v.value()->updateOperationalRange(years);
    }
}

void DateRangeSelectorPresenter::onViewAttached() { updateViewImpl(); }

} // namespace sprint_timer::ui

