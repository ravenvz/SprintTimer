/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#include "widgets/GoalProgressWindow.h"
#include "widgets/ProgressWidget.h"

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

    dailyProgress = new ProgressView(
            applicationSettings.dailyGoal(), 3, 10, 0.7, this);
    dailyProgress->setLegendTitle("Last 30 days");
    dailyProgress->setLegendTotalCaption("Total completed:");
    dailyProgress->setLegendAverageCaption("Average per day:");
    dailyProgress->setLegendPercentageCaption("Goal progress:");
    dailyProgress->setLegendGoalCaption("Daily goal:");

    weeklyProgress = new ProgressView(
            applicationSettings.weeklyGoal(), 3, 4, 0.8, this);
    weeklyProgress->setLegendTitle("Last 12 weeks");
    weeklyProgress->setLegendTotalCaption("Total completed:");
    weeklyProgress->setLegendAverageCaption("Average per week:");
    weeklyProgress->setLegendPercentageCaption("Goal progress:");
    weeklyProgress->setLegendGoalCaption("Weekly goal:");

    monthlyProgress = new ProgressView(
            applicationSettings.monthlyGoal(), 3, 4, 0.8, this);
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
    auto now = DateTime::currentDateTime();
    auto from = now.addDays(-29);
    coreService.requestSprintDailyDistribution(
            TimeSpan{from, now},
            std::bind(&GoalProgressWindow::onDailyDataReceived,
                      this,
                      std::placeholders::_1));
}

void GoalProgressWindow::synchronizeWeeklyData()
{
    auto now = DateTime::currentDateTime();
    auto from = now.addDays(-7 * 11 - static_cast<int>(now.dayOfWeek()) + 1);
    coreService.requestSprintWeeklyDistribution(
            TimeSpan{from, now},
            std::bind(&GoalProgressWindow::onWeeklyDataReceived,
                      this,
                      std::placeholders::_1));
}

void GoalProgressWindow::synchronizeMonthlyData()
{
    DateTime now = DateTime::currentDateTime();
    auto from = now.addMonths(-11).addDays(-static_cast<int>(now.day()) + 1);
    coreService.requestSprintMonthlyDistribution(
            TimeSpan{from, now},
            std::bind(&GoalProgressWindow::onMonthlyDataReceived,
                      this,
                      std::placeholders::_1));
}

void GoalProgressWindow::onDailyDataReceived(
    const Distribution<int>& distribution)
{
    dailyProgress->setData(distribution);
}

void GoalProgressWindow::onWeeklyDataReceived(
    const Distribution<int>& distribution)
{
    weeklyProgress->setData(distribution);
}

void GoalProgressWindow::onMonthlyDataReceived(
    const Distribution<int>& distribution)
{
    monthlyProgress->setData(distribution);
}
