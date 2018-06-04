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
#include "DateRangePickDialog.h"
#include "ui_date_pick_dialog.h"


DateRangePickDialog::DateRangePickDialog(DateInterval initialPeriod, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DateRangePickDialog)
{
    ui->setupUi(this);
    updateCalendarDates(initialPeriod);
    configureCalendar();
    // NOTE see ui file for used signal connections
}

DateRangePickDialog::~DateRangePickDialog() { delete ui; }

void DateRangePickDialog::configureCalendar()
{
    ui->cwStart->setMaximumDate(QDate::currentDate());
    ui->cwEnd->setMaximumDate(QDate::currentDate());

    // set first day of week TODO (make this configurable with global settings)
    ui->cwStart->setFirstDayOfWeek(Qt::Monday);
    ui->cwEnd->setFirstDayOfWeek(Qt::Monday);
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
