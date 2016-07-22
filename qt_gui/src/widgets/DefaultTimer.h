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

#ifndef DEFAULTTIMER_H_QE49BN8Q
#define DEFAULTTIMER_H_QE49BN8Q

#include "widgets/TimerWidgetBase.h"
#include <QWidget>
#include <memory>

namespace Ui {
class DefaultTimer;
}

class DefaultTimer : public TimerWidgetBase {

    Q_OBJECT

public:
    DefaultTimer(const IConfig& applicationSettings, QWidget* parent);
    ~DefaultTimer();
    void setTaskModel(QAbstractItemModel* model) override;
    void setCandidateIndex(int index) override;
    void updateGoalProgress(Progress progress) override;

private:
    Ui::DefaultTimer* ui;
    int progressBarMaxValue{0};

    void setTimerValue(Second timeLeft);
    void onIdleStateEntered() override;
    void onTaskStateEntered() override;
    void setUiToRunningState();
    void onBreakStateEntered() override;
    void onZoneStateEntered() override;
    void onZoneStateLeft() override;
    void onSubmissionStateEntered() override;
    void updateIndication(Second timeLeft) override;
};

#endif /* end of include guard: DEFAULTTIMER_H_QE49BN8Q */
