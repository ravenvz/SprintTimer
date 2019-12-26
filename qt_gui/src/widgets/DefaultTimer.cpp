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

** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "qt_gui/widgets/DefaultTimer.h"
#include "qt_gui/utils/WidgetUtils.h"
#include "ui_default_timer.h"

namespace sprint_timer::ui::qt_gui {

DefaultTimer::DefaultTimer(const IConfig& applicationSettings_,
                           QAbstractItemModel& sprintModel_,
                           std::unique_ptr<QComboBox> submissionBox_,
                           SprintRegistrator& sprintRegistrator_,
                           QWidget* parent_)
    : TimerWidgetBase{applicationSettings_, sprintModel_, parent_}
    , ui{std::make_unique<Ui::DefaultTimer>()}
    , submissionBox{submissionBox_.get()}
{
    ui->setupUi(this);

    ui->labelTimer->raise();

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
    connect(ui->pbZone, &QPushButton::clicked, [&]() {
        timer->toggleInTheZoneMode();
    });
    connect(ui->pbSubmit, &QPushButton::clicked, [&]() {
        if (submissionBox->currentIndex() != -1)
            requestSubmission();
    });
    connect(this,
            &TimerWidgetBase::submitRequested,
            &sprintRegistrator_,
            &SprintRegistrator::onSubmissionRequested);

    ui->gridLayout->addWidget(
        submissionBox_.release(), 0, 0, 1, 2, Qt::AlignVCenter);
    onIdleStateEnteredHook();
}

DefaultTimer::~DefaultTimer() = default;

void DefaultTimer::setTimerValue(std::chrono::seconds timeLeft)
{
    ui->labelTimer->setText(timerValueToText(timeLeft));
}

void DefaultTimer::onSprintStateEnteredHook() { setUiToRunningState(); }

void DefaultTimer::onBreakStateEnteredHook() { setUiToRunningState(); }

void DefaultTimer::setUiToRunningState()
{
    progressBarMaxValue = static_cast<int>(timer->currentDuration().count());
    submissionBox->hide();
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
    submissionBox->show();
    ui->pbZone->hide();
}

void DefaultTimer::onIdleStateEnteredHook()
{
    ui->progressBar->setValue(0);
    ui->progressBar->hide();
    submissionBox->hide();
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
    ui->progressBar->setValue(progressBarMaxValue -
                              static_cast<int>(timeLeft.count()));
    setTimerValue(timeLeft);
    ui->progressBar->repaint();
}

} // namespace sprint_timer::ui::qt_gui
