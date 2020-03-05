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
#include <core/CompositionObserver.h>
#include <core/IConfig.h>
#include <qt_gui/presentation/DateRangeSelectorPresenter.h>

namespace sprint_timer::compose {

class DateRangeSelectorPresenterProxy
    : public ui::contracts::DateRangeSelectorContract::Presenter,
      public ui::StatisticsColleague {
public:
    DateRangeSelectorPresenterProxy(
        QueryHandler<use_cases::OperationalRangeQuery, dw::DateRange>& handler_,
        ui::StatisticsMediator& mediator_,
        IConfig& settings_,
        Observable& configChangedSignaller_)
        : handler{handler_}
        , mediator{mediator_}
        , settings{settings_}
        , observer{configChangedSignaller_, [this]() { onConfigChanged(); }}
    {
    }

    void
    attachView(ui::contracts::DateRangeSelectorContract::View& view_) override
    {
        presenter.attachView(view_);
        BasePresenter::attachView(view_);
    }

    void
    detachView(ui::contracts::DateRangeSelectorContract::View& view_) override
    {
        presenter.detachView(view_);
        BasePresenter::detachView(view_);
    }

    void onSelectedRangeChanged(const dw::DateRange& dateRange) override
    {
        presenter.onSelectedRangeChanged(dateRange);
    }

    void onSharedDataChanged() override { }

private:
    QueryHandler<use_cases::OperationalRangeQuery, dw::DateRange>& handler;
    ui::StatisticsMediator& mediator;
    IConfig& settings;
    dw::Weekday cached{settings.firstDayOfWeek()};
    ui::DateRangeSelectorPresenter presenter{handler, mediator, cached};
    CompositionObserver observer;

    void updateViewImpl() override { }

    void onConfigChanged()
    {
        if (dw::Weekday actual = settings.firstDayOfWeek(); actual != cached) {
            cached = actual;
            presenter =
                ui::DateRangeSelectorPresenter{handler, mediator, cached};
            presenter.attachView(*view);
        }
    }
};

} // namespace sprint_timer::compose
