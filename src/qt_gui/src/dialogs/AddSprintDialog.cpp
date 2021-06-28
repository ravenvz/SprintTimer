/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include "qt_gui/dialogs/AddSprintDialog.h"
#include "qt_gui/models/CustomRoles.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include "ui_add_sprint_dialog.h"
#include <QCalendarWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QSignalBlocker>

namespace {

void shiftDate(QTimeEdit* timeEdit, const QDate& date);

} // namespace

namespace sprint_timer::ui::qt_gui {

AddSprintDialog::AddSprintDialog(
    contracts::RegisterSprintControl::Presenter& presenter_,
    QAbstractItemModel& taskModel_,
    dw::Weekday firstDayOfWeek_,
    std::chrono::minutes sprintDuration_)
    : sprintDuration{sprintDuration_}
{
    auto layout = std::make_unique<QFormLayout>();
    auto datePicker = std::make_unique<QCalendarWidget>();
    auto taskSelector = std::make_unique<QComboBox>();
    auto buttons = std::make_unique<QDialogButtonBox>(QDialogButtonBox::Ok |
                                                      QDialogButtonBox::Cancel);

    datePicker->setFirstDayOfWeek(
        firstDayOfWeek_ == dw::Weekday::Monday ? Qt::Monday : Qt::Sunday);
    datePicker->setMaximumDate(QDate::currentDate());
    sprintNumber->setMinimum(1);
    startTime->setDisplayFormat("hh:mm");
    finishTime->setDisplayFormat("hh:mm");
    finishTime->setDateTime(QDateTime::currentDateTime());

    connect(buttons.get(), &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons.get(), &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(startTime,
            &QTimeEdit::dateTimeChanged,
            this,
            &AddSprintDialog::adjustFinishTime);
    connect(finishTime,
            &QTimeEdit::dateTimeChanged,
            this,
            &AddSprintDialog::adjustStartTime);
    connect(sprintNumber,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &AddSprintDialog::adjustTime);
    connect(
        datePicker.get(), &QCalendarWidget::clicked, [this](const QDate& date) {
            shiftDate(startTime, date);
            shiftDate(finishTime, date);
        });
    connect(this,
            &QDialog::accepted,
            [this, &taskModel_, selector = taskSelector.get(), &presenter_]() {
                const auto taskUuid =
                    taskModel_
                        .data(taskModel_.index(selector->currentIndex(), 0),
                              CustomRoles::IdRole)
                        .toString()
                        .toStdString();
                const auto firstSprintStart =
                    utils::toDateTime(startTime->dateTime());
                const auto numSprints = sprintNumber->value();
                presenter_.registerConsecutiveSprints(
                    taskUuid, firstSprintStart, numSprints, sprintDuration);
            });

    taskSelector->setModel(&taskModel_);
    taskSelector->setItemDelegate(&delegate);

    layout->addRow("Task:", taskSelector.release());
    layout->addRow("Date:", datePicker.release());
    layout->addRow("Start time:", startTime);
    layout->addRow("Finish time:", finishTime);
    layout->addRow("Number of sprints:", sprintNumber);
    layout->addRow(buttons.release());

    setLayout(layout.release());

    adjustStartTime();
}

AddSprintDialog::~AddSprintDialog() = default;

void AddSprintDialog::adjustFinishTime()
{
    lastChangedTime = startTime;
    const QSignalBlocker blocker{finishTime};
    finishTime->setDateTime(
        startTime->dateTime().addSecs(totalSprintTime().count()));
}

void AddSprintDialog::adjustStartTime()
{
    lastChangedTime = finishTime;
    const QSignalBlocker blocker{startTime};
    startTime->setDateTime(
        finishTime->dateTime().addSecs(-totalSprintTime().count()));
}

void AddSprintDialog::adjustTime()
{
    lastChangedTime == startTime ? adjustFinishTime() : adjustStartTime();
}

std::chrono::seconds AddSprintDialog::totalSprintTime() const
{
    return sprintNumber->value() * sprintDuration;
}

} // namespace sprint_timer::ui::qt_gui

namespace {

void shiftDate(QTimeEdit* timeEdit, const QDate& date)
{
    const QSignalBlocker blocker{timeEdit};
    const auto time = timeEdit->time();
    timeEdit->setDate(date);
    timeEdit->setTime(time);
}

} // namespace
