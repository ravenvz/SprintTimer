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
#include "qt_gui/widgets/DailyProgressView.h"
#include "qt_gui/dialogs/WorkdaysDialog.h"
#include <QPushButton>
#include <QSpinBox>
#include <core/GroupByDay.h>
#include <core/use_cases/RequestSprintDistribution.h>
#include <core/utils/WeekdaySelection.h>

namespace {

dw::DateRange thirtyDaysBackTillNow();

} // namespace

namespace sprint_timer::ui::qt_gui {

DailyProgressView::DailyProgressView(
    IConfig& applicationSettings_,
    ISprintDistributionReader& dailyDistributionReader_,
    QueryInvoker& queryInvoker_,
    WorkdaysDialog& workdaysDialog_,
    QWidget* parent)
    : ProgressView{Rows{3}, Columns{10}, GaugeSize{0.7}, parent}
    , applicationSettings{applicationSettings_}
    , distributionReader{dailyDistributionReader_}
    , queryInvoker{queryInvoker_}
    , workdaysDialog{workdaysDialog_}
{
    setLegendTitle("Last 30 days");
    setLegendAverageCaption("Average per day:");

    auto configureWorkdaysButton
        = std::make_unique<QPushButton>("Configure workdays");
    connect(configureWorkdaysButton.get(),
            &QPushButton::clicked,
            [&workdaysDialog_]() { workdaysDialog_.exec(); });
    auto spinBoxGoal = std::make_unique<QSpinBox>(this);
    spinBoxGoal->setValue(applicationSettings.dailyGoal());
    connect(spinBoxGoal.get(),
            QOverload<int>::of(&QSpinBox::valueChanged),
            [&](int goal) { emit goalChanged(goal); });
    DailyProgressView::addLegendRow("Workday goal:", spinBoxGoal.release());
    DailyProgressView::addLegendRow(configureWorkdaysButton.release());

    // TODO should set goal directly in spinbox signal handler
    connect(this, &ProgressView::goalChanged, [this](int goal) {
        applicationSettings.setDailyGoal(goal);
        synchronize();
    });
    connect(&workdaysDialog_, &QDialog::accepted, [this]() {
        // TODO maybe first check if they really change before emitting?
        emit workdaysChange();
    });
}

void DailyProgressView::synchronize()
{
    using use_cases::RequestSprintDistribution;
    workdaysDialog.setWorkdayTracker(workdayTracker);
    queryInvoker.execute(std::make_unique<RequestSprintDistribution>(
        distributionReader,
        thirtyDaysBackTillNow(),
        [this](const auto& distribution) {
            GroupByDay groupByDayStrategy;
            setData(ProgressOverPeriod{thirtyDaysBackTillNow(),
                                       distribution,
                                       workdayTracker,
                                       groupByDayStrategy,
                                       applicationSettings.dailyGoal()});
        }));
}

} // namespace sprint_timer::ui::qt_gui

namespace {

dw::DateRange thirtyDaysBackTillNow()
{
    using namespace dw;
    auto now = current_date_local();
    auto from = now - Days{29};
    return {from, now};
}

} // namespace
