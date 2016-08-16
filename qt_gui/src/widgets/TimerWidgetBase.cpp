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

#include "widgets/TimerWidgetBase.h"
#include "core/PomodoroTimer.h"
#include <QMessageBox>


TimerWidgetBase::TimerWidgetBase(const IConfig& applicationSettings,
                                 QWidget* parent)
    : QWidget{parent}
    , applicationSettings{applicationSettings}
{
    timer = std::make_unique<PomodoroTimer>(
        std::bind(&TimerWidgetBase::onTimerTick, this, std::placeholders::_1),
        std::bind(
            &TimerWidgetBase::onTimerStateChanged, this, std::placeholders::_1),
        1000,
        applicationSettings);

    qRegisterMetaType<IStatefulTimer::State>("IStatefulTimer::State");

    connect(player.get(),
            static_cast<void (QMediaPlayer::*)(QMediaPlayer::Error)>(
                &QMediaPlayer::error),
            this,
            &TimerWidgetBase::onSoundError);
    connect(this,
            &TimerWidgetBase::timerUpdated,
            this,
            &TimerWidgetBase::onTimerUpdated);
    connect(this,
            &TimerWidgetBase::stateChanged,
            this,
            &TimerWidgetBase::onStateChanged);
}

void TimerWidgetBase::onTimerTick(long timeLeft)
{
    emit timerUpdated(timeLeft / 1000);
}

void TimerWidgetBase::onTimerStateChanged(IStatefulTimer::State state)
{
    emit stateChanged(state);
}

void TimerWidgetBase::onTimerUpdated(long timeLeft)
{
    int curVal{static_cast<Second>(timeLeft)};
    updateIndication(curVal);
    if (timeLeft == 0)
        playSound();
}

void TimerWidgetBase::onStateChanged(IStatefulTimer::State state)
{
    switch (state) {
    case IStatefulTimer::State::Task:
        onTaskStateEntered();
        break;
    case IStatefulTimer::State::Break:
        onBreakStateEntered();
        break;
    case IStatefulTimer::State::LongBreak:
        onBreakStateEntered();
        break;
    case IStatefulTimer::State::Finished:
        onSubmissionStateEntered();
        break;
    case IStatefulTimer::State::Idle:
        onIdleStateEntered();
        break;
    case IStatefulTimer::State::ZoneEntered:
        onZoneStateEntered();
        break;
    case IStatefulTimer::State::ZoneLeft:
        onZoneStateLeft();
        break;
    default:
        onIdleStateEntered();
    }
}
void TimerWidgetBase::startTask() { timer->start(); }

void TimerWidgetBase::cancelTask()
{
    ConfirmationDialog cancelDialog;
    QString description("This will destroy current sprint!");
    cancelDialog.setActionDescription(description);
    if (timer->state() == IStatefulTimer::State::Break
        || timer->state() == IStatefulTimer::State::LongBreak
        || cancelDialog.exec()) {
        timer->cancel();
    }
}

/* NOTE that right now, implementation class must make sure
 * that it has valid submission candidate, otherwise behaviour is undefined. */
void TimerWidgetBase::requestSubmission()
{
    emit submitRequested(timer->completedTaskIntervals());
    timer->clearIntervalsBuffer();
    startTask();
}

void TimerWidgetBase::playSound() const
{
    if (timer->state() == IStatefulTimer::State::ZoneEntered
        || !applicationSettings.soundIsEnabled()) {
        return;
    }
    player->setMedia(QUrl::fromLocalFile(
        QString::fromStdString(applicationSettings.soundFilePath())));
    player->setVolume(applicationSettings.soundVolume());
    player->play();
}

void TimerWidgetBase::onSoundError(QMediaPlayer::Error error)
{
    QMessageBox::warning(
        this,
        "Sound playback error",
        QString("Error occured when trying to play sound file:\n %1\n\n%2")
            .arg(QString::fromStdString(applicationSettings.soundFilePath()))
            .arg(player->errorString()));
}


QString TimerWidgetBase::constructTimerValue(Second timeLeft)
{
    return QString("%1:%2").arg(
        QString::number(timeLeft / secondsPerMinute),
        QString::number(timeLeft % secondsPerMinute).rightJustified(2, '0'));
}
