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
#ifndef DATERANGESELECTORPRESENTER_H_9Y6GTFSZ
#define DATERANGESELECTORPRESENTER_H_9Y6GTFSZ

#include "qt_gui/presentation/DateRangeSelectorContract.h"
#include "qt_gui/presentation/StatisticsMediator.h"
#include <core/QueryHandler.h>
#include <core/use_cases/request_op_range/OperationalRangeQuery.h>

namespace sprint_timer::ui {

class DateRangeSelectorPresenter
    : public contracts::DateRangeSelectorContract::Presenter {
public:
    DateRangeSelectorPresenter(
        QueryHandler<use_cases::OperationalRangeQuery, dw::DateRange>& handler_,
        DateRangeChangeListener& dateRangeChangeListener_,
        dw::Weekday firstDayOfWeek);

    DateRangeSelectorPresenter(DateRangeSelectorPresenter&&) = default;

    DateRangeSelectorPresenter&
    operator=(DateRangeSelectorPresenter&&) = default;

    void onSelectedRangeChanged(const dw::DateRange& dateRange) override;

private:
    std::reference_wrapper<
        QueryHandler<use_cases::OperationalRangeQuery, dw::DateRange>>
        handler;
    std::reference_wrapper<DateRangeChangeListener> dateRangeChangeListener;
    dw::Weekday firstDayOfWeek;

    void updateViewImpl() override;

    void onViewAttached() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: DATERANGESELECTORPRESENTER_H_9Y6GTFSZ */
