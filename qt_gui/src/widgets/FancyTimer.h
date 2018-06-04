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

#ifndef FANCYTIMER_H_PXO2DFJW
#define FANCYTIMER_H_PXO2DFJW

#include "widgets/CombinedIndicator.h"
#include "widgets/TimerWidgetBase.h"

namespace Ui {
class FancyTimer;
} // namespace Ui

class FancyTimer : public TimerWidgetBase {
public:
    FancyTimer(const IConfig& applicationSettings, QWidget* parent);
    ~FancyTimer() override;

    void setTaskModel(QAbstractItemModel* model) override;
    void setCandidateIndex(int index) override;
    void updateGoalProgress(Progress progress) override;

private:
    Ui::FancyTimer* ui;
    const int indicatorSize{150};
    CombinedIndicator* combinedIndicator;
//    const QColor taskStateColor{QColor{"#eb6c59"}};
//    const QColor breakStateColor{QColor{"#73c245"}};
//    const QColor zoneStateColor{Qt::darkYellow};

    void onSprintStateEnteredHook() override;
    void onSprintStateLeftHook() override;
    void onBreakStateEnteredHook() override;
    void onIdleStateEnteredHook() override;
    void onZoneStateEnteredHook() override;
    void onZoneStateLeftHook() override;
    void updateIndication(std::chrono::seconds timeLeft) override;
    bool indicationUpdateShouldBeIgnored() const;

private slots:
    void onIndicatorClicked();
};

#endif /* end of include guard: FANCYTIMER_H_PXO2DFJW */
