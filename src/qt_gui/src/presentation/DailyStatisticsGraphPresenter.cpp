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
#include "qt_gui/presentation/DailyStatisticsGraphPresenter.h"
#include <format>
#include <string_view>

namespace {

using GraphValues = std::vector<std::pair<
    sprint_timer::ui::contracts::DailyStatisticGraphContract::DayNumber,
    sprint_timer::ui::contracts::DailyStatisticGraphContract::Value>>;

constexpr std::string_view dailyGraphColor{"#f63c0d"};
constexpr std::string_view averageColor{"#3949c4"};
constexpr std::string_view goalColor{"#39c473"};
constexpr std::string_view pointColor{"#ffffff"};
constexpr double penWidthF{2.2};

void updateAll(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const sprint_timer::api::DailyStatisticsDTO& dailyStatistics,
    const dw::DateRange& range);

void updateLegend(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const sprint_timer::api::DailyStatisticsDTO& dailyStatistics);

void updateDailyGraph(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const sprint_timer::api::DailyStatisticsDTO& dailyStatistics,
    const dw::DateRange& range);

void updateActualAverageGraph(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const sprint_timer::api::DailyStatisticsDTO& dailyStatistics,
    const dw::DateRange& range);

void updateExpectedAverageGraph(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const sprint_timer::api::DailyStatisticsDTO& dailyStatistics,
    const dw::DateRange& range);

std::vector<
    sprint_timer::ui::contracts::DailyStatisticGraphContract::GraphValue>
polylineGraphValues(std::span<const int> distribution,
                    const dw::DateRange& dateRange);

std::vector<
    sprint_timer::ui::contracts::DailyStatisticGraphContract::GraphValue>
buildLineGraph(const dw::DateRange& range, double value);

constexpr size_t sizeInDays(const dw::DateRange& dateRange) noexcept
{
    return static_cast<size_t>(dateRange.duration().count()) + 1;
}

} // namespace

namespace sprint_timer::ui {

DailyStatisticsGraphPresenter::DailyStatisticsGraphPresenter(
    daily_statistics_handler_t& dailyStatisticsHandler_,
    StatisticsMediator& mediator_,
    const StatisticsContext& statisticsContext_)
    : dailyStatisticsHandler{dailyStatisticsHandler_}
    , mediator{mediator_}
    , statisticsContext{statisticsContext_}
{
    mediator.addColleague(this);
}

DailyStatisticsGraphPresenter::~DailyStatisticsGraphPresenter()
{
    mediator.removeColleague(this);
}

void DailyStatisticsGraphPresenter::onSharedDataChanged()
{
    fetchData();
    updateView();
}

void DailyStatisticsGraphPresenter::fetchDataImpl()
{
    const auto [numTopTags, selectedRange, selectedTag] = statisticsContext;
    if (!selectedRange) {
        return;
    }
    dailyStatistics = dailyStatisticsHandler.handle(
        api::DailyStatisticsQuery{numTopTags, selectedTag, *selectedRange});
}

void DailyStatisticsGraphPresenter::updateViewImpl()
{
    auto v = view();
    if (!v) {
        return;
    }

    if (!statisticsContext.selectedRange || !dailyStatistics) {
        v.value()->updateLegend(
            ui::contracts::DailyStatisticGraphContract::LegendData{"No data",
                                                                   "No data"});
        return;
    }

    v.value()->clearGraphs();
    updateAll(v.value(), *dailyStatistics, *statisticsContext.selectedRange);
}

} // namespace sprint_timer::ui

namespace {

void updateAll(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const sprint_timer::api::DailyStatisticsDTO& dailyStatistics,
    const dw::DateRange& range)
{
    updateLegend(view, dailyStatistics);
    updateDailyGraph(view, dailyStatistics, range);
    updateExpectedAverageGraph(view, dailyStatistics, range);
    updateActualAverageGraph(view, dailyStatistics, range);
}

void updateLegend(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const sprint_timer::api::DailyStatisticsDTO& dailyStatistics)
{
    using sprint_timer::ui::contracts::DailyStatisticGraphContract::LegendData;
    view->updateLegend(LegendData{
        std::to_string(dailyStatistics.totalSprints),
        std::format("{:.2f}", dailyStatistics.averageSprintsPerWorkday)});
}

void updateDailyGraph(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const sprint_timer::api::DailyStatisticsDTO& dailyStatistics,
    const dw::DateRange& range)
{
    using namespace sprint_timer::ui::contracts::DailyStatisticGraphContract;
    view->drawGraph(
        GraphData{GraphOptions{penWidthF,
                               std::string{dailyGraphColor},
                               std::string{pointColor},
                               true,
                               LineStyle::Solid},
                  polylineGraphValues(dailyStatistics.distribution, range)});
}

std::vector<
    sprint_timer::ui::contracts::DailyStatisticGraphContract::GraphValue>
polylineGraphValues(std::span<const int> distribution,
                    const dw::DateRange& dateRange)
{
    using namespace sprint_timer::ui::contracts::DailyStatisticGraphContract;
    std::vector<GraphValue> values;
    values.reserve(sizeInDays(dateRange));

    auto toGraphValue = [i = 0, date = dateRange.start()](int val) mutable {
        const auto x = Value{static_cast<double>(i)};
        const auto y = Value{static_cast<double>(val)};
        const auto label_date =
            static_cast<unsigned>((date + dw::Days{i++}).day());
        return GraphValue{x, y, std::to_string(label_date)};
    };

    std::ranges::transform(
        distribution, std::back_inserter(values), toGraphValue);

    return values;
}

void updateActualAverageGraph(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const sprint_timer::api::DailyStatisticsDTO& dailyStatistics,
    const dw::DateRange& range)
{
    using namespace sprint_timer::ui::contracts::DailyStatisticGraphContract;

    const auto average = dailyStatistics.averageSprintsPerWorkday;
    GraphOptions graphOptions{
        penWidthF, std::string{averageColor}, "", false, LineStyle::Solid};
    const auto graphValues = buildLineGraph(range, average);
    view->drawGraph(GraphData{graphOptions, graphValues});
}

void updateExpectedAverageGraph(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const sprint_timer::api::DailyStatisticsDTO& dailyStatistics,
    const dw::DateRange& range)
{
    using namespace sprint_timer::ui::contracts::DailyStatisticGraphContract;

    const auto expectedAverage = dailyStatistics.expectedSprintsPerWorkday;
    const auto graphValues = buildLineGraph(range, expectedAverage);
    GraphOptions graphOptions{
        penWidthF, std::string{goalColor}, "", false, LineStyle::Dash};
    view->drawGraph(GraphData{graphOptions, graphValues});
}

std::vector<
    sprint_timer::ui::contracts::DailyStatisticGraphContract::GraphValue>
buildLineGraph(const dw::DateRange& range, double value)
{
    using namespace sprint_timer::ui::contracts::DailyStatisticGraphContract;
    // Note that sizeInDays(range) >= 1 always
    std::vector<GraphValue> values{
        GraphValue{Value{0}, Value{value}, ""},
        GraphValue{Value{static_cast<double>(sizeInDays(range) - 1)},
                   Value{value},
                   ""}};
    return values;
}

} // namespace
