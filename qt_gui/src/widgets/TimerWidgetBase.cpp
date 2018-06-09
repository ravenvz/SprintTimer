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

#include "widgets/TimerWidgetBase.h"
#include "core/StatefulTimer.h"
#include <QMessageBox>

namespace qt_gui {

TimerWidgetBase::TimerWidgetBase(const IConfig& applicationSettings,
                                 QWidget* parent)
    : QWidget{parent}
    , applicationSettings{applicationSettings}
    , currentState{SprintTimerCore::IStatefulTimer::StateId::IdleEntered}
{
    using namespace SprintTimerCore;
    timer = std::make_unique<StatefulTimer>(
        [this](auto timeLeft) { this->onTimerTick(timeLeft); },
        [this](auto state) { this->onTimerStateChanged(state); },
        std::chrono::seconds{1},
        applicationSettings);

    qRegisterMetaType<std::chrono::seconds>("std::chrono::seconds");
    qRegisterMetaType<SprintTimerCore::IStatefulTimer::StateId>(
            "SprintTimerCore::IStatefulTimer::StateId");

    connect(player.get(),
            static_cast<void (QMediaPlayer::*)(QMediaPlayer::Error)>(
                &QMediaPlayer::error),
            this,
            &TimerWidgetBase::onSoundPlaybackError);
    connect(this,
            &TimerWidgetBase::timerUpdated,
            this,
            &TimerWidgetBase::onTimerUpdated);
    connect(this,
            &TimerWidgetBase::stateChanged,
            this,
            &TimerWidgetBase::onStateChanged);
}

void TimerWidgetBase::onTimerTick(std::chrono::seconds timeLeft)
{
    emit timerUpdated(timeLeft);
}

void TimerWidgetBase::onTimerStateChanged(SprintTimerCore::IStatefulTimer::StateId state)
{
    emit stateChanged(state);
}

void TimerWidgetBase::onTimerUpdated(std::chrono::seconds timeLeft)
{
    updateIndication(timeLeft);
}

void TimerWidgetBase::onStateChanged(SprintTimerCore::IStatefulTimer::StateId state)
{
    using namespace SprintTimerCore;
    currentState = state;
    switch (state) {
    case IStatefulTimer::StateId::SprintEntered:
        onSprintStateEnteredHook();
        break;
    case IStatefulTimer::StateId::SprintFinished:
        onSprintStateLeftHook();
        break;
    case IStatefulTimer::StateId::SprintCancelled:
        onSprintStateCancelledHook();
        break;
    case IStatefulTimer::StateId::BreakEntered:
        onBreakStateEnteredHook();
        break;
    case IStatefulTimer::StateId::BreakLeft:
        onBreakStateFinishedHook();
        break;
    case IStatefulTimer::StateId::BreakCancelled:
        onBreakStateCancelledHook();
        break;
    case IStatefulTimer::StateId::IdleEntered:
        onIdleStateEnteredHook();
        break;
    case IStatefulTimer::StateId::IdleLeft:
        onIdleStateLeftHook();
        break;
    case IStatefulTimer::StateId::ZoneEntered:
        onZoneStateEnteredHook();
        break;
    case IStatefulTimer::StateId::ZoneLeft:
        onZoneStateLeftHook();
        break;
    default:
        onIdleStateEnteredHook();
    }
}

void TimerWidgetBase::onSprintStateEnteredHook()
{

}

void TimerWidgetBase::onSprintStateLeftHook()
{
    playSound();
}

void TimerWidgetBase::onSprintStateCancelledHook()
{

}

void TimerWidgetBase::onBreakStateEnteredHook()
{

}

void TimerWidgetBase::onBreakStateFinishedHook()
{
    playSound();
}

void TimerWidgetBase::onBreakStateCancelledHook()
{

}

void TimerWidgetBase::onIdleStateEnteredHook()
{

}

void TimerWidgetBase::onIdleStateLeftHook()
{

}

void TimerWidgetBase::onZoneStateEnteredHook()
{

}

void TimerWidgetBase::onZoneStateLeftHook()
{

}

void TimerWidgetBase::startTask() { timer->start(); }

void TimerWidgetBase::cancelTask()
{
    using namespace SprintTimerCore;
    ConfirmationDialog cancelDialog;
    QString description("This will destroy current sprint!");
    cancelDialog.setActionDescription(description);
    if (currentState == IStatefulTimer::StateId::BreakEntered
            || cancelDialog.exec()) {
        timer->cancel();
    }
}

/* NOTE that right now, implementation class must make sure
 * that it has valid submission candidate, otherwise behaviour is undefined. */
void TimerWidgetBase::requestSubmission()
{
    emit submitRequested(timer->completedSprints());
    timer->clearSprintsBuffer();
    startTask();
}

void TimerWidgetBase::playSound() const
{
    if (currentState == SprintTimerCore::IStatefulTimer::StateId::ZoneEntered
        || !applicationSettings.soundIsEnabled()) {
        return;
    }
    player->setMedia(QUrl::fromLocalFile(
        QString::fromStdString(applicationSettings.soundFilePath())));
    player->setVolume(applicationSettings.soundVolume());
    player->play();
}

void TimerWidgetBase::onSoundPlaybackError(QMediaPlayer::Error error)
{
    QMessageBox::warning(
        this,
        "Sound playback error",
        QString("Error occured when trying to play sound file:\n %1\n\n%2")
            .arg(QString::fromStdString(applicationSettings.soundFilePath()))
            .arg(player->errorString()));
}


QString TimerWidgetBase::timerValueToText(std::chrono::seconds timeLeft)
{
    using namespace std::chrono;
    minutes min = duration_cast<minutes>(timeLeft);
    seconds sec = timeLeft - min;
    return QString("%1:%2").arg(
        QString::number(min.count()),
        QString::number(sec.count()).rightJustified(2, '0'));
}

} // namespace qt_gui

