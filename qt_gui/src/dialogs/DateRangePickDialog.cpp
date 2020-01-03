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
#include "qt_gui/utils/DateTimeConverter.h"
#include "ui_date_pick_dialog.h"

namespace sprint_timer::ui::qt_gui {

DateRangePickDialog::DateRangePickDialog(const IConfig& applicationSettings_,
                                         QWidget* parent_)
    : QDialog{parent_}
    , ui{std::make_unique<Ui::DateRangePickDialog>()}
    , applicationSettings{applicationSettings_}
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
}

DateRangePickDialog::~DateRangePickDialog() = default;

dw::DateRange DateRangePickDialog::selectedRange()
{
    return dw::DateRange{utils::toDate(ui->cwStart->selectedDate()),
                         utils::toDate(ui->cwEnd->selectedDate())};
}

void DateRangePickDialog::setSelectionRange(const dw::DateRange& dateRange)
{
    ui->dePickStartDate->setDate(utils::toQDate(dateRange.start()));
    ui->dePickEndDate->setDate(utils::toQDate(dateRange.finish()));
}

void DateRangePickDialog::showEvent(QShowEvent* event)
{
    if (applicationSettings.firstDayOfWeek() == dw::Weekday::Monday) {
        ui->cwStart->setFirstDayOfWeek(Qt::Monday);
        ui->cwEnd->setFirstDayOfWeek(Qt::Monday);
    }
    else {
        ui->cwStart->setFirstDayOfWeek(Qt::Sunday);
        ui->cwEnd->setFirstDayOfWeek(Qt::Sunday);
    }
    QDialog::showEvent(event);
}

} // namespace sprint_timer::ui::qt_gui
