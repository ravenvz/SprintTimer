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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>


FancyTimer::FancyTimer(const IConfig& applicationSettings, QWidget* parent)
    : TimerWidgetBase{applicationSettings, parent}
    , ui{new Ui::FancyTimer}
{
    ui->setupUi(this);
    combinedIndicator = new CombinedIndicator();
    ui->gridLayout->addWidget(combinedIndicator, 1, 0, Qt::AlignCenter);
    setIdleState();
    connect(combinedIndicator,
            &CombinedIndicator::indicatorClicked,
            this,
            &FancyTimer::onIndicatorClicked);
    connect(this, &FancyTimer::timerUpdated, this, &FancyTimer::onTimerUpdated);
    connect(
        ui->cbxSubmissionCandidate,
        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        [&](int index) { emit submissionCandidateChanged(index); });
}

FancyTimer::~FancyTimer() { delete ui; }

void FancyTimer::setSubmissionCandidateDescription(const QString& description)
{
}

void FancyTimer::setTaskModel(QAbstractItemModel* model)
{
    ui->cbxSubmissionCandidate->setModel(model);
}

void FancyTimer::updateGoalProgress(Progress progress) {}

void FancyTimer::setIdleState()
{
    state = State::Idle;
    ui->cbxSubmissionCandidate->hide();
    combinedIndicator->setColor(QColor{"#eb6c59"});
    combinedIndicator->setStatus("Start");
    combinedIndicator->setDuration(timer.taskDuration() * secondsPerMinute);
    combinedIndicator->repaint();
}

void FancyTimer::setRunningState()
{
    state = State::Running;
    ui->cbxSubmissionCandidate->hide();
    auto duration = timer.taskDuration() * secondsPerMinute;
    combinedIndicator->updateIndication(duration);
    combinedIndicator->setStatus(constructTimerValue(duration));
    combinedIndicator->repaint();
}

void FancyTimer::setSubmissionState()
{
    state = State::Submission;
    ui->cbxSubmissionCandidate->show();
    combinedIndicator->setColor(QColor("#73c245"));
    combinedIndicator->setDuration(timer.taskDuration() * secondsPerMinute);
    combinedIndicator->setStatus("Submit");
}

void FancyTimer::updateIndication(Second timeLeft)
{
    combinedIndicator->updateIndication(timeLeft);
    combinedIndicator->setStatus(constructTimerValue(timeLeft));
    combinedIndicator->repaint();
}

void FancyTimer::onIndicatorClicked()
{
    switch (state) {
    case State::Submission:
        requestSubmission();
        break;
    case State::Idle:
        startTask();
        break;
    default:
        break;
    }
}

void FancyTimer::setCandidateIndex(int index)
{
    ui->cbxSubmissionCandidate->setCurrentIndex(index);
}
