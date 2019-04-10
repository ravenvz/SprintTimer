/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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

#include "qt_gui/widgets/TimerWidgetBase.h"
#include <QAbstractItemModel>
#include <QWidget>
#include <memory>

namespace Ui {
class DefaultTimer;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class DefaultTimer : public TimerWidgetBase {

    Q_OBJECT

public:
    DefaultTimer(const IConfig& applicationSettings_,
                 QAbstractItemModel& taskModel,
                 QWidget* parent);
    ~DefaultTimer() override;
    DefaultTimer(const DefaultTimer&) = delete;
    DefaultTimer(DefaultTimer&&) = delete;
    DefaultTimer& operator=(const DefaultTimer&) = delete;
    DefaultTimer& operator=(const DefaultTimer&&) = delete;

    void setCandidateIndex(int index) override;
    void updateGoalProgress(Progress progress) override;

private:
    std::unique_ptr<Ui::DefaultTimer> ui;
    int progressBarMaxValue{0};

    void setTimerValue(std::chrono::seconds timeLeft);
    void onSprintStateEnteredHook() override;
    void onSprintStateLeftHook() override;
    void onBreakStateEnteredHook() override;
    void onIdleStateEnteredHook() override;
    void onZoneStateEnteredHook() override;
    void onZoneStateLeftHook() override;
    void setUiToRunningState();
    void updateIndication(std::chrono::seconds timeLeft) override;
};

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: DEFAULTTIMER_H_QE49BN8Q */
