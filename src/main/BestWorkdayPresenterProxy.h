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
#include "api/IConfig.h"
#include "core/CompositionObserver.h"
#include "core/Observable.h"
#include "qt_gui/presentation/BestWorkdayPresenter.h"

namespace sprint_timer::compose {

class BestWorkdayPresenterProxy : public ui::contracts::BestWorkday::Presenter,
                                  public ui::StatisticsColleague {
public:
    BestWorkdayPresenterProxy(asp::QueryHandler<api::WorkdayStatisticsQuery>&
                                  workdayStatisticsHandler_,
                              ui::StatisticsMediator& mediator_,
                              const ui::StatisticsContext& statisticsContext_,
                              api::IConfig& settings_,
                              Observable& configChangedSignaller_)
        : workdayStatisticsHandler{workdayStatisticsHandler_}
        , mediator{mediator_}
        , statisticsContext{statisticsContext_}
        , settings{settings_}
        , configChangedWatcher{configChangedSignaller_,
                               [this]() { onConfigChanged(); }}
    {
    }

    void onSharedDataChanged() override { }

private:
    asp::QueryHandler<api::WorkdayStatisticsQuery>& workdayStatisticsHandler;
    ui::StatisticsMediator& mediator;
    const ui::StatisticsContext& statisticsContext;
    api::IConfig& settings;
    dw::Weekday cached{settings.firstDayOfWeek()};
    ui::BestWorkdayPresenter presenter{
        workdayStatisticsHandler, mediator, statisticsContext, cached};
    CompositionObserver configChangedWatcher;

    void onConfigChanged()
    {
        if (auto current = settings.firstDayOfWeek(); current != cached) {
            cached = settings.firstDayOfWeek();
            presenter = ui::BestWorkdayPresenter{
                workdayStatisticsHandler, mediator, statisticsContext, cached};
            if (auto v = view(); v) {
                presenter.attachView(*v.value());
            }
        }
    }

    void onViewAttached() override { presenter.attachView(*view().value()); }

    void beforeViewDetached() override
    {
        presenter.detachView(*view().value());
    }
};

} // namespace sprint_timer::compose
