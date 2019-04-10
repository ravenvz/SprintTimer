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
#include "qt_gui/dialogs/DateRangePickDialog.h"
#include "ui_date_pick_dialog.h"

namespace sprint_timer::ui::qt_gui {


DateRangePickDialog::DateRangePickDialog(DateInterval initialPeriod,
                                         FirstDayOfWeek firstDayOfWeek,
                                         QWidget* parent)
    : QDialog{parent}
    , ui{std::make_unique<Ui::DateRangePickDialog>()}
{
    ui->setupUi(this);
    updateCalendarDates(initialPeriod);
    configureCalendar(firstDayOfWeek);
    // NOTE see ui file for used signal connections
}

DateRangePickDialog::~DateRangePickDialog() = default;

void DateRangePickDialog::configureCalendar(FirstDayOfWeek firstDayOfWeek)
{
    ui->cwStart->setMaximumDate(QDate::currentDate());
    ui->cwEnd->setMaximumDate(QDate::currentDate());

    if (firstDayOfWeek == FirstDayOfWeek::Monday) {
        ui->cwStart->setFirstDayOfWeek(Qt::Monday);
        ui->cwEnd->setFirstDayOfWeek(Qt::Monday);
    }
}

void DateRangePickDialog::updateCalendarDates(DateInterval& period)
{
    ui->dePickStartDate->setDate(period.startDate);
    ui->dePickEndDate->setDate(period.endDate);
}

DateInterval DateRangePickDialog::getNewInterval()
{
    return DateInterval{ui->cwStart->selectedDate(), ui->cwEnd->selectedDate()};
}

} // namespace sprint_timer::ui::qt_gui
