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
#include "utils/WidgetUtils.h"

DefaultTimer::DefaultTimer(const IConfig& applicationSettings, QWidget* parent)
    : TimerWidgetBase{applicationSettings, parent}
    , ui{new Ui::DefaultTimer}
{
    ui->setupUi(this);

    WidgetUtils::setRetainSizeWhenHidden(ui->pbCancel);
    WidgetUtils::setRetainSizeWhenHidden(ui->pbZone);
    WidgetUtils::setRetainSizeWhenHidden(ui->progressBar);
    WidgetUtils::setRetainSizeWhenHidden(ui->pbStart);
    WidgetUtils::setRetainSizeWhenHidden(ui->pbSubmit);

    connect(ui->pbStart, &QPushButton::clicked, this, &DefaultTimer::startTask);
    connect(
        ui->pbCancel, &QPushButton::clicked, this, &DefaultTimer::cancelTask);
    connect(this,
            &TimerWidgetBase::timerUpdated,
            this,
            &DefaultTimer::onTimerUpdated);
    onIdleStateEntered();
    connect(ui->pbZone, &QPushButton::clicked, [&]() {
        timer->toggleInTheZoneMode();
    });
    connect(
        ui->cbxSubmissionCandidate,
        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        [&](int index) {
            if (ui->cbxSubmissionCandidate->isVisible())
                emit submissionCandidateChanged(index);
        });
    connect(ui->pbSubmit, &QPushButton::clicked, [&]() {
        if (ui->cbxSubmissionCandidate->currentIndex() != -1)
            requestSubmission();
    });
}

DefaultTimer::~DefaultTimer() { delete ui; }

void DefaultTimer::setTaskModel(QAbstractItemModel* model)
{
    ui->cbxSubmissionCandidate->setModel(model);
}

void DefaultTimer::setCandidateIndex(int index)
{
    ui->cbxSubmissionCandidate->setCurrentIndex(index);
}

void DefaultTimer::updateGoalProgress(Progress progress)
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
    ui->labelTimer->setText(constructTimerValue(timeLeft));
}

void DefaultTimer::onIdleStateEntered()
{
    ui->progressBar->setValue(0);
    ui->progressBar->hide();
    ui->cbxSubmissionCandidate->hide();
    ui->pbCancel->hide();
    ui->pbSubmit->hide();
    ui->pbStart->show();
    ui->labelTimer->hide();
    progressBarMaxValue = 0;
    ui->pbZone->hide();
}

void DefaultTimer::onTaskStateEntered() { setUiToRunningState(); }

void DefaultTimer::onBreakStateEntered() { setUiToRunningState(); }

void DefaultTimer::setUiToRunningState()
{
    progressBarMaxValue = timer->currentDuration() * secondsPerMinute;
    ui->progressBar->setMaximum(progressBarMaxValue);
    setTimerValue(progressBarMaxValue);
    ui->progressBar->setValue(0);
    ui->pbStart->hide();
    ui->pbSubmit->hide();
    ui->labelTimer->show();
    ui->progressBar->show();
    ui->pbCancel->show();
    ui->pbCancel->setEnabled(true);
    ui->pbZone->show();
}

void DefaultTimer::onSubmissionStateEntered()
{
    ui->labelTimer->hide();
    ui->progressBar->hide();
    ui->pbSubmit->show();
    ui->cbxSubmissionCandidate->show();
    ui->pbZone->hide();
}

void DefaultTimer::onZoneStateEntered() { ui->pbCancel->setEnabled(false); }

void DefaultTimer::onZoneStateLeft() { ui->pbCancel->setEnabled(true); }

void DefaultTimer::updateIndication(Second timeLeft)
{
    ui->progressBar->setValue(progressBarMaxValue - timeLeft);
    setTimerValue(timeLeft);
    ui->progressBar->repaint();
}
