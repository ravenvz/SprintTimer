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
#include "qt_gui/dialogs/DateRangePickDialog.h"
#include "ui_date_pick_dialog.h"

namespace sprint_timer::ui::qt_gui {

DateRangePickDialog::DateRangePickDialog(
    Qt::DayOfWeek firstDayOfWeek_,
    const std::pair<QDate, QDate>& dateRange_,
    QWidget* parent_)
    : QDialog{parent_}
    , ui{std::make_unique<Ui::DateRangePickDialog>()}
{
    ui->setupUi(this);
    ui->cwStart->setMaximumDate(QDate::currentDate());
    ui->cwEnd->setMaximumDate(QDate::currentDate());

    // NOTE also see ui file for used signal connections
    connect(ui->cwStart,
            &QCalendarWidget::clicked,
            ui->cwEnd,
            &QCalendarWidget::setMinimumDate);
    connect(ui->cwEnd,
            &QCalendarWidget::clicked,
            ui->cwStart,
            &QCalendarWidget::setMaximumDate);

    ui->cwStart->setFirstDayOfWeek(firstDayOfWeek_);
    ui->cwEnd->setFirstDayOfWeek(firstDayOfWeek_);

    ui->dePickStartDate->setDate(dateRange_.first);
    ui->dePickEndDate->setDate(dateRange_.second);
}

DateRangePickDialog::~DateRangePickDialog() = default;

auto DateRangePickDialog::selectedRange() -> std::pair<QDate, QDate>
{
    return {ui->cwStart->selectedDate(), ui->cwEnd->selectedDate()};
}

} // namespace sprint_timer::ui::qt_gui
