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
#include "qt_gui/widgets/MonthlyProgressView.h"
#include <core/use_cases/RequestSprintDistribution.h>
#include <core/utils/WeekdaySelection.h>

namespace {

dw::TimeSpan twelveMonthsBackTillNow();

} // namespace

namespace sprint_timer::ui::qt_gui {

MonthlyProgressView::MonthlyProgressView(
    IConfig& applicationSettings,
    QueryInvoker& queryInvoker,
    ISprintDistributionReader& monthlyDistributionReader,
    QWidget* parent)
    : ProgressView{GoalValue{applicationSettings.monthlyGoal()},
                   Rows{3},
                   Columns{4},
                   GaugeSize{0.8}}
    , queryInvoker{queryInvoker}
    , distributionReader{monthlyDistributionReader}
{
    setLegendTitle("Last 12 months");
    setLegendAverageCaption("Average per month:");
    setLegendGoalCaption("Monthly goal:");
    connect(this,
            &ProgressView::goalChanged,
            [this, &applicationSettings](int goal) {
                applicationSettings.setMonthlyGoal(goal);
                synchronize();
            });
    synchronize();
}

void MonthlyProgressView::synchronize()
{
    using use_cases::RequestSprintDistribution;
    queryInvoker.execute(std::make_unique<RequestSprintDistribution>(
        distributionReader,
        twelveMonthsBackTillNow(),
        [this](const auto& distribution) {
            setData(distribution, distribution.getNumBins());
        }));
}

} // namespace sprint_timer::ui::qt_gui

namespace {

dw::TimeSpan twelveMonthsBackTillNow()
{
    using dw::DateTime;
    using dw::TimeSpan;
    auto now = DateTime::currentDateTimeLocal();
    auto from = now.add(DateTime::Months{-11});
    from = from.add(DateTime::Days{-std::min(from.day(), now.day()) + 1});
    return TimeSpan{from, now};
}

} // namespace
