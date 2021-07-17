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
#ifndef STATISTICSWINDOWPROXY_H_VYTYRJFC
#define STATISTICSWINDOWPROXY_H_VYTYRJFC

#include "DisplayableLifestyleProxy.h"
#include "core/IConfig.h"
#include "core/QueryHandler.h"
#include "core/use_cases/request_sprints/RequestSprintsQuery.h"
#include "qt_gui/utils/WidgetUtils.h"
#include "qt_gui/widgets/StatisticsWindow.h"
#include <QAbstractItemModel>

#include "qt_gui/presentation/BestWorkdayContract.h"
#include "qt_gui/presentation/DailyStatisticsGraphContract.h"
#include "qt_gui/presentation/DateRangeSelectorContract.h"
#include "qt_gui/presentation/BestWorktimeContract.h"
#include "qt_gui/presentation/TopTagDiagramContract.h"
#include "qt_gui/widgets/BestWorkdayWidget.h"
#include "qt_gui/widgets/BestWorktimeWidget.h"
#include "qt_gui/widgets/DailyTimelineGraph.h"
#include "qt_gui/widgets/DateRangeSelector.h"
#include "qt_gui/widgets/TopTagDiagram.h"

namespace sprint_timer::compose {

class StatisticsWindowProxy : public DisplayableLifestyleProxy {
public:
    StatisticsWindowProxy(
        mvp::BasePresenter<ui::contracts::DailyStatisticGraphContract::View>&
            dailyStatisticsGraphPresenter_,
        ui::contracts::BestWorkday::Presenter& bestWorkdayPresenter_,
        mvp::BasePresenter<ui::contracts::BestWorktimeContract::View>&
            bestWorktimePresenter_,
        ui::contracts::TopTagDiagramContract::Presenter& tagDiagramPresenter_,
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
    mvp::BasePresenter<ui::contracts::DailyStatisticGraphContract::View>&
        dailyStatisticsGraphPresenter;
    ui::contracts::BestWorkday::Presenter& bestWorkdayPresenter;
    mvp::BasePresenter<ui::contracts::BestWorktimeContract::View>&
        bestWorktimePresenter;
    ui::contracts::TopTagDiagramContract::Presenter& tagDiagramPresenter;
    ui::contracts::DateRangeSelectorContract::Presenter&
        dateRangeSelectorPresenter;

    std::unique_ptr<ui::qt_gui::StandaloneDisplayableWidget> create() override
    {
        using namespace sprint_timer::ui::qt_gui;
        auto selector = std::make_unique<DateRangeSelector>();
        selector->setPresenter(dateRangeSelectorPresenter);
        auto bestWorkdayView = std::make_unique<BestWorkdayWidget>();
        bestWorkdayView->setPresenter(bestWorkdayPresenter);
        auto distributionDiagram = std::make_unique<TopTagDiagram>();
        distributionDiagram->setPresenter(tagDiagramPresenter);
        auto bestWorktimeView = std::make_unique<BestWorktimeWidget>();
        bestWorktimeView->setPresenter(bestWorktimePresenter);
        auto dailyTimelineGraph = std::make_unique<DailyTimelineGraph>();
        dailyTimelineGraph->setPresenter(dailyStatisticsGraphPresenter);
        return std::make_unique<StatisticsWindow>(
            std::move(dailyTimelineGraph),
            std::move(bestWorkdayView),
            std::move(bestWorktimeView),
            std::move(distributionDiagram),
            std::move(selector));
    }
};

} // namespace sprint_timer::compose

#endif /* end of include guard: STATISTICSWINDOWPROXY_H_VYTYRJFC */
