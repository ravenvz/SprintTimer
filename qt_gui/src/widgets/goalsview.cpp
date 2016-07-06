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
#include "goalsview.h"
#include "gauge.h"
#include "ui_goalsview.h"

GoalsView::GoalsView(IConfig& applicationSettings,
                     IPomodoroService& pomodoroService,
                     QWidget* parent)
    : DataWidget{parent}
    , ui{new Ui::GoalsView}
    , applicationSettings{applicationSettings}
    , pomodoroService{pomodoroService}

{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->spinBoxDailyGoal->setValue(applicationSettings.dailyPomodorosGoal());
    ui->spinBoxWeeklyGoal->setValue(applicationSettings.weeklyPomodorosGoal());
    ui->spinBoxMonthlyGoal->setValue(
        applicationSettings.monthlyPomodorosGoal());
    displayData();
    connect(ui->spinBoxDailyGoal,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(updateDailyGoal(int)));
    connect(ui->spinBoxWeeklyGoal,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(updateWeeklyGoal(int)));
    connect(ui->spinBoxMonthlyGoal,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(updateMonthlyGoal(int)));
}

GoalsView::~GoalsView() { delete ui; }

void GoalsView::synchronize() { displayData(); }

void GoalsView::displayData()
{
    displayDailyData();
    displayWeeklyData();
    displayMonthlyData();
}

void GoalsView::displayDailyData()
{
    DateTime now = DateTime::currentDateTime();
    DateTime from = now.addDays(-30);
    pomodoroService.requestPomodoroDailyDistribution(
        TimeSpan{from, now},
        std::bind(&GoalsView::onDailyDistributionReceived,
                  this,
                  std::placeholders::_1));
}

void GoalsView::displayWeeklyData()
{
    DateTime now = DateTime::currentDateTime();
    DateTime from
        = now.addDays(-7 * 11 - static_cast<int>(now.dayOfWeek()) + 1);
    pomodoroService.requestPomodoroWeeklyDistribution(
        TimeSpan{from, now},
        std::bind(&GoalsView::onWeeklyDistributionReceived,
                  this,
                  std::placeholders::_1));
}

void GoalsView::displayMonthlyData()
{
    DateTime now = DateTime::currentDateTime();
    DateTime from = now.addMonths(-11).addDays(-static_cast<int>(now.day()));
    pomodoroService.requestPomodoroMonthlyDistribution(
        TimeSpan{from, now},
        std::bind(&GoalsView::onMonthlyDistributionReceived,
                  this,
                  std::placeholders::_1));
}

void GoalsView::updateProgressBar(QProgressBar* bar, int goal, int value)
{
    bar->setMaximum(goal);
    bar->setFormat("%v/%m");
    bar->hide();
    if (goal == 0) {
        return;
    }
    QPalette p = bar->palette();
    if (value == static_cast<int>(goal)) {
        p.setColor(QPalette::Highlight, targetGoalReached);
        p.setColor(QPalette::Base, targetGoalReached);
    }
    else if (value > static_cast<int>(goal)) {
        p.setColor(QPalette::Highlight, overwork);
        p.setColor(QPalette::Base, targetGoalReached);
        bar->setFormat(QString("%1").arg(value) + QString("/%m"));
        value %= goal;
    }
    else {
        p.setColor(QPalette::Highlight, workInProgress);
        p.setColor(QPalette::Base, Qt::white);
    }
    bar->setPalette(p);
    bar->setValue(value);
    bar->show();
}

void GoalsView::drawPeriodDiagram(QGridLayout* layout,
                                  const Distribution<int>& distribution,
                                  int goal,
                                  int rowNum,
                                  int colNum)
{
    clearDiagramLayout(layout);
    GaugeFactory factory;
    for (int row = 0, ind = 0; row < rowNum; ++row) {
        for (int col = 0; col < colNum; ++col, ++ind) {
            layout->addWidget(factory.create(distribution.getBinValue(
                                                 static_cast<unsigned>(ind)),
                                             goal,
                                             this),
                              row,
                              col);
        }
    }
}

void GoalsView::clearDiagramLayout(QGridLayout* layout)
{
    QLayoutItem* child;
    while ((child = layout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
}

void GoalsView::updateDailyGoal(int newValue)
{
    applicationSettings.setDailyPomodorosGoal(static_cast<int>(newValue));
    displayDailyData();
}

void GoalsView::updateWeeklyGoal(int newValue)
{
    applicationSettings.setWeeklyPomodorosGoal(static_cast<int>(newValue));
    displayWeeklyData();
}

void GoalsView::updateMonthlyGoal(int newValue)
{
    applicationSettings.setMonthlyPomodorosGoal(static_cast<int>(newValue));
    displayMonthlyData();
}

void GoalsView::onDailyDistributionReceived(const Distribution<int>& lastDays)
{
    int dailyGoal = applicationSettings.dailyPomodorosGoal();
    int monthlyGoal = applicationSettings.monthlyPomodorosGoal();
    ui->labelLastMonthAverage->setText(formatDecimal(lastDays.getAverage()));
    ui->labelLastMonthPercentage->setText(QString("%1%").arg(
        formatDecimal(percentage(lastDays.getTotal(), monthlyGoal))));
    ui->labelTodayProgress->setText(QString("%1").arg(lastDays.getTotal()));
    updateProgressBar(ui->progressBarToday,
                      dailyGoal,
                      lastDays.getBinValue(lastDays.getNumBins() - 1));
    drawPeriodDiagram(
        ui->gridLayoutLastMonthDiagram, lastDays, dailyGoal, 3, 10);
}

void GoalsView::onWeeklyDistributionReceived(const Distribution<int>& lastWeeks)
{
    int weeklyGoal = applicationSettings.weeklyPomodorosGoal();
    ui->labelLastQuarterAverage->setText(formatDecimal(lastWeeks.getAverage()));
    ui->labelLastQuarterPercentage->setText(QString("%1%").arg(formatDecimal(
        percentage(lastWeeks.getTotal(),
                   static_cast<int>(lastWeeks.getNumBins()) * weeklyGoal))));
    ui->labelWeekProgress->setText(QString("%1").arg(lastWeeks.getTotal()));
    updateProgressBar(ui->progressBarWeek,
                      weeklyGoal,
                      lastWeeks.getBinValue(lastWeeks.getNumBins() - 1));
    drawPeriodDiagram(
        ui->gridLayoutLastQuarterDiagram, lastWeeks, weeklyGoal, 3, 4);
}

void GoalsView::onMonthlyDistributionReceived(
    const Distribution<int>& lastMonths)
{
    int monthlyGoal = applicationSettings.monthlyPomodorosGoal();
    ui->labelLastYearAverage->setText(formatDecimal(lastMonths.getAverage()));
    ui->labelLastYearPercentage->setText(QString("%1%").arg(formatDecimal(
        percentage(lastMonths.getTotal(),
                   static_cast<int>(lastMonths.getNumBins()) * monthlyGoal))));
    ui->labelMonthProgress->setText(QString("%1").arg(lastMonths.getTotal()));
    updateProgressBar(ui->progressBarMonth,
                      monthlyGoal,
                      lastMonths.getBinValue(lastMonths.getNumBins() - 1));
    drawPeriodDiagram(
        ui->gridLayoutLastYearDiagram, lastMonths, monthlyGoal, 3, 4);
}

QString formatDecimal(double decimal)
{
    return QString("%1").arg(decimal, 2, 'f', 2, '0');
}

double percentage(int chunk, int total)
{
    return total != 0 ? static_cast<double>(chunk) * 100 / total : 0;
}
