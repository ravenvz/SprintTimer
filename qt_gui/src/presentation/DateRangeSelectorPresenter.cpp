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
#include "qt_gui/presentation/DateRangeSelectorPresenter.h"

namespace sprint_timer::ui {

DateRangeSelectorPresenter::DateRangeSelectorPresenter(
    QueryHandler<use_cases::OperationalRangeQuery, dw::DateRange>& handler_,
    StatisticsMediator& mediator_,
    dw::Weekday firstDayOfWeek_)
    : handler{handler_}
    , mediator{mediator_}
    , firstDayOfWeek{firstDayOfWeek_}
{
    mediator.get().addColleague(this);
}

DateRangeSelectorPresenter::~DateRangeSelectorPresenter()
{
    mediator.get().removeColleague(this);
}

void DateRangeSelectorPresenter::onSelectedRangeChanged(
    const dw::DateRange& dateRange)
{
    mediator.get().changeRange(this, dateRange);
}

void DateRangeSelectorPresenter::updateViewImpl()
{
    const auto range = handler.get().handle(use_cases::OperationalRangeQuery{});
    std::vector<int> years;
    const int startYear = static_cast<int>(range.start().year());
    const int lastYear = static_cast<int>(range.finish().year());
    for (int current = startYear; current <= lastYear; ++current) {
        years.push_back(current);
    }
    view->setFirstDayOfWeek(firstDayOfWeek);
    view->updateOperationalRange(years);
}

void DateRangeSelectorPresenter::onSharedDataChanged() { }

} // namespace sprint_timer::ui

