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
#include "core/use_cases/request_sprint_distribution/Common.h"

namespace {

const std::string barBorderColor{"#f63c0d"};
const std::string barColor{"#73c245"};
const std::vector<int> mondayFirstOrder{1, 2, 3, 4, 5, 6, 7};
const std::vector<int> sundayFirstOrder{7, 1, 2, 3, 4, 5, 6};
// constexpr double penWidth{1.2};

constexpr size_t daysInWeek{7};

sprint_timer::Distribution<double>
weekdayStatistics(const std::vector<sprint_timer::entities::Sprint>& sprints,
                  const dw::DateRange& dateRange);

std::vector<int> weekdayFrequency(const dw::DateRange& dateRange);

} // namespace

namespace sprint_timer::ui {

BestWorkdayPresenter::BestWorkdayPresenter(StatisticsMediator& mediator_,
                                           dw::Weekday firstDayOfWeek_)
    : mediator{mediator_}
    , firstDayOfWeek{std::move(firstDayOfWeek_)}
{
    mediator.get().addColleague(this);
}

BestWorkdayPresenter::~BestWorkdayPresenter()
{
    mediator.get().removeColleague(this);
}

void BestWorkdayPresenter::onSharedDataChanged() { updateView(); }

void BestWorkdayPresenter::updateViewImpl()
{
    using contracts::BestWorkday::View;
    const auto range = mediator.get().range();
    if (!range) {
        updateWithDefaultValues();
        return;
    }

    const auto& sprints = mediator.get().sprints();
    if (sprints.empty()) {
        updateWithDefaultValues();
        return;
    }

    const auto distribution = weekdayStatistics(sprints, *range);
    updateLegend(distribution);
    updateBars(distribution);
}

void BestWorkdayPresenter::onViewAttached() { updateView(); }

void BestWorkdayPresenter::updateLegend(
    const Distribution<double>& distribution) const
{
    using contracts::BestWorkday::View;

    if (auto v = view(); v) {
        const double average = distribution.getAverage();
        const int relativeComparisonInPercent =
            static_cast<int>((distribution.getMax() - average) * 100 / average);
        View::LegendData legendData{
            static_cast<int>(distribution.getMaxValueBin() + 1),
            std::to_string(relativeComparisonInPercent)};
        v.value()->displayLegend(legendData);
    }
}

void BestWorkdayPresenter::updateBars(
    const Distribution<double>& distribution) const
{
    using contracts::BestWorkday::View;

    if (auto v = view(); v) {
        auto values = distribution.getDistributionVector();
        std::rotate(begin(values),
                    begin(values) + static_cast<unsigned>(firstDayOfWeek),
                    end(values));
        v.value()->displayBars(View::BarD{barBorderColor,
                                          barColor,
                                          values,
                                          firstDayOfWeek == dw::Weekday::Monday
                                              ? mondayFirstOrder
                                              : sundayFirstOrder});
    }
}

void BestWorkdayPresenter::updateWithDefaultValues() const
{
    using contracts::BestWorkday::View;

    if (auto v = view(); v) {
        v.value()->displayLegend(View::LegendData{-1, "No data"});
        v.value()->displayBars(View::BarD{barBorderColor,
                                          barColor,
                                          {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                                          firstDayOfWeek == dw::Weekday::Monday
                                              ? mondayFirstOrder
                                              : sundayFirstOrder});
    }
}

} // namespace sprint_timer::ui

namespace {

sprint_timer::Distribution<double>
weekdayStatistics(const std::vector<sprint_timer::entities::Sprint>& sprints,
                  const dw::DateRange& dateRange)
{
    using sprint_timer::use_cases::containsDate;
    std::vector<double> sprintsPerWeekday(daysInWeek, 0);

    // TODO do we really need this check? Maybe we should rely on datastore to
    // return sprints in the correct range and test for it there?
    for (const auto& sprint : sprints) {
        if (!containsDate(dateRange, sprint.startTime().date()))
            continue;
        ++sprintsPerWeekday[static_cast<size_t>(sprint.startTime().weekday())];
    }

    return sprint_timer::Distribution<double>{std::move(sprintsPerWeekday),
                                              weekdayFrequency(dateRange)};
}

std::vector<int> weekdayFrequency(const dw::DateRange& dateRange)
{
    using sprint_timer::use_cases::sizeInDays;
    std::vector<int> result(daysInWeek, 0);
    for (size_t dayNum = 0; dayNum < sizeInDays(dateRange); ++dayNum) {
        const auto date = dateRange.start() + dw::Days{dayNum};
        const auto weekdayNumber = static_cast<unsigned>(dw::weekday(date));
        ++result[weekdayNumber];
    }
    return result;
}

} // namespace
