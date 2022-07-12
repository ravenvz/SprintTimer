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
#include "qt_gui/presentation/BestWorkdayPresenter.h"
#include <algorithm>
#include <ranges>

namespace {

constexpr std::string_view barBorderColor{"#f63c0d"};
constexpr std::string_view barColor{"#73c245"};
constexpr std::array<int, 7> mondayFirstOrder{1, 2, 3, 4, 5, 6, 7};
constexpr std::array<int, 7> sundayFirstOrder{7, 1, 2, 3, 4, 5, 6};
constexpr size_t daysInWeek{7};
constexpr std::array<double, 7> zeroBarValues{
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
constexpr double threshold{0.001};

} // namespace

namespace sprint_timer::ui {

BestWorkdayPresenter::BestWorkdayPresenter(
    workday_statistics_handler_t& workdayStatisticsHandler_,
    StatisticsMediator& mediator_,
    const StatisticsContext& statisticsContext_,
    dw::Weekday firstDayOfWeek_)
    : workdayStatisticsHandler{workdayStatisticsHandler_}
    , mediator{mediator_}
    , statisticsContext{statisticsContext_}
    , firstDayOfWeek{firstDayOfWeek_}
{
    mediator.get().addColleague(this);
}

BestWorkdayPresenter::~BestWorkdayPresenter()
{
    mediator.get().removeColleague(this);
}

void BestWorkdayPresenter::onSharedDataChanged()
{
    fetchData();
    updateView();
}

void BestWorkdayPresenter::fetchDataImpl()
{
    const auto [numTopTags, selectedRange, selectedTag] =
        statisticsContext.get();
    if (selectedRange) {
        workdayStatistics = workdayStatisticsHandler.get().handle(
            api::WorkdayStatisticsQuery{
                numTopTags, selectedTag, *selectedRange});
    }
}

void BestWorkdayPresenter::updateViewImpl()
{
    using contracts::BestWorkday::View;
    if (!statisticsContext.get().selectedRange || !workdayStatistics) {
        updateWithDefaultValues();
        return;
    }

    updateLegend(workdayStatistics->bestWorkday,
                 workdayStatistics->percentageAboveAverage);
    updateBars(workdayStatistics->sprintDistribution);
}

void BestWorkdayPresenter::updateLegend(dw::Weekday bestWorkday,
                                        int percentageAboveAverage) const
{
    using contracts::BestWorkday::View;

    if (auto v = view(); v) {
        View::LegendData legendData{static_cast<int>(bestWorkday) + 1,
                                    std::to_string(percentageAboveAverage)};
        v.value()->displayLegend(legendData);
    }
}

void BestWorkdayPresenter::updateBars(
    std::array<double, 7> weekdayDistribution) const
{
    using contracts::BestWorkday::View;

    if (auto v = view(); v) {
        std::rotate(begin(weekdayDistribution),
                    begin(weekdayDistribution) +
                        static_cast<unsigned>(firstDayOfWeek),
                    end(weekdayDistribution));
        v.value()->displayBars(View::BarD{
            std::string{barBorderColor},
            std::string{barColor},
            std::span<const double>{weekdayDistribution},
            firstDayOfWeek == dw::Weekday::Monday ? mondayFirstOrder
                                                  : sundayFirstOrder});
    }
}

void BestWorkdayPresenter::updateWithDefaultValues() const
{
    using contracts::BestWorkday::View;

    if (auto v = view(); v) {
        v.value()->displayLegend(View::LegendData{-1, "No data"});
        v.value()->displayBars(View::BarD{std::string{barBorderColor},
                                          std::string{barColor},
                                          zeroBarValues,
                                          firstDayOfWeek == dw::Weekday::Monday
                                              ? mondayFirstOrder
                                              : sundayFirstOrder});
    }
}

} // namespace sprint_timer::ui
