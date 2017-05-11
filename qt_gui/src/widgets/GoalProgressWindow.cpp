/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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

#include "widgets/GoalProgressWindow.h"
#include "core/utils/WeekdaySelection.h"
#include "widgets/ProgressWidget.h"

namespace {

TimeSpan thirtyDaysBackTillNow()
{
    auto now = DateTime::currentDateTimeLocal();
    auto from = now.add(DateTime::Days{-29});
    return TimeSpan{from, now};
}

TimeSpan twelveWeeksBackTillNow()
{
    auto now = DateTime::currentDateTimeLocal();
    auto from = now.add(DateTime::Days{-7 * 11 - static_cast<int>(now.dayOfWeek())});
    return TimeSpan{from, now};
}

TimeSpan twelveMonthsBackTillNow()
{
    auto now = DateTime::currentDateTimeLocal();
    auto from = now.add(DateTime::Months{-11});
    from = from.add(DateTime::Days{-std::min(from.day(), now.day()) + 1});
    return TimeSpan{from, now};
}

} // namespace

GoalProgressWindow::GoalProgressWindow(IConfig& applicationSettings,
                                       ICoreService& coreService,
                                       QWidget* parent)
    : DataWidget{parent}
    , applicationSettings{applicationSettings}
    , coreService{coreService}
{
    layout = new QGridLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 10, 20, 10);

    configureWorkdaysButton = new QPushButton("Configure", this);

    dailyProgress
        = new ProgressView(applicationSettings.dailyGoal(), 3, 10, 0.7, this);
    dailyProgress->setLegendTitle("Last 30 days");
    dailyProgress->setLegendTotalCaption("Total completed:");
    dailyProgress->setLegendAverageCaption("Average per day:");
    dailyProgress->setLegendPercentageCaption("Goal progress:");
    dailyProgress->setLegendGoalCaption("Daily goal:");
    dailyProgress->addLegendRow("Workdays:", configureWorkdaysButton);

    weeklyProgress
        = new ProgressView(applicationSettings.weeklyGoal(), 3, 4, 0.8, this);
    weeklyProgress->setLegendTitle("Last 12 weeks");
    weeklyProgress->setLegendTotalCaption("Total completed:");
    weeklyProgress->setLegendAverageCaption("Average per week:");
    weeklyProgress->setLegendPercentageCaption("Goal progress:");
    weeklyProgress->setLegendGoalCaption("Weekly goal:");

    monthlyProgress
        = new ProgressView(applicationSettings.monthlyGoal(), 3, 4, 0.8, this);
    monthlyProgress->setLegendTitle("Last 12 months");
    monthlyProgress->setLegendTotalCaption("Total completed:");
    monthlyProgress->setLegendAverageCaption("Average per month:");
    monthlyProgress->setLegendPercentageCaption("Goal progress:");
    monthlyProgress->setLegendGoalCaption("Monthly goal:");

    dailyProgress->setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::MinimumExpanding);
    weeklyProgress->setSizePolicy(QSizePolicy::Preferred,
                                  QSizePolicy::MinimumExpanding);
    monthlyProgress->setSizePolicy(QSizePolicy::Preferred,
                                   QSizePolicy::MinimumExpanding);

    layout->addWidget(dailyProgress, 0, 0, 1, 2);
    layout->addWidget(weeklyProgress, 1, 0, 1, 1);
    layout->addWidget(monthlyProgress, 1, 1, 1, 1);

    setLayout(layout);

    connect(dailyProgress,
            &ProgressView::goalChanged,
            this,
            [this, &applicationSettings](int goal) {
                applicationSettings.setDailyGoal(goal);
                synchronizeDailyData();
            });
    connect(weeklyProgress,
            &ProgressView::goalChanged,
            this,
            [this, &applicationSettings](int goal) {
                applicationSettings.setWeeklyGoal(goal);
                synchronizeWeeklyData();
            });
    connect(monthlyProgress,
            &ProgressView::goalChanged,
            this,
            [this, &applicationSettings](int goal) {
                applicationSettings.setMonthlyGoal(goal);
                synchronizeMonthlyData();
            });
    connect(configureWorkdaysButton,
            &QPushButton::clicked,
            this,
            &GoalProgressWindow::launchWorkdaysConfigurationDialog);

    synchronize();
}

QSize GoalProgressWindow::sizeHint() const { return expectedSize; }

void GoalProgressWindow::synchronize()
{
    synchronizeDailyData();
    synchronizeWeeklyData();
    synchronizeMonthlyData();
}

void GoalProgressWindow::synchronizeDailyData()
{
    coreService.requestSprintDailyDistribution(
        thirtyDaysBackTillNow(), [this](const auto& distribution) {
            this->onDailyDataReceived(distribution);
        });
}

void GoalProgressWindow::synchronizeWeeklyData()
{
    coreService.requestSprintWeeklyDistribution(
        twelveWeeksBackTillNow(), [this](const auto& distribution) {
            this->onWeeklyDataReceived(distribution);
        });
}

void GoalProgressWindow::synchronizeMonthlyData()
{
    coreService.requestSprintMonthlyDistribution(
        twelveMonthsBackTillNow(), [this](const auto& distribution) {
            this->onMonthlyDataReceived(distribution);
        });
}

void GoalProgressWindow::onDailyDataReceived(
    const Distribution<int>& distribution)
{
    dailyProgress->setData(distribution,
                           calculateNumWorkdaysBins(thirtyDaysBackTillNow()));
}

void GoalProgressWindow::onWeeklyDataReceived(
    const Distribution<int>& distribution)
{
    weeklyProgress->setData(distribution, distribution.getNumBins());
}

void GoalProgressWindow::onMonthlyDataReceived(
    const Distribution<int>& distribution)
{
    monthlyProgress->setData(distribution, distribution.getNumBins());
}

void GoalProgressWindow::launchWorkdaysConfigurationDialog()
{
    workdaysDialog.reset(new WorkdaysDialog{applicationSettings});
    workdaysDialog->setModal(true);
    workdaysDialog->show();
}

int GoalProgressWindow::calculateNumWorkdaysBins(const TimeSpan& timeSpan) const
{
    WeekdaySelection workdays{applicationSettings.workdaysCode()};
    int numBins{0};
    for (auto day = timeSpan.start(); day <= timeSpan.finish();
         day = day.add(DateTime::Days{1})) {
        if (workdays.isSelected(
                static_cast<DateTime::Weekday>(day.dayOfWeek())))
            ++numBins;
    }

    return numBins;
}
