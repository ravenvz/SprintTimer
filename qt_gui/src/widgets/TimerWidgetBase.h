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
using Second = long long;
constexpr Second secondsPerMinute{60};


/* Provides base class for Timer widget. */
class TimerWidgetBase : public QWidget {

    Q_OBJECT

public:
    TimerWidgetBase(const IConfig& applicationSettings, QWidget* parent);
    ~TimerWidgetBase() = default;
    virtual void setTaskModel(QAbstractItemModel* model) = 0;
    virtual void setCandidateIndex(int index) = 0;
    virtual void updateGoalProgress(Progress progress) = 0;

protected:
    const IConfig& applicationSettings;
    std::unique_ptr<IStatefulTimer> timer;
    std::unique_ptr<QMediaPlayer> player = std::make_unique<QMediaPlayer>();

    virtual void onTaskStateEntered() = 0;
    virtual void onBreakStateEntered() = 0;
    virtual void onSubmissionStateEntered() = 0;
    virtual void onIdleStateEntered() = 0;
    virtual void onZoneStateEntered() = 0;
    virtual void onZoneStateLeft() = 0;
    virtual void requestSubmission();
    virtual void updateIndication(Second timeLeft) = 0;
    virtual QString constructTimerValue(Second timeLeft);
    void playSound() const;

protected slots:
    void onTimerTick(long long timeLeft);
    void onTimerStateChanged(IStatefulTimer::State state);
    void onTimerUpdated(long long timeLeft);
    void onStateChanged(IStatefulTimer::State state);
    void startTask();
    void cancelTask();
    void onSoundError(QMediaPlayer::Error error);

signals:
    void timerUpdated(long long timeLeft);
    void stateChanged(IStatefulTimer::State state);
    void submitRequested(std::vector<TimeSpan> completedTaskIntervals);
    void submissionCandidateChanged(int index);
};

#endif /* end of include guard: ITIMERWIDGET_H_REB4PSQX */
