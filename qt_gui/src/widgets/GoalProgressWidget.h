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

#ifndef POMODORO_GOALWIDGETTEMP_H
#define POMODORO_GOALWIDGETTEMP_H


#include "core/Distribution.h"
#include "widgets/DataWidget.h"
#include <QGridLayout>
#include <core/IConfig.h>
#include <core/ICoreService.h>

class ProgressView;


class GoalProgressWidget : public DataWidget {
public:
    GoalProgressWidget(IConfig& applicationSettings,
                       ICoreService& pomodoroService,
                       QWidget* parent = 0);
    virtual void synchronize() override;

    virtual QSize sizeHint() const override;

private:
    IConfig& applicationSettings;
    ICoreService& pomodoroService;
    QGridLayout* layout;
    ProgressView* dailyProgress;
    ProgressView* weeklyProgress;
    ProgressView* monthlyProgress;
    const QSize expectedSize{1225, 650};

    void synchronizeDailyData();

    void synchronizeWeeklyData();

    void synchronizeMonthlyData();

    void onDailyDataReceived(const Distribution<int>& distribution);

    void onWeeklyDataReceived(const Distribution<int>& distribution);

    void onMonthlyDataReceived(const Distribution<int>& distribution);
};


#endif // POMODORO_GOALWIDGETTEMP_H
