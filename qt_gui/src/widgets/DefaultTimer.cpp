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

** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#include "widgets/DefaultTimer.h"
#include "ui_default_timer.h"
#include "utils/WidgetUtils.h"

namespace {
    constexpr char const* workgoalMetStyleSheet{"QLabel { color: green; }"};
    constexpr char const* overworkStyleSheet{"QLabel { color: red; }"};
    constexpr char const* underworkStyleSheet{"QLabel { color: black; }"};
} // namespace

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
    onIdleStateEnteredHook();
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
    int dailyGoal = applicationSettings.dailyGoal();
    if (dailyGoal == 0) {
        ui->labelDailyGoalProgress->hide();
        return;
    }
    ui->labelDailyGoalProgress->setText(QString("Daily goal progress: %1/%2")
                                            .arg(progress)
                                            .arg(dailyGoal));
    if (progress== dailyGoal) {
        ui->labelDailyGoalProgress->setStyleSheet(workgoalMetStyleSheet);
    }
    else if (progress> dailyGoal) {
        ui->labelDailyGoalProgress->setStyleSheet(overworkStyleSheet);
    }
    else {
        ui->labelDailyGoalProgress->setStyleSheet(underworkStyleSheet);
    }
}

void DefaultTimer::setTimerValue(std::chrono::seconds timeLeft)
{
    ui->labelTimer->setText(timerValueToText(timeLeft));
}

void DefaultTimer::onSprintStateEnteredHook() { setUiToRunningState(); }

void DefaultTimer::onBreakStateEnteredHook() { setUiToRunningState(); }

void DefaultTimer::setUiToRunningState()
{
    progressBarMaxValue = timer->currentDuration().count();
    ui->progressBar->setMaximum(progressBarMaxValue);
    setTimerValue(timer->currentDuration());
    ui->progressBar->setValue(0);
    ui->pbStart->hide();
    ui->pbSubmit->hide();
    ui->labelTimer->show();
    ui->progressBar->show();
    ui->pbCancel->show();
    ui->pbCancel->setEnabled(true);
    ui->pbZone->show();
}

void DefaultTimer::onSprintStateLeftHook()
{
    TimerWidgetBase::onSprintStateLeftHook();
    ui->labelTimer->hide();
    ui->pbStart->hide();
    ui->pbCancel->show();
    ui->progressBar->hide();
    ui->pbSubmit->show();
    ui->cbxSubmissionCandidate->show();
    ui->pbZone->hide();
}

void DefaultTimer::onIdleStateEnteredHook()
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

void DefaultTimer::onZoneStateEnteredHook() { ui->pbCancel->setEnabled(false); }

void DefaultTimer::onZoneStateLeftHook() { ui->pbCancel->setEnabled(true); }

void DefaultTimer::updateIndication(std::chrono::seconds timeLeft)
{
    ui->progressBar->setValue(progressBarMaxValue - timeLeft.count());
    setTimerValue(timeLeft);
    ui->progressBar->repaint();
}
