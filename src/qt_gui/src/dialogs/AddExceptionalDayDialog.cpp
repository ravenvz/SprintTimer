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

#include "qt_gui/dialogs/AddExceptionalDayDialog.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include <QCalendarWidget>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>

namespace sprint_timer::ui::qt_gui {

AddExceptionalDayDialog::AddExceptionalDayDialog(dw::Weekday firstDayOfWeek_,
                                                 dw::Date preselectedDate_,
                                                 OutputData& data_,
                                                 QDialog* parent_)
    : QDialog{parent_}
{
    auto layout = std::make_unique<QFormLayout>();
    auto calendar = std::make_unique<QCalendarWidget>();
    auto dateHint = std::make_unique<QLabel>();
    auto numSprintsField = std::make_unique<QSpinBox>();
    auto daysDurationField = std::make_unique<QSpinBox>();
    auto buttons = std::make_unique<QDialogButtonBox>(QDialogButtonBox::Ok |
                                                      QDialogButtonBox::Cancel);

    calendar->setFirstDayOfWeek(
        firstDayOfWeek_ == dw::Weekday::Monday ? Qt::Monday : Qt::Sunday);
    calendar->setSelectedDate(utils::toQDate(preselectedDate_));

    daysDurationField->setValue(1);

    connect(buttons.get(), &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons.get(), &QDialogButtonBox::rejected, this, &QDialog::reject);
    dateHint->setText(calendar->selectedDate().toString());
    connect(calendar.get(),
            &QCalendarWidget::selectionChanged,
            [calWidg = calendar.get(), dateHintWidg = dateHint.get()]() {
                dateHintWidg->setText(calWidg->selectedDate().toString());
            });
    connect(this,
            &QDialog::accepted,
            [&data_,
             numDays = daysDurationField.get(),
             numSprints = numSprintsField.get(),
             cal = calendar.get()]() {
                data_.numDays = numDays->value();
                data_.sprintsPerDay = numSprints->value();
                data_.startDate = utils::toDate(cal->selectedDate());
            });

    layout->addRow(calendar.release());
    layout->addRow("StartDate:", dateHint.release());
    layout->addRow("Number of days:", daysDurationField.release());
    layout->addRow("Number of sprints:", numSprintsField.release());
    layout->addRow(buttons.release());

    setLayout(layout.release());
}

} // namespace sprint_timer::ui::qt_gui

