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
#include "qt_gui/widgets/GoalProgressWindow.h"
#include "qt_gui/widgets/ProgressView.h"
#include <QGridLayout>
#include <QPushButton>
#include <core/utils/WeekdaySelection.h>

#include <core/use_cases/RequestSprintDistribution.h>

namespace sprint_timer::ui::qt_gui {

using dw::DateTime;
using dw::TimeSpan;
using use_cases::RequestSprintDistribution;

namespace {

    TimeSpan thirtyDaysBackTillNow();

    TimeSpan twelveWeeksBackTillNow();

    TimeSpan twelveMonthsBackTillNow();

    void setProgressData(ProgressView* progressView,
                         const Distribution<int>& distribution,
                         size_t numBins);

} // namespace

GoalProgressWindow::GoalProgressWindow(
    IConfig& applicationSettings,
    ProgressView* dailyProgress,
    ProgressView* weeklyProgress,
    ProgressView* monthlyProgress,
    WorkdaysDialog& workdaysDialog,
    ISprintDistributionReader& dailyDistributionReader,
    ISprintDistributionReader& weeklyDistributionReader,
    ISprintDistributionReader& monthlyDistributionReader,
    QueryExecutor& queryExecutor,
    QWidget* parent)
    : DataWidget{parent}
    , applicationSettings{applicationSettings}
    , dailyProgress{dailyProgress}
    , weeklyProgress{weeklyProgress}
    , monthlyProgress{monthlyProgress}
    , workdaysDialog{workdaysDialog}
    , dailyDistributionReader{dailyDistributionReader}
    , weeklyDistributionReader{weeklyDistributionReader}
    , monthlyDistributionReader{monthlyDistributionReader}
    , queryExecutor{queryExecutor}
{
    QGridLayout* layout = new QGridLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 10, 20, 10);

    auto* configureWorkdaysButton = new QPushButton("Configure", this);
    dailyProgress->addLegendRow("Workdays:", configureWorkdaysButton);

    dailyProgress->setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::MinimumExpanding);
    weeklyProgress->setSizePolicy(QSizePolicy::Preferred,
                                  QSizePolicy::MinimumExpanding);
    monthlyProgress->setSizePolicy(QSizePolicy::Preferred,
                                   QSizePolicy::MinimumExpanding);

    layout->addWidget(dailyProgress, 0, 0, 1, 2);
    layout->addWidget(weeklyProgress, 1, 0, 1, 1);
    layout->addWidget(monthlyProgress, 1, 1, 1, 1);

    setLayout(layout); // reparents

    setWindowIcon(QIcon(":icons/sprint_timer.png"));

    connect(dailyProgress,
            &ProgressView::goalChanged,
            [this, &applicationSettings](int goal) {
                applicationSettings.setDailyGoal(goal);
                synchronizeDailyData();
            });
    connect(weeklyProgress,
            &ProgressView::goalChanged,
            [this, &applicationSettings](int goal) {
                applicationSettings.setWeeklyGoal(goal);
                synchronizeWeeklyData();
            });
    connect(monthlyProgress,
            &ProgressView::goalChanged,
            [this, &applicationSettings](int goal) {
                applicationSettings.setMonthlyGoal(goal);
                synchronizeMonthlyData();
            });
    connect(configureWorkdaysButton,
            &QPushButton::clicked,
            this,
            &GoalProgressWindow::launchWorkdaysConfigurationDialog);
    connect(&workdaysDialog,
            &QDialog::accepted,
            this,
            &GoalProgressWindow::synchronizeDailyData);

    synchronize();
}

QSize GoalProgressWindow::sizeHint() const { return QSize{1225, 650}; }

void GoalProgressWindow::synchronize()
{
    synchronizeDailyData();
    synchronizeWeeklyData();
    synchronizeMonthlyData();
}

// TODO might be worth it to make ProgressView a hierarhy and put those
// synchronize methods inside

void GoalProgressWindow::synchronizeDailyData()
{
    queryExecutor.executeQuery(std::make_unique<RequestSprintDistribution>(
        dailyDistributionReader,
        thirtyDaysBackTillNow(),
        [this](const auto& distribution) {
            const WeekdaySelection workdays{applicationSettings.workdaysCode()};
            setProgressData(dailyProgress,
                            distribution,
                            numWorkdays(thirtyDaysBackTillNow(), workdays));
        }));
}

void GoalProgressWindow::synchronizeWeeklyData()
{
    queryExecutor.executeQuery(std::make_unique<RequestSprintDistribution>(
        weeklyDistributionReader,
        twelveWeeksBackTillNow(),
        [this](const auto& distribution) {
            setProgressData(
                weeklyProgress, distribution, distribution.getNumBins());
        }));
}

void GoalProgressWindow::synchronizeMonthlyData()
{
    queryExecutor.executeQuery(std::make_unique<RequestSprintDistribution>(
        monthlyDistributionReader,
        twelveMonthsBackTillNow(),
        [this](const auto& distribution) {
            setProgressData(
                monthlyProgress, distribution, distribution.getNumBins());
        }));
}

void GoalProgressWindow::launchWorkdaysConfigurationDialog()
{
    workdaysDialog.exec();
}

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
        auto from = now.add(
            DateTime::Days{-7 * 11 - static_cast<int>(now.dayOfWeek())});
        return TimeSpan{from, now};
    }

    TimeSpan twelveMonthsBackTillNow()
    {
        auto now = DateTime::currentDateTimeLocal();
        auto from = now.add(DateTime::Months{-11});
        from = from.add(DateTime::Days{-std::min(from.day(), now.day()) + 1});
        return TimeSpan{from, now};
    }

    void setProgressData(ProgressView* progressView,
                         const Distribution<int>& distribution,
                         size_t numBins)
    {
        progressView->setData(distribution, numBins);
    }

} // namespace

} // namespace sprint_timer::ui::qt_gui
