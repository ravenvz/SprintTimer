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
#include "qt_gui/dialogs/AddSprintDialog.h"
#include "qt_gui/models/TaskModelRoles.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include "ui_add_sprint_dialog.h"
#include <QCalendarWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QSignalBlocker>

namespace {

void shiftDate(QTimeEdit* timeEdit, const QDate& date);

std::chrono::seconds totalSprintLength(int numSprints,
                                       std::chrono::seconds sprintDuration);

} // namespace

namespace sprint_timer::ui::qt_gui {

AddSprintDialog::AddSprintDialog(const std::vector<entities::Task>& /*tasks_*/,
                                 std::vector<entities::Sprint>& addedSprints_,
                                 dw::Weekday firstDayOfWeek_,
                                 std::chrono::minutes sprintDuration_,
                                 QDialog* parent_)
    : QDialog{parent_}
    , sprintDuration{sprintDuration_}
{
    auto layout = std::make_unique<QFormLayout>();
    auto taskSelector = std::make_unique<QComboBox>();
    auto datePicker = std::make_unique<QCalendarWidget>();

    datePicker->setFirstDayOfWeek(
        firstDayOfWeek_ == dw::Weekday::Monday ? Qt::Monday : Qt::Sunday);
    datePicker->setMaximumDate(QDate::currentDate());
    sprintNumber->setMinimum(1);
    startTime->setDisplayFormat("hh:mm");
    finishTime->setDisplayFormat("hh:mm");

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

    layout->addRow("Task:", taskSelector.release());
    layout->addRow("Date:", datePicker.release());
    layout->addRow("Start time:", startTime);
    layout->addRow("Finish time:", finishTime);
    layout->addRow("Number of sprints:", sprintNumber);

    adjustFinishTime(startTime->dateTime());

    setLayout(layout.release());
}

AddSprintDialog::~AddSprintDialog() = default;

void AddSprintDialog::adjustFinishTime(const QDateTime& dateTime)
{
    lastChangedTime = startTime;
    const QSignalBlocker blocker{finishTime};
    finishTime->setDateTime(dateTime.addSecs(
        totalSprintLength(sprintNumber->value(), sprintDuration).count()));
}

void AddSprintDialog::adjustStartTime(const QDateTime& dateTime)
{
    lastChangedTime = finishTime;
    const QSignalBlocker blocker{startTime};
    startTime->setDateTime(dateTime.addSecs(
        -totalSprintLength(sprintNumber->value(), sprintDuration).count()));
}

void AddSprintDialog::adjustTime()
{
    if (lastChangedTime == startTime) {
        adjustFinishTime(startTime->dateTime());
    }
    else {
        adjustStartTime(finishTime->dateTime());
    }
}

void AddSprintDialog::accept() { }

} // namespace sprint_timer::ui::qt_gui

namespace {

void shiftDate(QTimeEdit* timeEdit, const QDate& date)
{
    const QSignalBlocker blocker{timeEdit};
    const auto time = timeEdit->time();
    timeEdit->setDate(date);
    timeEdit->setTime(time);
}

std::chrono::seconds totalSprintLength(int numSprints,
                                       std::chrono::seconds sprintDuration)
{
    return numSprints * sprintDuration;
}

} // namespace
