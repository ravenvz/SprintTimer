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
#include "qt_gui/presentation/DailyStatisticsGraphPresenter.h"
#include <core/use_cases/request_sprint_distribution/Common.h>
#include <numeric>

namespace {

using GraphValues = std::vector<std::pair<
    sprint_timer::ui::contracts::DailyStatisticGraphContract::DayNumber,
    sprint_timer::ui::contracts::DailyStatisticGraphContract::Value>>;

const std::string dailyGraphColor{"#f63c0d"};
const std::string averageColor{"#3949c4"};
const std::string goalColor{"#39c473"};
const std::string pointColor{"#ffffff"};
constexpr double penWidthF{2.2};

void updateAll(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const sprint_timer::Distribution<double>& distribution,
    const sprint_timer::WorkSchedule& schedule,
    const dw::DateRange& range);

void updateLegend(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const std::vector<double>& distribution,
    const sprint_timer::WorkSchedule& schedule,
    const dw::DateRange& range);

void updateDailyGraph(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const std::vector<double>& distribution,
    const dw::DateRange& range);

void updateActualAverageGraph(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const std::vector<double>& distribution,
    const sprint_timer::WorkSchedule& schedule,
    const dw::DateRange& range);

void updateExpectedAverageGraph(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const std::vector<double>& distribution,
    const sprint_timer::WorkSchedule& schedule,
    const dw::DateRange& range);

double computeExpectedAverage(const dw::DateRange& dateRange,
                              const sprint_timer::WorkSchedule& schedule);

double computeActualAverage(const std::vector<double>& distribution,
                            const dw::DateRange& dateRange,
                            const sprint_timer::WorkSchedule& schedule);

std::vector<
    sprint_timer::ui::contracts::DailyStatisticGraphContract::GraphValue>
buildLineGraph(const dw::DateRange& range, double value);

sprint_timer::Distribution<double>
dailyStatistics(const std::vector<sprint_timer::entities::Sprint>& sprints,
                const dw::DateRange& dateRange);

constexpr size_t daysBetween(const dw::Date& date,
                             const dw::DateTime& dateTime) noexcept;

} // namespace

namespace sprint_timer::ui {

DailyStatisticsGraphPresenter::DailyStatisticsGraphPresenter(
    QueryHandler<use_cases::WorkScheduleQuery, WorkSchedule>&
        workScheduleHandler_,
    StatisticsMediator& mediator_)
    : workScheduleHandler{workScheduleHandler_}
    , mediator{mediator_}
{
    mediator.addColleague(this);
}

DailyStatisticsGraphPresenter::~DailyStatisticsGraphPresenter()
{
    mediator.removeColleague(this);
}

void DailyStatisticsGraphPresenter::onSharedDataChanged() { updateView(); }

void DailyStatisticsGraphPresenter::updateViewImpl()
{
    const auto range = mediator.range();
    if (!range) {
        view->updateLegend(
            ui::contracts::DailyStatisticGraphContract::LegendData{"No data",
                                                                   "No data"});
        return;
    }

    const auto& sprints = mediator.sprints();
    const auto schedule =
        workScheduleHandler.handle(use_cases::WorkScheduleQuery{});
    const auto distribution = dailyStatistics(sprints, *range);
    view->clearGraphs();
    updateAll(view, distribution, schedule, *mediator.range());
}

} // namespace sprint_timer::ui

namespace {

void updateAll(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const sprint_timer::Distribution<double>& distribution,
    const sprint_timer::WorkSchedule& schedule,
    const dw::DateRange& range)
{
    const auto& vec = distribution.getDistributionVector();
    updateLegend(view, vec, schedule, range);
    updateDailyGraph(view, vec, range);
    updateExpectedAverageGraph(view, vec, schedule, range);
    updateActualAverageGraph(view, vec, schedule, range);
}

void updateLegend(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const std::vector<double>& distribution,
    const sprint_timer::WorkSchedule& schedule,
    const dw::DateRange& range)
{
    using sprint_timer::ui::contracts::DailyStatisticGraphContract::LegendData;
    using sprint_timer::utils::formatDecimal;
    const auto total =
        std::accumulate(cbegin(distribution), cend(distribution), 0);
    const auto average = computeActualAverage(distribution, range, schedule);
    view->updateLegend(
        LegendData{std::to_string(total), formatDecimal(average)});
}

void updateDailyGraph(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const std::vector<double>& distribution,
    const dw::DateRange& range)
{
    using namespace sprint_timer::ui::contracts::DailyStatisticGraphContract;
    using sprint_timer::use_cases::sizeInDays;

    std::vector<GraphValue> values;
    values.reserve(sizeInDays(range));
    auto date = range.start();
    for (size_t i = 0; i < sizeInDays(range); ++i) {
        values.push_back(GraphValue{
            Value{static_cast<double>(i)},
            Value{distribution[i]},
            std::to_string(
                DayNumber{static_cast<unsigned>(date.day())}.value)});
        date = date + dw::Days{1};
    }
    GraphOptions graphOptions{
        dailyGraphColor, penWidthF, true, pointColor, LineStyle::Solid};
    view->drawGraph(GraphData{graphOptions, values});
}

void updateActualAverageGraph(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const std::vector<double>& distribution,
    const sprint_timer::WorkSchedule& schedule,
    const dw::DateRange& range)
{
    using namespace sprint_timer::ui::contracts::DailyStatisticGraphContract;
    using sprint_timer::use_cases::sizeInDays;

    const auto average = computeActualAverage(distribution, range, schedule);
    GraphOptions graphOptions{
        averageColor, penWidthF, false, "", LineStyle::Solid};
    const auto graphValues = buildLineGraph(range, average);
    view->drawGraph(GraphData{graphOptions, graphValues});
}

void updateExpectedAverageGraph(
    sprint_timer::ui::contracts::DailyStatisticGraphContract::View* view,
    const std::vector<double>& distribution,
    const sprint_timer::WorkSchedule& schedule,
    const dw::DateRange& range)
{
    using namespace sprint_timer::ui::contracts::DailyStatisticGraphContract;
    using sprint_timer::use_cases::sizeInDays;

    const auto average = computeExpectedAverage(range, schedule);
    const auto graphValues = buildLineGraph(range, average);
    GraphOptions graphOptions{goalColor, penWidthF, false, "", LineStyle::Dash};
    view->drawGraph(GraphData{graphOptions, graphValues});
}

double computeExpectedAverage(const dw::DateRange& dateRange,
                              const sprint_timer::WorkSchedule& schedule)
{
    double goal{
        static_cast<double>(sprint_timer::goalFor(schedule, dateRange))};
    int numWorkdays{sprint_timer::numWorkdays(schedule, dateRange)};
    return numWorkdays > 0 ? goal / numWorkdays : 0;
}

double computeActualAverage(const std::vector<double>& distribution,
                            const dw::DateRange& dateRange,
                            const sprint_timer::WorkSchedule& schedule)
{
    double total =
        std::accumulate(cbegin(distribution), cend(distribution), 0.0);
    const auto numWorkdays = sprint_timer::numWorkdays(schedule, dateRange);
    return numWorkdays > 0 ? total / numWorkdays : 0;
}

std::vector<
    sprint_timer::ui::contracts::DailyStatisticGraphContract::GraphValue>
buildLineGraph(const dw::DateRange& range, double value)
{
    using sprint_timer::use_cases::sizeInDays;
    using namespace sprint_timer::ui::contracts::DailyStatisticGraphContract;
    // Note that sizeInDays(range) >= 1 always
    std::vector<GraphValue> values{
        GraphValue{Value{0}, Value{value}, ""},
        GraphValue{Value{static_cast<double>(sizeInDays(range) - 1)},
                   Value{value},
                   ""}};
    return values;
}

sprint_timer::Distribution<double>
dailyStatistics(const std::vector<sprint_timer::entities::Sprint>& sprints,
                const dw::DateRange& dateRange)
{
    using sprint_timer::entities::Sprint;
    using sprint_timer::use_cases::containsDate;
    std::vector<double> sprintsPerDay(dateRange.duration().count() + 1, 0);

    for (const Sprint& sprint : sprints) {
        if (!containsDate(dateRange, sprint.startTime().date()))
            continue;
        const auto dayNumber =
            daysBetween(dateRange.start(), sprint.timeSpan().start());
        ++sprintsPerDay[static_cast<size_t>(dayNumber)];
    }

    return sprint_timer::Distribution<double>{std::move(sprintsPerDay)};
}

constexpr size_t daysBetween(const dw::Date& date,
                             const dw::DateTime& dateTime) noexcept
{
    const auto range = dw::DateRange{
        date, dw::Date{dateTime.year(), dateTime.month(), dateTime.day()}};
    return range.duration().count();
}
} // namespace
