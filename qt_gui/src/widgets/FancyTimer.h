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

enum class State { Idle, Submission, Running };

class FancyTimer : public TimerWidgetBase {
public:
    FancyTimer(const IConfig& applicationSettings, QWidget* parent);
    ~FancyTimer();

    void setSubmissionCandidateDescription(const QString& description) final;
    void setTaskModel(QAbstractItemModel* model) final;
    void setCandidateIndex(int index) final;
    void updateGoalProgress(Progress progress) final;

private:
    Ui::FancyTimer* ui;
    CombinedIndicator* combinedIndicator;
    int progressBarMaxValue{0};
    State state;

    void setTimerValue(Second timeLeft);
    void setIdleState() final;
    void setRunningState() final;
    void setSubmissionState() final;
    void updateIndication(Second timeLeft) final;

private slots:
    void onIndicatorClicked();
};

#endif /* end of include guard: FANCYTIMER_H_PXO2DFJW */
