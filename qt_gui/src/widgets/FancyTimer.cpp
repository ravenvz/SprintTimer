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
#include "qt_gui/widgets/FancyTimer.h"
#include "qt_gui/utils/WidgetUtils.h"
#include "ui_fancy_timer.h"

namespace sprint_timer::ui::qt_gui {

namespace {

    constexpr char const* workgoalMetStyleSheet = "QLabel { color: green; }";
    constexpr char const* overworkStyleSheet = "QLabel { color: red; }";
    constexpr char const* underworkStyleSheet = "QLabel { color: black; }";
    const QColor taskStateColor {"#eb6c59"};
    const QColor breakStateColor {"#73c245"};
    const QColor zoneStateColor {Qt::darkYellow};

} // namespace

FancyTimer::FancyTimer(const IConfig& applicationSettings, QWidget* parent)
    : TimerWidgetBase {applicationSettings, parent}
    , ui {new Ui::FancyTimer}
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
    connect(ui->cbxSubmissionCandidate,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            [&](int index) {
                if (ui->cbxSubmissionCandidate->isVisible())
                    emit submissionCandidateChanged(index);
            });
    connect(ui->pbZone, &QPushButton::clicked, [&]() {
        timer->toggleInTheZoneMode();
    });
    connect(ui->pbCancel, &QPushButton::clicked, this, &FancyTimer::cancelTask);

    ui->cbxSubmissionCandidate->setItemDelegate(submissionItemDelegate.get());

    onIdleStateEnteredHook();
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
    timer->setNumFinishedSprints(progress);
    const auto dailyGoal = applicationSettings.dailyGoal();
    if (dailyGoal == 0) {
        ui->labelDailyGoalProgress->hide();
        return;
    }
    ui->labelDailyGoalProgress->setText(
        QString("Daily goal progress: %1/%2").arg(progress).arg(dailyGoal));
    if (progress == dailyGoal) {
        ui->labelDailyGoalProgress->setStyleSheet(workgoalMetStyleSheet);
    }
    else if (progress > dailyGoal) {
        ui->labelDailyGoalProgress->setStyleSheet(overworkStyleSheet);
    }
    else {
        ui->labelDailyGoalProgress->setStyleSheet(underworkStyleSheet);
    }
}

void FancyTimer::onSprintStateEnteredHook()
{
    ui->cbxSubmissionCandidate->hide();
    ui->pbCancel->show();
    ui->pbZone->show();
    std::chrono::seconds duration = timer->currentDuration();
    combinedIndicator->setColor(taskStateColor);
    combinedIndicator->setText(timerValueToText(duration));
    combinedIndicator->setMaxValue(static_cast<int>(duration.count()));
    combinedIndicator->setCurrentValue(static_cast<int>(duration.count()));
    combinedIndicator->setInvertedStyle(false);
    combinedIndicator->setDrawArc(true);
    combinedIndicator->repaint();
}

void FancyTimer::onSprintStateLeftHook()
{
    TimerWidgetBase::onSprintStateLeftHook();
    ui->cbxSubmissionCandidate->show();
    ui->pbCancel->show();
    ui->pbZone->hide();
    combinedIndicator->setText("Submit");
    combinedIndicator->setDrawArc(false);
    combinedIndicator->repaint();
}

void FancyTimer::onBreakStateEnteredHook()
{
    ui->cbxSubmissionCandidate->hide();
    ui->pbCancel->show();
    ui->pbZone->hide();
    std::chrono::seconds duration = timer->currentDuration();
    combinedIndicator->setColor(breakStateColor);
    combinedIndicator->setMaxValue(static_cast<int>(duration.count()));
    updateIndication(duration);
    combinedIndicator->setCurrentValue(static_cast<int>(duration.count()));
    combinedIndicator->setText(timerValueToText(duration));
    combinedIndicator->setInvertedStyle(true);
    combinedIndicator->setDrawArc(true);
    combinedIndicator->repaint();
}

void FancyTimer::onIdleStateEnteredHook()
{
    ui->cbxSubmissionCandidate->hide();
    ui->pbCancel->hide();
    ui->pbZone->hide();
    combinedIndicator->setText("Start");
    combinedIndicator->setInvertedStyle(false);
    combinedIndicator->setDrawArc(false);
    combinedIndicator->repaint();
}

void FancyTimer::onZoneStateEnteredHook()
{
    ui->pbCancel->hide();
    combinedIndicator->setColor(zoneStateColor);
    combinedIndicator->repaint();
}

void FancyTimer::onZoneStateLeftHook()
{
    ui->pbCancel->show();
    combinedIndicator->setColor(taskStateColor);
    combinedIndicator->repaint();
}

void FancyTimer::updateIndication(std::chrono::seconds timeLeft)
{
    if (indicationUpdateShouldBeIgnored())
        return;
    combinedIndicator->setCurrentValue(static_cast<int>(timeLeft.count()));
    combinedIndicator->setText(timerValueToText(timeLeft));
    combinedIndicator->repaint();
}

bool FancyTimer::indicationUpdateShouldBeIgnored() const
{
    return currentState == IStatefulTimer::StateId::IdleEntered
        || currentState == IStatefulTimer::StateId::SprintFinished;
}

void FancyTimer::onIndicatorClicked()
{
    switch (currentState) {
    case IStatefulTimer::StateId::SprintEntered:
        break;
    case IStatefulTimer::StateId::SprintLeft:
        break;
    case IStatefulTimer::StateId::SprintCancelled:
        break;
    case IStatefulTimer::StateId::SprintFinished:
        if (ui->cbxSubmissionCandidate->currentIndex() != -1)
            requestSubmission();
        break;
    case IStatefulTimer::StateId::IdleEntered:
        startTask();
        break;
    case IStatefulTimer::StateId::IdleLeft:
        break;
    case IStatefulTimer::StateId::BreakEntered:
        break;
    case IStatefulTimer::StateId::BreakLeft:
        break;
    case IStatefulTimer::StateId::BreakCancelled:
        break;
    case IStatefulTimer::StateId::ZoneEntered:
        break;
    case IStatefulTimer::StateId::ZoneLeft:
        break;
    }
}

} // namespace sprint_timer::ui::qt_gui
