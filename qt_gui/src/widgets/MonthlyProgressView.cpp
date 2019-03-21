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
#include <core/GroupByMonth.h>
#include <core/use_cases/RequestSprintDistribution.h>
#include <core/utils/WeekdaySelection.h>

namespace {

dw::DateTimeRange twelveMonthsBackTillNow();

} // namespace

namespace sprint_timer::ui::qt_gui {

MonthlyProgressView::MonthlyProgressView(
    IConfig& applicationSettings,
    QueryInvoker& queryInvoker,
    ISprintDistributionReader& monthlyDistributionReader,
    QWidget* parent)
    : ProgressView{Rows{3}, Columns{4}, GaugeSize{0.8}}
    , applicationSettings{applicationSettings}
    , queryInvoker{queryInvoker}
    , distributionReader{monthlyDistributionReader}
{
    setLegendTitle("Last 12 months");
    setLegendAverageCaption("Average per month:");
    synchronize();
}

void MonthlyProgressView::synchronize()
{
    using use_cases::RequestSprintDistribution;
    queryInvoker.execute(std::make_unique<RequestSprintDistribution>(
        distributionReader,
        twelveMonthsBackTillNow(),
        [this](const auto& distribution) {
            setData(ProgressOverPeriod{twelveMonthsBackTillNow(),
                                       distribution,
                                       applicationSettings.workdays(),
                                       GroupByMonth{},
                                       applicationSettings.dailyGoal()});
        }));
}

} // namespace sprint_timer::ui::qt_gui

namespace {

dw::DateTimeRange twelveMonthsBackTillNow()
{
    using namespace dw;

    // const auto now = current_date_local();
    // const auto elevenMonthsBack = now - Months{11};
    // const auto from = Date{elevenMonthsBack.year(), elevenMonthsBack.month(),
    // Day{1}}; const auto to = last_day_of_month(now); return DateRange{from,
    // to};


    const auto now = current_date_local();
    const auto months_back = now - Months{11};
    const auto to = DateTime{last_day_of_month(now)};
    const auto from = Date{months_back.year(), months_back.month(), Day{1}};
    return DateTimeRange{DateTime{from}, DateTime{to}};
}

} // namespace
