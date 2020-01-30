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
#ifndef ITIMERWIDGET_H_REB4PSQX
#define ITIMERWIDGET_H_REB4PSQX

#include "qt_gui/dialogs/ConfirmationDialog.h"
#include <QAbstractItemModel>
#include <QMediaPlayer>
#include <QWidget>
#include <core/GoalProgress.h>
#include <core/IConfig.h>
#include <core/IWorkflow.h>
#include <memory>

#ifdef _MSC_VER
#include "qt_gui/WinExport.h"
#endif // _MSC_VER

namespace sprint_timer::ui::qt_gui {

using Progress = int;

#ifdef _MSC_VER
class GLIB_EXPORT TimerWidgetBase : public QWidget {
#else
class TimerWidgetBase : public QWidget {
#endif // _MSV_VER
    Q_OBJECT

public:
    TimerWidgetBase(
        const IConfig& applicationSettings,
        QAbstractItemModel& sprintModel, // TODO Workaround to preserve long
                                         // break counter after restart. Remove
                                         // when refactored Workflow
        QWidget* parent);

protected:
    const IConfig& applicationSettings;
    std::unique_ptr<IWorkflow> workflow;
    std::unique_ptr<QMediaPlayer> player = std::make_unique<QMediaPlayer>();
    IWorkflow::StateId currentState;

    virtual void onSprintStateEnteredHook();
    virtual void onSprintStateLeftHook();
    virtual void onSprintStateCancelledHook();
    virtual void onBreakStateEnteredHook();
    virtual void onBreakStateFinishedHook();
    virtual void onBreakStateCancelledHook();
    virtual void onIdleStateEnteredHook();
    virtual void onIdleStateLeftHook();
    virtual void onZoneStateEnteredHook();
    virtual void onZoneStateLeftHook();
    virtual void requestSubmission();
    virtual void updateIndication(std::chrono::seconds timeLeft) = 0;
    virtual QString timerValueToText(std::chrono::seconds timeLeft);
    void playSound() const;

protected slots:
    void onTimerTick(std::chrono::seconds timeLeft);
    void onWorkflowStateChanged(IWorkflow::StateId state);
    void onTimerUpdated(std::chrono::seconds timeLeft);
    void onStateChanged(IWorkflow::StateId state);
    void startTask();
    void cancelTask();
    void onSoundPlaybackError(QMediaPlayer::Error error);

signals:
    void timerUpdated(std::chrono::seconds timeLeft);
    void stateChanged(IWorkflow::StateId state);
    void submitRequested(std::vector<dw::DateTimeRange> completedTaskIntervals);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: ITIMERWIDGET_H_REB4PSQX */
