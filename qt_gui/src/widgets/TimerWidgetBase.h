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

#ifndef ITIMERWIDGET_H_REB4PSQX
#define ITIMERWIDGET_H_REB4PSQX

#include "core/IConfig.h"
#include "core/PomodoroTimer.h"
#include "core/PomodoroTimer.h"
#include "dialogs/confirmationdialog.h"
#include <QMediaPlayer>
#include <QWidget>

#include <QDebug>

using Progress = int;
using Second = int;
constexpr Second secondsPerMinute{60};


class TimerWidgetBase : public QWidget {

    Q_OBJECT

public:
    TimerWidgetBase(const IConfig& applicationSettings, QWidget* parent);
    ~TimerWidgetBase() = default;
    virtual void clearBuffer();
    virtual void setSubmissionCandidateDescription(const QString& description)
        = 0;
    virtual void updateProgress(Progress progress) = 0;

protected:
    const IConfig& applicationSettings;
    PomodoroTimer timer;
    std::vector<TimeSpan> buffer;
    std::unique_ptr<QMediaPlayer> player = std::make_unique<QMediaPlayer>();

    virtual void setIdleState() = 0;
    virtual void setRunningState() = 0;
    virtual void setSubmissionState() = 0;
    virtual void requestSubmission();
    void playSound() const;
    virtual void updateIndication(Second timeLeft) = 0;

protected slots:
    void onTimerTick(long timeLeft);
    void onTimerUpdated(long timeLeft);
    void startTask();
    void cancelTask();
    void onSoundError(QMediaPlayer::Error error);

signals:
    void timerUpdated(long timeLeft);
    void submitRequested(const std::vector<TimeSpan>& completedTaskIntervals);
};

#endif /* end of include guard: ITIMERWIDGET_H_REB4PSQX */
