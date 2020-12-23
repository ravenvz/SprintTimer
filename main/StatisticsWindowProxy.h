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
#ifndef STATISTICSWINDOWPROXY_H_VYTYRJFC
#define STATISTICSWINDOWPROXY_H_VYTYRJFC

#include "DisplayableLifestyleProxy.h"
#include <QAbstractItemModel>
#include <core/IConfig.h>
#include <core/QueryHandler.h>
#include <core/use_cases/request_sprints/RequestSprintsQuery.h>
#include <qt_gui/DatasyncRelay.h>
#include <qt_gui/WorkScheduleWrapper.h>
#include <qt_gui/utils/WidgetUtils.h>
#include <qt_gui/widgets/StatisticsWindow.h>

#include <qt_gui/presentation/BasePresenter.h>
#include <qt_gui/presentation/BestWorkdayContract.h>
#include <qt_gui/presentation/DailyStatisticsGraphContract.h>
#include <qt_gui/presentation/DateRangeSelectorContract.h>
#include <qt_gui/presentation/DaytimeStatisticsContract.h>
#include <qt_gui/presentation/TagPieDiagramContract.h>
#include <qt_gui/widgets/BestWorkdayWidget.h>
#include <qt_gui/widgets/BestWorktimeWidget.h>
#include <qt_gui/widgets/DailyTimelineGraph.h>
#include <qt_gui/widgets/DateRangeSelector.h>
#include <qt_gui/widgets/DistributionDiagram.h>

namespace sprint_timer::compose {

class StatisticsWindowProxy : public DisplayableLifestyleProxy {
public:
    StatisticsWindowProxy(
        // IConfig& settings_,
        // TODO rename - remove contract from name, it might be inferred from
        // namespace
        ui::BasePresenter<ui::contracts::DailyStatisticGraphContract::View>&
            dailyStatisticsGraphPresenter_,
        ui::BasePresenter<ui::contracts::BestWorkday::View>&
            bestWorkdayPresenter_,
        ui::BasePresenter<ui::contracts::DaytimeStatisticsContract::View>&
            bestWorktimePresenter_,
        ui::contracts::TagPieDiagramContract::Presenter& tagDiagramPresenter_,
        ui::contracts::DateRangeSelectorContract::Presenter&
            dateRangeSelectorPresenter_)
        : dailyStatisticsGraphPresenter{dailyStatisticsGraphPresenter_}
        , bestWorkdayPresenter{bestWorkdayPresenter_}
        , bestWorktimePresenter{bestWorktimePresenter_}
        , tagDiagramPresenter{tagDiagramPresenter_}
        , dateRangeSelectorPresenter{dateRangeSelectorPresenter_}
    {
    }

private:
    ui::BasePresenter<ui::contracts::DailyStatisticGraphContract::View>&
        dailyStatisticsGraphPresenter;
    ui::BasePresenter<ui::contracts::BestWorkday::View>& bestWorkdayPresenter;
    ui::BasePresenter<ui::contracts::DaytimeStatisticsContract::View>&
        bestWorktimePresenter;
    ui::contracts::TagPieDiagramContract::Presenter& tagDiagramPresenter;
    ui::contracts::DateRangeSelectorContract::Presenter&
        dateRangeSelectorPresenter;

    std::unique_ptr<ui::qt_gui::StandaloneDisplayableWidget> create() override
    {
        using namespace sprint_timer::ui::qt_gui;
        return std::make_unique<StatisticsWindow>(
            std::make_unique<DailyTimelineGraph>(dailyStatisticsGraphPresenter),
            std::make_unique<BestWorkdayWidget>(bestWorkdayPresenter),
            std::make_unique<BestWorktimeWidget>(bestWorktimePresenter),
            std::make_unique<DistributionDiagram>(tagDiagramPresenter),
            std::make_unique<DateRangeSelector>(dateRangeSelectorPresenter));
    }
};

} // namespace sprint_timer::compose

#endif /* end of include guard: STATISTICSWINDOWPROXY_H_VYTYRJFC */
