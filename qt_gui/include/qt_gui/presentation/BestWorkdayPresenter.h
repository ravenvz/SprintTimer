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
#ifndef BESTWORKDAYPRESENTER_H_OIE4UYUT
#define BESTWORKDAYPRESENTER_H_OIE4UYUT

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/presentation/BestWorkdayContract.h"
#include "qt_gui/presentation/StatisticsMediator.h"

namespace sprint_timer::ui {

class BestWorkdayPresenter : public contracts::BestWorkday::Presenter,
                             public StatisticsColleague {
public:
    BestWorkdayPresenter(StatisticsMediator& mediator,
                         dw::Weekday firstDayOfWeek);

    ~BestWorkdayPresenter() override;

    void onSharedDataChanged() override;

    BestWorkdayPresenter(BestWorkdayPresenter&& other) = default;

    BestWorkdayPresenter& operator=(BestWorkdayPresenter&& other) = default;

private:
    std::reference_wrapper<StatisticsMediator> mediator;
    dw::Weekday firstDayOfWeek;

    void updateViewImpl() override;

    void onViewAttached() override;

    void updateLegend(const Distribution<double>& distribution) const;

    void updateBars(const Distribution<double>& distribution) const;

    void updateWithDefaultValues() const;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: BESTWORKDAYPRESENTER_H_OIE4UYUT */
