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
#include <QMessageBox>


TimerWidgetBase::TimerWidgetBase(const IConfig& applicationSettings,
                                 QWidget* parent)
    : QWidget{parent}
    , applicationSettings{applicationSettings}
    , timer{
          std::bind(&TimerWidgetBase::onTimerTick, this, std::placeholders::_1),
          1000,
          applicationSettings}
{
    connect(player.get(),
            static_cast<void (QMediaPlayer::*)(QMediaPlayer::Error)>(
                &QMediaPlayer::error),
            this,
            &TimerWidgetBase::onSoundError);
}

void TimerWidgetBase::clearBuffer() { buffer.clear(); }

void TimerWidgetBase::onTimerTick(long timeLeft)
{
    emit timerUpdated(timeLeft / 1000);
}

void TimerWidgetBase::startTask()
{
    timer.run();
    setRunningState();
}

void TimerWidgetBase::cancelTask()
{
    ConfirmationDialog cancelDialog;
    QString description("This will destroy current pomodoro!");
    cancelDialog.setActionDescription(description);
    if (timer.isBreak() || cancelDialog.exec()) {
        timer.cancel();
        setIdleState();
    }
}

void TimerWidgetBase::requestSubmission()
{
    emit submitRequested(buffer);
    setRunningState();
    startTask();
}

void TimerWidgetBase::playSound() const
{
    if (timer.inTheZone() || !applicationSettings.soundIsEnabled()) {
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

void TimerWidgetBase::onTimerUpdated(long timeLeft)
{
    int curVal{static_cast<Second>(timeLeft)};
    updateIndication(curVal);
    if (timeLeft > 0)
        return;

    playSound();

    if (timer.inTheZone()) {
        buffer.push_back(timer.finish());
        startTask();
    }
    else if (timer.isBreak()) {
        timer.finish();
        setIdleState();
    }
    else {
        buffer.push_back(timer.finish());
        setSubmissionState();
    }
}
