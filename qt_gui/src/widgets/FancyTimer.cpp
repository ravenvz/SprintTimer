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

#include "widgets/FancyTimer.h"
#include "ui_fancy_timer.h"
#include "utils/WidgetUtils.h"


FancyTimer::FancyTimer(const IConfig& applicationSettings, QWidget* parent)
    : TimerWidgetBase{applicationSettings, parent}
    , ui{new Ui::FancyTimer}
{
    ui->setupUi(this);
    combinedIndicator = new CombinedIndicator(indicatorSize, this);
    combinedIndicator->setSizePolicy(QSizePolicy::MinimumExpanding,
                                     QSizePolicy::MinimumExpanding);
    ui->gridLayout->addWidget(
        combinedIndicator, 2, 0, 1, 2, Qt::AlignHCenter | Qt::AlignTop);

    WidgetUtils::setRetainSizeWhenHidden(ui->pbCancel);
    WidgetUtils::setRetainSizeWhenHidden(ui->pbZone);
    WidgetUtils::setRetainSizeWhenHidden(ui->cbxSubmissionCandidate);

    connect(combinedIndicator,
            &CombinedIndicator::indicatorClicked,
            this,
            &FancyTimer::onIndicatorClicked);
    connect(
        ui->cbxSubmissionCandidate,
        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        [&](int index) {
            if (ui->cbxSubmissionCandidate->isVisible()) {
                emit submissionCandidateChanged(index);
            }
        });
    connect(ui->pbZone, &QPushButton::clicked, [&]() {
        timer->toggleInTheZoneMode();
    });
    connect(ui->pbCancel, &QPushButton::clicked, this, &FancyTimer::cancelTask);

    onIdleStateEntered();
}

FancyTimer::~FancyTimer() { delete ui; }

void FancyTimer::setTaskModel(QAbstractItemModel* model)
{
    ui->cbxSubmissionCandidate->setModel(model);
}

void FancyTimer::setCandidateIndex(int index)
{
    if (ui->cbxSubmissionCandidate->isVisible())
        ui->cbxSubmissionCandidate->setCurrentIndex(index);
}

void FancyTimer::updateGoalProgress(Progress progress)
{
    int dailyGoal = applicationSettings.dailyGoal();
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

void FancyTimer::onIdleStateEntered()
{
    ui->cbxSubmissionCandidate->hide();
    ui->pbCancel->hide();
    ui->pbZone->hide();
    combinedIndicator->setText("Start");
    combinedIndicator->setInvertedStyle(false);
    combinedIndicator->setDrawArc(false);
    combinedIndicator->repaint();
}

void FancyTimer::onTaskStateEntered()
{
    ui->cbxSubmissionCandidate->hide();
    ui->pbCancel->show();
    ui->pbZone->show();
    auto duration = timer->currentDuration() * secondsPerMinute;
    combinedIndicator->setColor(taskStateColor);
    combinedIndicator->setText(constructTimerValue(duration));
    combinedIndicator->setMaxValue(duration);
    combinedIndicator->setCurrentValue(duration);
    combinedIndicator->setInvertedStyle(false);
    combinedIndicator->setDrawArc(true);
    combinedIndicator->repaint();
}

void FancyTimer::onBreakStateEntered()
{
    ui->cbxSubmissionCandidate->hide();
    ui->pbCancel->show();
    ui->pbZone->hide();
    auto duration = timer->currentDuration() * secondsPerMinute;
    combinedIndicator->setColor(breakStateColor);
    combinedIndicator->setMaxValue(duration);
    updateIndication(duration);
    combinedIndicator->setCurrentValue(duration);
    combinedIndicator->setText(constructTimerValue(duration));
    combinedIndicator->setInvertedStyle(true);
    combinedIndicator->setDrawArc(true);
    combinedIndicator->repaint();
}

void FancyTimer::onZoneStateEntered()
{
    ui->pbCancel->hide();
    combinedIndicator->setColor(zoneStateColor);
    combinedIndicator->repaint();
}

void FancyTimer::onZoneStateLeft()
{
    ui->pbCancel->show();
    combinedIndicator->setColor(taskStateColor);
    combinedIndicator->repaint();
}

void FancyTimer::onSubmissionStateEntered()
{
    qDebug() << "Setting submission state";
    ui->cbxSubmissionCandidate->show();
    ui->pbCancel->show();
    ui->pbZone->hide();
    combinedIndicator->setText("Submit");
    combinedIndicator->setDrawArc(false);
    combinedIndicator->repaint();
}

void FancyTimer::updateIndication(Second timeLeft)
{
    if (timer->state() == IStatefulTimer::State::Idle
        || timer->state() == IStatefulTimer::State::Finished)
        return;
    combinedIndicator->setCurrentValue(timeLeft);
    combinedIndicator->setText(constructTimerValue(timeLeft));
    combinedIndicator->repaint();
}

void FancyTimer::onIndicatorClicked()
{
    switch (timer->state()) {
    case IStatefulTimer::State::Finished:
        if (ui->cbxSubmissionCandidate->currentIndex() != -1)
            requestSubmission();
        break;
    case IStatefulTimer::State::Idle:
        startTask();
        break;
    default:
        break;
    }
}

void FancyTimer::setUiToRunningState()
{

}
