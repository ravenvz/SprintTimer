/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include "qt_gui/widgets/WeeklyProgressView.h"
#include <core/GroupByWeek.h>
#include <core/use_cases/RequestSprintDistribution.h>
#include <core/utils/WeekdaySelection.h>

namespace {

dw::TimeSpan
twelveWeeksBackTillNow(sprint_timer::FirstDayOfWeek firstDayOfWeek);

} // namespace

namespace sprint_timer::ui::qt_gui {

WeeklyProgressView::WeeklyProgressView(
    IConfig& applicationSettings,
    QueryInvoker& queryInvoker,
    ISprintDistributionReader& weeklyDistributionReader,
    QWidget* parent)
    : ProgressView{Rows{3}, Columns{4}, GaugeSize{0.8}}
    , applicationSettings{applicationSettings}
    , queryInvoker{queryInvoker}
    , distributionReader{weeklyDistributionReader}
{
    setLegendTitle("Last 12 weeks");
    synchronize();
}

void WeeklyProgressView::synchronize()
{
    using use_cases::RequestSprintDistribution;
    queryInvoker.execute(std::make_unique<RequestSprintDistribution>(
        distributionReader,
        twelveWeeksBackTillNow(firstDayOfWeek),
        [this](const auto& distribution) {
            // TODO perhaps make config return dw::DateTime::Weekday?
            if (applicationSettings.firstDayOfWeek() == FirstDayOfWeek::Monday)
                setData(ProgressOverPeriod{
                    twelveWeeksBackTillNow(firstDayOfWeek),
                    distribution,
                    applicationSettings.workdays(),
                    GroupByWeek{dw::DateTime::Weekday::Monday},
                    applicationSettings.dailyGoal()});
            else
                setData(ProgressOverPeriod{
                    twelveWeeksBackTillNow(firstDayOfWeek),
                    distribution,
                    applicationSettings.workdays(),
                    GroupByWeek{dw::DateTime::Weekday::Sunday},
                    applicationSettings.dailyGoal()});
        }));
}

} // namespace sprint_timer::ui::qt_gui

namespace {

dw::TimeSpan twelveWeeksBackTillNow(sprint_timer::FirstDayOfWeek firstDayOfWeek)
{
    using dw::DateTime;
    using dw::TimeSpan;
    const int firstDayOffset{static_cast<int>(firstDayOfWeek)};
    auto now = DateTime::currentDateTimeLocal();
    const auto from = now.add(DateTime::Days{
        -7 * 11 - static_cast<int>(now.dayOfWeek()) - firstDayOffset});
    const dw::DateTime::Weekday firstDay{
        firstDayOfWeek == sprint_timer::FirstDayOfWeek::Monday
            ? dw::DateTime::Weekday::Monday
            : dw::DateTime::Weekday::Sunday};
    while (true) {
        auto nextDay = now.add(dw::DateTime::Days(1));
        if (nextDay.dayOfWeek() == firstDay)
            break;
        now = nextDay;
    }
    return TimeSpan{from, now};
}

} // namespace
