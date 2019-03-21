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
#include "qt_gui/dialogs/WorkdaysDialog.h"
#include "ui_workdays_dialog.h"
#include <core/utils/WeekdaySelection.h>

namespace sprint_timer::ui::qt_gui {

WorkdaysDialog::WorkdaysDialog(IConfig& applicationSettings, QDialog* parent)
    : QDialog{parent}
    , ui{std::make_unique<Ui::WorkdaysDialog>()}
    , settings{applicationSettings}
{
    ui->setupUi(this);
    initializeDayBoxes();
}

WorkdaysDialog::~WorkdaysDialog() = default;

void WorkdaysDialog::accept()
{
    settings.setWorkdays(pollWorkdaysCode());
    QDialog::accept();
}

void WorkdaysDialog::initializeDayBoxes()
{
    using dw::DateTime;
    const utils::WeekdaySelection selection{settings.workdays()};
    ui->mondayBox->setChecked(selection.isSelected(dw::Weekday::Monday));
    ui->tuesdayBox->setChecked(selection.isSelected(dw::Weekday::Tuesday));
    ui->wednesdayBox->setChecked(selection.isSelected(dw::Weekday::Wednesday));
    ui->thursdayBox->setChecked(selection.isSelected(dw::Weekday::Thursday));
    ui->fridayBox->setChecked(selection.isSelected(dw::Weekday::Friday));
    ui->saturdayBox->setChecked(selection.isSelected(dw::Weekday::Saturday));
    ui->sundayBox->setChecked(selection.isSelected(dw::Weekday::Sunday));
}

utils::WeekdaySelection WorkdaysDialog::pollWorkdaysCode() const
{
    using dw::DateTime;

    utils::WeekdaySelection selection;

    if (ui->mondayBox->isChecked())
        selection.selectDay(dw::Weekday::Monday);
    if (ui->tuesdayBox->isChecked())
        selection.selectDay(dw::Weekday::Tuesday);
    if (ui->wednesdayBox->isChecked())
        selection.selectDay(dw::Weekday::Wednesday);
    if (ui->thursdayBox->isChecked())
        selection.selectDay(dw::Weekday::Thursday);
    if (ui->fridayBox->isChecked())
        selection.selectDay(dw::Weekday::Friday);
    if (ui->saturdayBox->isChecked())
        selection.selectDay(dw::Weekday::Saturday);
    if (ui->sundayBox->isChecked())
        selection.selectDay(dw::Weekday::Sunday);

    return selection;
}

} // namespace sprint_timer::ui::qt_gui
