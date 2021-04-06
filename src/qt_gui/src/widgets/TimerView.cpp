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
#include "qt_gui/widgets/TimerView.h"
#include "qt_gui/models/CustomRoles.h"
#include "qt_gui/utils/WidgetUtils.h"
#include <QApplication>

namespace {

QString timerValueToText(std::chrono::seconds timeLeft);

constexpr QEvent::Type uiChangeEvent =
    static_cast<QEvent::Type>(QEvent::User + 1);

constexpr QEvent::Type timerTickEvent =
    static_cast<QEvent::Type>(QEvent::User + 2);

class TimerTickEvent : public QEvent {
public:
    explicit TimerTickEvent(std::chrono::seconds timeLeft_)
        : QEvent{timerTickEvent}
        , timeLeft{timeLeft_}
    {
    }

    std::chrono::seconds timerValue() const { return timeLeft; }

private:
    std::chrono::seconds timeLeft;
};

struct UiChangedEvent : public QEvent {
    UiChangedEvent(
        sprint_timer::ui::contracts::TimerContract::TimerUiModel&& model_)
        : QEvent{uiChangeEvent}
        , model{std::move(model_)}
    {
    }

    sprint_timer::ui::contracts::TimerContract::TimerUiModel model;
};

} // namespace

namespace sprint_timer::ui::qt_gui {

TimerView::TimerView(ui::contracts::RegisterSprintControl::Presenter&
                         registerSprintControlPresenter_,
                     QAbstractItemModel& taskModel_,
                     std::unique_ptr<CombinedIndicator> combinedIndicator_,
                     QWidget* parent_)
    : QWidget{parent_}
    , registerSprintControlPresenter{registerSprintControlPresenter_}
    , taskModel{taskModel_}
    , combinedIndicator{combinedIndicator_.get()}
{
    auto pbCancel_ = std::make_unique<QPushButton>("Cancel");
    auto pbZone_ = std::make_unique<QPushButton>("InTheZone");
    auto submissionBox_ = std::make_unique<QComboBox>();
    submissionBox = submissionBox_.get();
    submissionBox->setModel(&taskModel_);
    pbCancel_->setFlat(true);
    pbZone_->setFlat(true);
    pbZone_->setCheckable(true);
    WidgetUtils::setRetainSizeWhenHidden(pbZone_.get());
    WidgetUtils::setRetainSizeWhenHidden(pbCancel_.get());
    WidgetUtils::setRetainSizeWhenHidden(submissionBox_.get());
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

    connect(combinedIndicator, &CombinedIndicator::indicatorClicked, [this]() {
        if (auto p = presenter(); p) {
            p.value()->onTimerClicked();
        }
    });
    connect(pbCancel, &QPushButton::clicked, [this]() {
        if (auto p = presenter(); p) {
            p.value()->onCancelClicked();
        }
    });
    connect(pbZone, &QPushButton::clicked, [this]() {
        if (auto p = presenter(); p) {
            p.value()->onZoneClicked();
        }
    });
    connect(submissionBox,
            QOverload<int>::of(&QComboBox::activated),
            [this, &taskModel_](int row) {
                auto uuid =
                    taskModel_
                        .data(taskModel_.index(row, 0), CustomRoles::IdRole)
                        .toString()
                        .toStdString();
                if (auto p = presenter(); p) {
                    p.value()->changeTaskSelection(static_cast<size_t>(row),
                                                   std::move(uuid));
                }
            });
}

TimerView::~TimerView() = default;

void TimerView::setupUi(contracts::TimerContract::TimerUiModel&& model)
{
    QApplication::postEvent(
        this, std::make_unique<UiChangedEvent>(std::move(model)).release());
}

void TimerView::updateTimerValue(std::chrono::seconds currentValue)
{
    QCoreApplication::postEvent(
        this, std::make_unique<TimerTickEvent>(currentValue).release());
}

void TimerView::submitSprints(
    const std::vector<dw::DateTimeRange>& timeIntervals)
{
    if (!submissionBox->isVisible()) {
        return;
    }
    const auto taskUuid =
        taskModel
            .data(taskModel.index(submissionBox->currentIndex(), 0),
                  CustomRoles::IdRole)
            .toString()
            .toStdString();
    registerSprintControlPresenter.registerSprintBulk(taskUuid, timeIntervals);
}

void TimerView::selectTask(size_t taskIndex)
{
    submissionBox->setCurrentIndex(static_cast<int>(taskIndex));
}

void TimerView::customEvent(QEvent* event)
{
    if (event->type() == timerTickEvent) {
        onUpdateTimerValue(dynamic_cast<TimerTickEvent*>(event)->timerValue());
    }
    else if (event->type() == uiChangeEvent) {
        auto* evt = dynamic_cast<UiChangedEvent*>(event);
        onUiChangeRequested(evt->model);
    }
}

void TimerView::onUpdateTimerValue(std::chrono::seconds currentValue)
{
    combinedIndicator->setCurrentValue(static_cast<int>(currentValue.count()));
    combinedIndicator->setText(timerValueToText(currentValue));
    combinedIndicator->repaint();
}

void TimerView::onUiChangeRequested(
    const contracts::TimerContract::TimerUiModel& model)
{
    auto fromStringView = [](std::string_view sv) {
        return QString::fromStdString(std::string{sv});
    };
    submissionBox->setVisible(model.submissionBoxVisible);
    pbCancel->setVisible(model.cancelControlVisible);
    pbZone->setVisible(model.zoneControlVisible);
    combinedIndicator->setInvertedStyle(model.invertedDial);
    combinedIndicator->setDrawArc(model.dialVisible);
    if (auto color = model.dialColor; color) {
        combinedIndicator->setColor(fromStringView(*color));
    }
    if (auto timerText = model.centerText; timerText) {
        combinedIndicator->setText(fromStringView(*timerText));
    }
    if (auto totalDuration = model.timerTotalDuration; totalDuration) {
        combinedIndicator->setMaxValue(
            static_cast<int>(totalDuration->count()));
        combinedIndicator->setCurrentValue(
            static_cast<int>(totalDuration->count()));
        combinedIndicator->setText(timerValueToText(*totalDuration));
    }
    combinedIndicator->repaint();
}

} // namespace sprint_timer::ui::qt_gui

namespace {

QString timerValueToText(std::chrono::seconds timeLeft)
{
    using namespace std::chrono;
    minutes min = duration_cast<minutes>(timeLeft);
    seconds sec = timeLeft - min;
    return QString("%1:%2").arg(
        QString::number(min.count()),
        QString::number(sec.count()).rightJustified(2, '0'));
}

} // namespace
