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

#include "qt_gui/dialogs/AddExceptionalDayDialog.h"
#include "ui_add_exceptional_day_dialog.h"

namespace sprint_timer::ui::qt_gui {

AddExceptionalDayDialog::AddExceptionalDayDialog(QDialog* parent)
    : QDialog{parent}
    , ui{std::make_unique<Ui::AddExceptionalDayDialog>()}
{
    ui->setupUi(this);
    startDate_ = ui->calendarWidget->selectedDate();
    ui->lblDatePlaceholder->setText(startDate_.toString());
    connect(ui->calendarWidget, &QCalendarWidget::selectionChanged, [this]() {
        startDate_ = ui->calendarWidget->selectedDate();
        ui->lblDatePlaceholder->setText(startDate_.toString());
    });
    connect(ui->spbNumDays,
            QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int newValue) { numDays_ = newValue; });
}

AddExceptionalDayDialog::~AddExceptionalDayDialog() = default;

void AddExceptionalDayDialog::accept()
{
    QDialog::accept();
    ui->spbNumDays->setValue(1);
}

QDate AddExceptionalDayDialog::startDate() const { return startDate_; }

int AddExceptionalDayDialog::numDays() const { return numDays_; }

void AddExceptionalDayDialog::setStartDate(const QDate& date)
{
    startDate_ = date;
    ui->lblDatePlaceholder->setText(startDate_.toString());
}

void AddExceptionalDayDialog::setNumDays(int numDays) { numDays_ = numDays; }

} // namespace sprint_timer::ui::qt_gui

