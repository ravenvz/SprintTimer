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
#include "qt_gui/widgets/FancyTimer.h"
#include "qt_gui/utils/WidgetUtils.h"
#include <QGridLayout>
#include <QPushButton>

#include <QDebug>

namespace {

const QColor taskStateColor{"#eb6c59"};
const QColor breakStateColor{"#73c245"};
const QColor zoneStateColor{Qt::darkYellow};

} // namespace

namespace sprint_timer::ui::qt_gui {

FancyTimer::FancyTimer(const IConfig& applicationSettings_,
                       QAbstractItemModel& sprintModel_,
                       std::unique_ptr<QComboBox> submissionBox_,
                       std::unique_ptr<CombinedIndicator> combinedIndicator_,
                       SprintRegistrator& sprintRegistrator_,
                       QWidget* parent_)
    : TimerWidgetBase{applicationSettings_, sprintModel_, parent_}
    , submissionBox{submissionBox_.get()}
    , combinedIndicator{combinedIndicator_.get()}
{
    combinedIndicator_->setSizePolicy(QSizePolicy::MinimumExpanding,
                                      QSizePolicy::MinimumExpanding);

    auto pbCancel_ = std::make_unique<QPushButton>("Cancel", this);
    auto pbZone_ = std::make_unique<QPushButton>("InTheZone", this);
    pbCancel_->setFlat(true);
    pbZone_->setFlat(true);

    WidgetUtils::setRetainSizeWhenHidden(pbCancel_.get());
    WidgetUtils::setRetainSizeWhenHidden(pbZone_.get());
    WidgetUtils::setRetainSizeWhenHidden(submissionBox);

    connect(combinedIndicator_.get(),
            &CombinedIndicator::indicatorClicked,
            this,
            &FancyTimer::onIndicatorClicked);
    connect(pbZone_.get(), &QPushButton::clicked, [&]() {
        timer->toggleInTheZoneMode();
    });
    connect(
        pbCancel_.get(), &QPushButton::clicked, this, &FancyTimer::cancelTask);

    connect(this,
            &TimerWidgetBase::submitRequested,
            &sprintRegistrator_,
            &SprintRegistrator::onSubmissionRequested);

    pbCancel = pbCancel_.get();
    pbZone = pbZone_.get();

    auto layout = std::make_unique<QGridLayout>();
    layout->addWidget(combinedIndicator_.release(),
                      1,
                      0,
                      1,
                      2,
                      Qt::AlignHCenter | Qt::AlignTop);
    layout->addWidget(submissionBox_.release(), 0, 0, 1, 2, Qt::AlignHCenter);
    layout->addWidget(pbCancel_.release(), 2, 0, 1, 1, Qt::AlignLeft);
    layout->addWidget(pbZone_.release(), 2, 1, 1, 1, Qt::AlignRight);
    setLayout(layout.release());

    onIdleStateEnteredHook();
}

FancyTimer::~FancyTimer() = default;

void FancyTimer::onSprintStateEnteredHook()
{
    submissionBox->hide();
    pbCancel->show();
    pbZone->show();
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
    submissionBox->show();
    pbCancel->show();
    pbZone->hide();
    combinedIndicator->setText("Submit");
    combinedIndicator->setDrawArc(false);
    combinedIndicator->repaint();
}

void FancyTimer::onBreakStateEnteredHook()
{
    submissionBox->hide();
    pbCancel->show();
    pbZone->hide();
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
    submissionBox->hide();
    pbCancel->hide();
    pbZone->hide();
    combinedIndicator->setText("Start");
    combinedIndicator->setInvertedStyle(false);
    combinedIndicator->setDrawArc(false);
    combinedIndicator->repaint();
}

void FancyTimer::onZoneStateEnteredHook()
{
    pbCancel->hide();
    combinedIndicator->setColor(zoneStateColor);
    combinedIndicator->repaint();
}

void FancyTimer::onZoneStateLeftHook()
{
    pbCancel->show();
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
    return currentState == IStatefulTimer::StateId::IdleEntered ||
           currentState == IStatefulTimer::StateId::SprintFinished;
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
        if (submissionBox->currentIndex() != -1)
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
