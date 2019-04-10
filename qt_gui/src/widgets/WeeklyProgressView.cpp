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
#include "qt_gui/widgets/WeeklyProgressView.h"
#include <core/GroupByWeek.h>
#include <core/use_cases/RequestSprintDistribution.h>
#include <core/utils/WeekdaySelection.h>

namespace {

dw::DateRange
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
                setData(
                    ProgressOverPeriod{twelveWeeksBackTillNow(firstDayOfWeek),
                                       distribution,
                                       workdayTracker,
                                       GroupByWeek{dw::Weekday::Monday},
                                       applicationSettings.dailyGoal()});
            else
                setData(
                    ProgressOverPeriod{twelveWeeksBackTillNow(firstDayOfWeek),
                                       distribution,
                                       workdayTracker,
                                       GroupByWeek{dw::Weekday::Sunday},
                                       applicationSettings.dailyGoal()});
        }));
}

} // namespace sprint_timer::ui::qt_gui

namespace {

dw::DateRange
twelveWeeksBackTillNow(sprint_timer::FirstDayOfWeek firstDayOfWeek)
{
    using namespace dw;
    auto now = current_date_local();
    const auto from = prev_weekday(now - Weeks{11},
                                   static_cast<dw::Weekday>(firstDayOfWeek));
    const auto lastDayOfWeek{firstDayOfWeek
                                     == sprint_timer::FirstDayOfWeek::Monday
                                 ? dw::Weekday::Sunday
                                 : dw::Weekday::Saturday};
    const auto to = next_weekday(now, lastDayOfWeek);
    return {from, to};
}

} // namespace
