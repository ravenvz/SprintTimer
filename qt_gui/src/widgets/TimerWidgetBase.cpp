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
#include "qt_gui/widgets/TimerWidgetBase.h"
#include <QMessageBox>
#include <core/Workflow.h>

namespace sprint_timer::ui::qt_gui {

TimerWidgetBase::TimerWidgetBase(const IConfig& applicationSettings,
                                 QAbstractItemModel& sprintModel,
                                 QWidget* parent)
    : QWidget{parent}
    , applicationSettings{applicationSettings}
    , currentState{IWorkflow::StateId::IdleEntered}
{
    workflow = std::make_unique<Workflow>(
        [this](auto timeLeft) { this->onTimerTick(timeLeft); },
        [this](auto state) { this->onWorkflowStateChanged(state); },
        std::chrono::seconds{1},
        applicationSettings);

    qRegisterMetaType<std::chrono::seconds>("std::chrono::seconds");
    qRegisterMetaType<IWorkflow::StateId>("IWorkflow::StateId");

    connect(
        &sprintModel, &QAbstractItemModel::modelReset, [this, &sprintModel] {
            workflow->setNumFinishedSprints(sprintModel.rowCount());
        });

    connect(player.get(),
            QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
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

void TimerWidgetBase::onWorkflowStateChanged(IWorkflow::StateId state)
{
    emit stateChanged(state);
}

void TimerWidgetBase::onTimerUpdated(std::chrono::seconds timeLeft)
{
    updateIndication(timeLeft);
}

void TimerWidgetBase::onStateChanged(IWorkflow::StateId state)
{
    currentState = state;
    switch (state) {
    case IWorkflow::StateId::SprintEntered:
        onSprintStateEnteredHook();
        break;
    case IWorkflow::StateId::SprintLeft:
        break;
    case IWorkflow::StateId::SprintFinished:
        onSprintStateLeftHook();
        break;
    case IWorkflow::StateId::SprintCancelled:
        onSprintStateCancelledHook();
        break;
    case IWorkflow::StateId::BreakEntered:
        onBreakStateEnteredHook();
        break;
    case IWorkflow::StateId::BreakLeft:
        onBreakStateFinishedHook();
        break;
    case IWorkflow::StateId::BreakCancelled:
        onBreakStateCancelledHook();
        break;
    case IWorkflow::StateId::IdleEntered:
        onIdleStateEnteredHook();
        break;
    case IWorkflow::StateId::IdleLeft:
        onIdleStateLeftHook();
        break;
    case IWorkflow::StateId::ZoneEntered:
        onZoneStateEnteredHook();
        break;
    case IWorkflow::StateId::ZoneLeft:
        onZoneStateLeftHook();
        break;
    default:
        onIdleStateEnteredHook();
    }
}

void TimerWidgetBase::onSprintStateEnteredHook() {}

void TimerWidgetBase::onSprintStateLeftHook() { playSound(); }

void TimerWidgetBase::onSprintStateCancelledHook() {}

void TimerWidgetBase::onBreakStateEnteredHook() {}

void TimerWidgetBase::onBreakStateFinishedHook() { playSound(); }

void TimerWidgetBase::onBreakStateCancelledHook() {}

void TimerWidgetBase::onIdleStateEnteredHook() {}

void TimerWidgetBase::onIdleStateLeftHook() {}

void TimerWidgetBase::onZoneStateEnteredHook() {}

void TimerWidgetBase::onZoneStateLeftHook() {}

void TimerWidgetBase::startTask() { workflow->start(); }

void TimerWidgetBase::cancelTask()
{
    ConfirmationDialog cancelDialog;
    QString description("This will destroy current sprint!");
    cancelDialog.setActionDescription(description);
    if (currentState == IWorkflow::StateId::BreakEntered ||
        cancelDialog.exec()) {
        workflow->cancel();
    }
}

/* NOTE that right now, implementation class must make sure
 * that it has valid submission candidate, otherwise behaviour is undefined. */
void TimerWidgetBase::requestSubmission()
{
    emit submitRequested(workflow->completedSprints());
    workflow->clearSprintsBuffer();
    startTask();
}

void TimerWidgetBase::playSound() const
{
    if (currentState == IWorkflow::StateId::ZoneEntered ||
        !applicationSettings.soundIsEnabled()) {
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

} // namespace sprint_timer::ui::qt_gui
