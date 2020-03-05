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
#include <core/Observable.h>
#include <qt_gui/presentation/BestWorkdayPresenter.h>

namespace sprint_timer::compose {

class BestWorkdayPresenterProxy
    : public ui::BasePresenter<ui::contracts::BestWorkday::View>,
      public ui::StatisticsColleague {
public:
    BestWorkdayPresenterProxy(ui::StatisticsMediator& mediator_,
                              IConfig& settings_,
                              Observable& configChangedSignaller_)
        : mediator{mediator_}
        , settings{settings_}
        , configChangedWatcher{configChangedSignaller_,
                               [this]() { onConfigChanged(); }}
    {
    }

    void attachView(ui::contracts::BestWorkday::View& view_) override
    {
        presenter.attachView(view_);
        BasePresenter::attachView(view_);
    }

    void detachView(ui::contracts::BestWorkday::View& view_) override
    {
        presenter.detachView(view_);
        BasePresenter::detachView(view_);
    }

    void onSharedDataChanged() override { }

private:
    ui::StatisticsMediator& mediator;
    IConfig& settings;
    dw::Weekday cached{settings.firstDayOfWeek()};
    ui::BestWorkdayPresenter presenter{mediator, cached};
    CompositionObserver configChangedWatcher;

    void updateViewImpl() override { }

    void onConfigChanged()
    {
        if (view && settings.firstDayOfWeek() != cached) {
            cached = settings.firstDayOfWeek();
            presenter = ui::BestWorkdayPresenter{mediator, cached};
            presenter.attachView(*view);
        }
    }
};

} // namespace sprint_timer::compose
