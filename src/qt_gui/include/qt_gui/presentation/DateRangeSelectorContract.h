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
#ifndef DATERANGESELECTORCONTRACT_H_QMKBCECA
#define DATERANGESELECTORCONTRACT_H_QMKBCECA

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"
#include <date_wrapper/date_wrapper.h>
#include <vector>

namespace sprint_timer::ui::contracts::DateRangeSelectorContract {

class Presenter;

class View : public mvp::BaseView<View, Presenter> {
public:
    virtual void updateOperationalRange(const std::vector<int>& years) = 0;

    virtual void setFirstDayOfWeek(dw::Weekday weekday) = 0;
};

class Presenter : public mvp::BasePresenter<View> {
public:
    virtual void onSelectedRangeChanged(const dw::DateRange& dateRange) = 0;
};

} // namespace sprint_timer::ui::contracts::DateRangeSelectorContract

#endif /* end of include guard: DATERANGESELECTORCONTRACT_H_QMKBCECA */
