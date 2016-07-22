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

#ifndef FANCYTIMER_H_PXO2DFJW
#define FANCYTIMER_H_PXO2DFJW

#include "widgets/CombinedIndicator.h"
#include "widgets/TimerWidgetBase.h"

namespace Ui {
class FancyTimer;
}

class FancyTimer : public TimerWidgetBase {
public:
    FancyTimer(const IConfig& applicationSettings, QWidget* parent);
    ~FancyTimer();

    void setTaskModel(QAbstractItemModel* model) override;
    void setCandidateIndex(int index) override;
    void updateGoalProgress(Progress progress) override;

private:
    Ui::FancyTimer* ui;
    CombinedIndicator* combinedIndicator;
    const QColor taskStateColor{QColor{"#eb6c59"}};
    const QColor breakStateColor{QColor{"#73c245"}};
    const QColor zoneStateColor{Qt::darkYellow};

    void onIdleStateEntered() override;
    void onTaskStateEntered() override;
    void setUiToRunningState();
    void onSubmissionStateEntered() override;
    void onBreakStateEntered() override;
    void onZoneStateEntered() override;
    void onZoneStateLeft() override;
    void updateIndication(Second timeLeft) override;

private slots:
    void onIndicatorClicked();
};

#endif /* end of include guard: FANCYTIMER_H_PXO2DFJW */
