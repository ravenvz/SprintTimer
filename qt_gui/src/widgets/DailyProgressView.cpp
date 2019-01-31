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

dw::TimeSpan thirtyDaysBackTillNow();

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
{
    setLegendTitle("Last 30 days");
    setLegendAverageCaption("Average per day:");

    auto configureWorkdaysButton = std::make_unique<QPushButton>("Configure");
    connect(configureWorkdaysButton.get(),
            &QPushButton::clicked,
            [&workdaysDialog_]() { workdaysDialog_.exec(); });
    auto spinBoxGoal = std::make_unique<QSpinBox>(this);
    spinBoxGoal->setValue(applicationSettings.dailyGoal());
    connect(spinBoxGoal.get(),
            QOverload<int>::of(&QSpinBox::valueChanged),
            [&](int goal) { emit goalChanged(goal); });
    DailyProgressView::addLegendRow("Workday goal:", spinBoxGoal.release());
    DailyProgressView::addLegendRow("Workdays",
                                    configureWorkdaysButton.release());

    // TODO should set goal directly in spinbox singal handler
    connect(this, &ProgressView::goalChanged, [this](int goal) {
        applicationSettings.setDailyGoal(goal);
        synchronize();
    });
    connect(&workdaysDialog_, &QDialog::accepted, [this]() {
        synchronize();
        emit workdaysChange();
    });
    synchronize();
}

void DailyProgressView::synchronize()
{
    using use_cases::RequestSprintDistribution;
    queryInvoker.execute(std::make_unique<RequestSprintDistribution>(
        distributionReader,
        thirtyDaysBackTillNow(),
        [this](const auto& distribution) {
            GroupByDay groupByDayStrategy;
            setData(ProgressOverPeriod{thirtyDaysBackTillNow(),
                                       distribution,
                                       applicationSettings.workdays(),
                                       groupByDayStrategy,
                                       applicationSettings.dailyGoal()});
        }));
}

} // namespace sprint_timer::ui::qt_gui

namespace {

dw::TimeSpan thirtyDaysBackTillNow()
{
    using namespace dw;
    auto now = DateTime::currentDateTimeLocal();
    auto from = now.add(DateTime::Days{-29});
    return TimeSpan{from, now};
}

} // namespace
