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

#include "widgets/DefaultTimer.h"
#include "ui_default_timer.h"

DefaultTimer::DefaultTimer(const IConfig& applicationSettings, QWidget* parent)
    : TimerWidgetBase{applicationSettings, parent}
    , ui{new Ui::DefaultTimer}
{
    ui->setupUi(this);

    connect(ui->pbStart, &QPushButton::clicked, this, &DefaultTimer::startTask);
    connect(
        ui->pbCancel, &QPushButton::clicked, this, &DefaultTimer::cancelTask);
    connect(this,
            &TimerWidgetBase::timerUpdated,
            this,
            &DefaultTimer::onTimerUpdated);
    setIdleState();
    connect(ui->pbZone, &QPushButton::clicked, [&]() {
        timer.toggleInTheZoneMode();
    });
    connect(ui->leDoneTask,
            &QLineEdit::returnPressed,
            this,
            &DefaultTimer::requestSubmission);
}

DefaultTimer::~DefaultTimer() { delete ui; }

void DefaultTimer::setSubmissionCandidateDescription(const QString& description)
{
    ui->leDoneTask->setText(description);
}

void DefaultTimer::updateProgress(Progress progress)
{
    int dailyGoal = applicationSettings.dailyPomodorosGoal();
    if (dailyGoal == 0) {
        ui->labelDailyGoalProgress->hide();
        return;
    }
    int completedSoFar = progress;
    ui->labelDailyGoalProgress->setText(QString("Daily goal progress: %1/%2")
                                            .arg(completedSoFar)
                                            .arg(dailyGoal));
    if (completedSoFar == dailyGoal) {
        ui->labelDailyGoalProgress->setStyleSheet("QLabel { color: green; }");
    }
    else if (completedSoFar > dailyGoal) {
        ui->labelDailyGoalProgress->setStyleSheet("QLabel { color: red; }");
    }
    else {
        ui->labelDailyGoalProgress->setStyleSheet("QLabel { color: black; }");
    }
}

void DefaultTimer::setTimerValue(Second timeLeft)
{
    QString timerValue = QString("%1:%2").arg(
        QString::number(timeLeft / secondsPerMinute),
        QString::number(timeLeft % secondsPerMinute).rightJustified(2, '0'));
    ui->labelTimer->setText(timerValue);
}

void DefaultTimer::updateIndication(Second timeLeft)
{
    ui->progressBar->setValue(progressBarMaxValue - timeLeft);
    setTimerValue(timeLeft);
    ui->progressBar->repaint();
}

void DefaultTimer::setRunningState()
{
    progressBarMaxValue = timer.taskDuration() * secondsPerMinute;
    ui->progressBar->setMaximum(progressBarMaxValue);
    setTimerValue(progressBarMaxValue);
    ui->progressBar->setValue(0);
    ui->pbStart->hide();
    ui->labelTimer->show();
    ui->progressBar->show();
    ui->pbCancel->show();
}

void DefaultTimer::setIdleState()
{
    ui->progressBar->setValue(0);
    ui->progressBar->hide();
    ui->leDoneTask->hide();
    ui->pbCancel->hide();
    ui->pbStart->show();
    ui->labelTimer->hide();
    progressBarMaxValue = 0;
}

void DefaultTimer::setSubmissionState()
{
    ui->labelTimer->hide();
    ui->progressBar->hide();
    ui->leDoneTask->show();
}
