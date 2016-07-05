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
#ifndef GOALSVIEW_H
#define GOALSVIEW_H

#include "core/IConfig.h"
#include "core/IPomodoroService.h"
#include "widgets/DataWidget.h"
#include <QGridLayout>
#include <QProgressBar>
#include <memory>

namespace Ui {
class GoalsView;
}

class GoalsView : public DataWidget {
    Q_OBJECT

public:
    GoalsView(IConfig& applicationSettings,
              IPomodoroService& pomodoroService,
              QWidget* parent = nullptr);
    ~GoalsView();
    void synchronize() final;

private slots:
    void updateDailyGoal(int newValue);
    void updateWeeklyGoal(int newValue);
    void updateMonthlyGoal(int newValue);
    void onDailyDistributionReceived(const Distribution<int>& lastDays);
    void onWeeklyDistributionReceived(const Distribution<int>& lastWeeks);
    void onMonthlyDistributionReceived(const Distribution<int>& lastMonths);

private:
    Ui::GoalsView* ui;
    IConfig& applicationSettings;
    const QColor targetGoalReached = QColor("#6baa15");
    const QColor overwork = Qt::red;
    const QColor workInProgress = Qt::gray;
    IPomodoroService& pomodoroService;

    void displayData();
    void displayDailyData();
    void displayWeeklyData();
    void displayMonthlyData();
    void drawPeriodDiagram(QGridLayout* layout,
                           const Distribution<int>& distribution,
                           int goal,
                           int rowNum,
                           int colNum);
    void clearDiagramLayout(QGridLayout* layout);
    void updateProgressBar(QProgressBar* bar, int goal, int value);
};

QString formatDecimal(double decimal);

double percentage(int chunk, int total);


#endif
