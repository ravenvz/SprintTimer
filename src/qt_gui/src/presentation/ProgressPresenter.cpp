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
#include "qt_gui/presentation/ProgressPresenter.h"

namespace {

using sprint_timer::GoalProgress;
using sprint_timer::ProgressOverPeriod;
using sprint_timer::ui::contracts::DailyProgress::GaugeValues;
using sprint_timer::ui::contracts::DailyProgress::LegendData;
using sprint_timer::ui::contracts::DailyProgress::ProgressBarData;

const std::string normalEmptyColor{"#a0a0a4"};
const std::string normalFilledColor{"#6baa15"};
const std::string overworkEmptyColor{normalFilledColor};
const std::string overworkFilledColor{"#ff0000"};
const std::string restDayEmptyColor{"#ffa500"};

LegendData composeLegendData(const ProgressOverPeriod& progress);

GaugeValues composeGaugeValues(const GoalProgress& progress);

std::vector<GaugeValues> composeGaugeData(const ProgressOverPeriod& progress);

ProgressBarData composeProgressBarData(const ProgressOverPeriod& progress);

} // namespace

namespace sprint_timer::ui {

ProgressPresenter::ProgressPresenter(
    request_progress_hdl_t& requestProgressHandler_)
    : requestProgressHandler{requestProgressHandler_}
{
}

void ProgressPresenter::updateViewImpl()
{
    if (auto v = view(); v) {
        const auto progress =
            requestProgressHandler.handle(use_cases::RequestProgressQuery{});
        v.value()->displayGauges(composeGaugeData(progress));
        v.value()->displayProgressBar(composeProgressBarData(progress));
        v.value()->displayLegend(composeLegendData(progress));
    }
}

void ProgressPresenter::onViewAttached() { updateView(); }

} // namespace sprint_timer::ui

namespace {

LegendData composeLegendData(const ProgressOverPeriod& progress)
{
    using sprint_timer::utils::formatDecimal;
    std::stringstream ss;
    ss << progress.actual() << "/" << progress.estimated();
    const std::string count{ss.str()};
    ss.clear();
    const std::string left{progress.isOverwork() ? "Overwork:"
                                                 : "Left to complete:"};
    const std::string difference{
        std::to_string(std::abs(progress.difference()))};
    const std::string average{
        progress.averagePerGroupPeriod()
            ? formatDecimal(*progress.averagePerGroupPeriod())
            : "n/a"};
    const std::string percentage{
        progress.percentage() ? formatDecimal(*progress.percentage()) + "%"
                              : "n/a"};
    LegendData legendData{count, left, difference, average, percentage};
    return legendData;
}

std::vector<GaugeValues> composeGaugeData(const ProgressOverPeriod& progress)
{
    std::vector<GaugeValues> gaugeValues;
    gaugeValues.reserve(progress.size());
    for (size_t i = 0; i < progress.size(); ++i) {
        gaugeValues.push_back(composeGaugeValues(progress.getValue(i)));
    }
    return gaugeValues;
}

GaugeValues composeGaugeValues(const GoalProgress& progress)
{
    std::string emptyColor;
    std::string filledColor;

    if (progress.estimated() == 0) {
        emptyColor = restDayEmptyColor;
        filledColor = restDayEmptyColor;
    }
    else if (progress.actual() == 0) {
        emptyColor = normalEmptyColor;
        filledColor = normalEmptyColor;
    }
    else if (progress.actual() == progress.estimated()) {
        emptyColor = normalFilledColor;
        filledColor = normalFilledColor;
    }
    else if (progress.actual() > progress.estimated()) {
        emptyColor = overworkEmptyColor;
        filledColor = overworkFilledColor;
    }
    else if (progress.actual() < progress.estimated()) {
        emptyColor = normalEmptyColor;
        filledColor = normalFilledColor;
    }
    return GaugeValues{
        progress.estimated(), progress.actual(), emptyColor, filledColor};
}

ProgressBarData composeProgressBarData(const ProgressOverPeriod& progress)
{
    std::string emptyColor{"#ffffff"};
    std::string filledColor{normalEmptyColor};

    if (progress.size() == 0)
        return ProgressBarData{progress.estimated(),
                               progress.actual(),
                               emptyColor,
                               emptyColor,
                               false};

    const auto& lastBin = progress.getValue(progress.size() - 1);

    if (lastBin.actual() > lastBin.estimated()) {
        emptyColor = normalFilledColor;
        filledColor = overworkFilledColor;
    }
    else if (lastBin.actual() == lastBin.estimated()) {
        filledColor = normalFilledColor;
    }
    return ProgressBarData{lastBin.estimated(),
                           lastBin.actual(),
                           emptyColor,
                           filledColor,
                           lastBin.estimated() != 0};
}

} // namespace

