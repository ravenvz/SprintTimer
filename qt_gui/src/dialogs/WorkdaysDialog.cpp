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

#include "WorkdaysDialog.h"
#include "core/utils/WeekdaySelection.h"
#include "ui_workdays_dialog.h"

WorkdaysDialog::WorkdaysDialog(IConfig& applicationSettings, QDialog* parent)
    : QDialog{parent}
    , ui{new Ui::WorkdaysDialog}
    , settings{applicationSettings}
{
    ui->setupUi(this);
    initializeDayBoxes();
}

WorkdaysDialog::~WorkdaysDialog() { delete ui; }

void WorkdaysDialog::accept()
{
    settings.setWorkdaysCode(pollWorkdaysCode());
    QDialog::accept();
}

void WorkdaysDialog::initializeDayBoxes()
{
    using dw::DateTime;
    const WeekdaySelection selection{settings.workdaysCode()};
    ui->mondayBox->setChecked(selection.isSelected(DateTime::Weekday::Monday));
    ui->tuesdayBox->setChecked(
        selection.isSelected(DateTime::Weekday::Tuesday));
    ui->wednesdayBox->setChecked(
        selection.isSelected(DateTime::Weekday::Wednesday));
    ui->thursdayBox->setChecked(
        selection.isSelected(DateTime::Weekday::Thursday));
    ui->fridayBox->setChecked(selection.isSelected(DateTime::Weekday::Friday));
    ui->saturdayBox->setChecked(
        selection.isSelected(DateTime::Weekday::Saturday));
    ui->sundayBox->setChecked(selection.isSelected(DateTime::Weekday::Sunday));
}

unsigned WorkdaysDialog::pollWorkdaysCode() const
{
    using dw::DateTime;

    WeekdaySelection selection;

    if (ui->mondayBox->isChecked())
        selection.selectDay(DateTime::Weekday::Monday);
    if (ui->tuesdayBox->isChecked())
        selection.selectDay(DateTime::Weekday::Tuesday);
    if (ui->wednesdayBox->isChecked())
        selection.selectDay(DateTime::Weekday::Wednesday);
    if (ui->thursdayBox->isChecked())
        selection.selectDay(DateTime::Weekday::Thursday);
    if (ui->fridayBox->isChecked())
        selection.selectDay(DateTime::Weekday::Friday);
    if (ui->saturdayBox->isChecked())
        selection.selectDay(DateTime::Weekday::Saturday);
    if (ui->sundayBox->isChecked())
        selection.selectDay(DateTime::Weekday::Sunday);

    return selection.selectionMask();
}
