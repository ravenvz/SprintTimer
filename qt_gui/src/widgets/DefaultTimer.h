/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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
