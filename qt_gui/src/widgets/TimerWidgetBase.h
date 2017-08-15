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

#ifndef ITIMERWIDGET_H_REB4PSQX
#define ITIMERWIDGET_H_REB4PSQX

#include "core/IConfig.h"
#include "core/IStatefulTimer.h"
#include "dialogs/ConfirmationDialog.h"
#include <QAbstractItemModel>
#include <QMediaPlayer>
#include <QWidget>
#include <memory>

using Progress = int;

class TimerWidgetBase : public QWidget {

    Q_OBJECT

public:
    TimerWidgetBase(const IConfig& applicationSettings, QWidget* parent);
    virtual void setTaskModel(QAbstractItemModel* model) = 0;
    virtual void setCandidateIndex(int index) = 0;
    virtual void updateGoalProgress(Progress progress) = 0;

protected:
    const IConfig& applicationSettings;
    std::unique_ptr<SprintTimerCore::IStatefulTimer> timer;
    std::unique_ptr<QMediaPlayer> player = std::make_unique<QMediaPlayer>();
    SprintTimerCore::IStatefulTimer::StateId currentState;

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
    void onTimerStateChanged(SprintTimerCore::IStatefulTimer::StateId state);
    void onTimerUpdated(std::chrono::seconds timeLeft);
    void onStateChanged(SprintTimerCore::IStatefulTimer::StateId state);
    void startTask();
    void cancelTask();
    void onSoundPlaybackError(QMediaPlayer::Error error);

signals:
    void timerUpdated(std::chrono::seconds timeLeft);
    void stateChanged(SprintTimerCore::IStatefulTimer::StateId state);
    void submitRequested(std::vector<dw::TimeSpan> completedTaskIntervals);
    void submissionCandidateChanged(int index);
};

#endif /* end of include guard: ITIMERWIDGET_H_REB4PSQX */
