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
#include "qt_gui/presentation/BestWorktimePresenter.h"
#include "core/use_cases/request_sprint_distribution/DayPart.h"

namespace {

struct SprintDaytimeDistribution {
    sprint_timer::Distribution<double> dayPartDistribution;
    std::vector<dw::DateTimeRange> timeRanges;
};

constexpr std::string_view filledColor{"#f63c0d"};

void updateViewWithStubData(
    sprint_timer::ui::contracts::BestWorktimeContract::View& view);

void updateViewWithValidData(
    sprint_timer::ui::contracts::BestWorktimeContract::View& view,
    const sprint_timer::use_cases::WorktimeStatisticsDTO& distribution);

} // namespace

namespace sprint_timer::ui {

BestWorktimePresenter::BestWorktimePresenter(
    worktime_statistics_handler_t& worktimeStatisticsHandler_,
    StatisticsMediator& mediator_,
    const StatisticsContext& statisticsContext_)
    : worktimeStatisticsHandler{worktimeStatisticsHandler_}
    , mediator{mediator_}
    , statisticsContext{statisticsContext_}
{
    mediator.addColleague(this);
}

BestWorktimePresenter::~BestWorktimePresenter()
{
    mediator.removeColleague(this);
}

void BestWorktimePresenter::onSharedDataChanged()
{
    fetchData();
    updateView();
}

void BestWorktimePresenter::fetchDataImpl()
{
    if (!statisticsContext.currentRange()) {
        return;
    }

    worktimeStatistics = worktimeStatisticsHandler.handle(
        use_cases::WorktimeStatisticsQuery{statisticsContext.numTopTags(),
                                           statisticsContext.selectedTag(),
                                           *statisticsContext.currentRange()});
}

void BestWorktimePresenter::updateViewImpl()
{
    if (auto v = view(); v) {
        if (!statisticsContext.currentRange() || !worktimeStatistics) {
            updateViewWithStubData(*v.value());
        }
        else {
            updateViewWithValidData(*v.value(), *worktimeStatistics);
        }
    }
}

} // namespace sprint_timer::ui

namespace {

void updateViewWithStubData(
    sprint_timer::ui::contracts::BestWorktimeContract::View& view)
{
    using namespace sprint_timer::ui::contracts::BestWorktimeContract;
    view.updateLegend(LegendData{"No data", ""});
    view.updateDiagram(DiagramData{std::string{filledColor},
                                   std::vector<dw::DateTimeRange>{}});
}

void updateViewWithValidData(
    sprint_timer::ui::contracts::BestWorktimeContract::View& view,
    const sprint_timer::use_cases::WorktimeStatisticsDTO& distribution)
{
    using namespace sprint_timer::ui::contracts::BestWorktimeContract;
    using namespace sprint_timer::use_cases;
    // const auto maxValueBin = static_cast<unsigned>(
    //     distribution.dayPartDistribution.getMaxValueBin());
    view.updateLegend(LegendData{dayPartName(distribution.bestWorktime),
                                 dayPartHours(distribution.bestWorktime)});
    view.updateDiagram(
        DiagramData{std::string{filledColor}, distribution.timeRanges});
}

} // namespace

