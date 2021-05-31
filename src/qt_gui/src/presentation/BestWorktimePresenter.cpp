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

constexpr std::string_view filledColor{"#f63c0d"};

void updateViewWithStubData(
    sprint_timer::ui::contracts::BestWorktimeContract::View& view);

void updateViewWithValidData(
    sprint_timer::ui::contracts::BestWorktimeContract::View& view,
    const sprint_timer::ui::SprintDaytimeDistribution& distribution);

sprint_timer::ui::SprintDaytimeDistribution
buildDistribution(const std::vector<sprint_timer::entities::Sprint>& sprints);

sprint_timer::Distribution<double>
workingHoursStatistics(const std::vector<dw::DateTimeRange>& timeSpans);

std::vector<dw::DateTimeRange>
extractRanges(const std::vector<sprint_timer::entities::Sprint>& sprints);

} // namespace

namespace sprint_timer::ui {

BestWorktimePresenter::BestWorktimePresenter(
    StatisticsMediator& mediator_, const StatisticsContext& statisticsContext_)
    : mediator{mediator_}
    , statisticsContext{statisticsContext_}
{
    mediator.addColleague(this);
}

BestWorktimePresenter::~BestWorktimePresenter()
{
    mediator.removeColleague(this);
}

void BestWorktimePresenter::onSharedDataChanged() { updateView(); }

void BestWorktimePresenter::updateViewImpl()
{
    if (auto v = view(); v) {
        if (!statisticsContext.currentRange()) {
            updateViewWithStubData(*v.value());
            return;
        }
        const auto& sprints = statisticsContext.sprints();
        if (sprints.empty()) {
            updateViewWithStubData(*v.value());
            return;
        }
        const auto distribution = buildDistribution(sprints);
        updateViewWithValidData(*v.value(), distribution);
    }
}

void BestWorktimePresenter::onViewAttached() { updateView(); }

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
    const sprint_timer::ui::SprintDaytimeDistribution& distribution)
{
    using namespace sprint_timer::ui::contracts::BestWorktimeContract;
    using namespace sprint_timer::use_cases;
    const auto maxValueBin = static_cast<unsigned>(
        distribution.dayPartDistribution.getMaxValueBin());
    view.updateLegend(
        LegendData{dayPartName(maxValueBin), dayPartHours(maxValueBin)});
    view.updateDiagram(
        DiagramData{std::string{filledColor}, distribution.timeRanges});
}

sprint_timer::ui::SprintDaytimeDistribution
buildDistribution(const std::vector<sprint_timer::entities::Sprint>& sprints)
{
    const auto ranges = extractRanges(sprints);
    return sprint_timer::ui::SprintDaytimeDistribution{
        workingHoursStatistics(ranges), ranges};
}

sprint_timer::Distribution<double>
workingHoursStatistics(const std::vector<dw::DateTimeRange>& timeSpans)
{
    using namespace sprint_timer::use_cases;
    std::vector<double> sprintsPerDayPart(numParts, 0);

    for (const auto& range : timeSpans) {
        ++sprintsPerDayPart[static_cast<size_t>(dayPart(range))];
    }

    return sprint_timer::Distribution<double>{std::move(sprintsPerDayPart)};
}

std::vector<dw::DateTimeRange>
extractRanges(const std::vector<sprint_timer::entities::Sprint>& sprints)
{
    std::vector<dw::DateTimeRange> sprintRanges;
    sprintRanges.reserve(sprints.size());
    std::transform(cbegin(sprints),
                   cend(sprints),
                   std::back_inserter(sprintRanges),
                   [](const auto& elem) { return elem.timeSpan(); });
    return sprintRanges;
}

} // namespace

