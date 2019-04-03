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
#include "qt_gui/utils/DateTimeConverter.h"
#include "ui_workdays_dialog.h"
#include <core/use_cases/AddExtraHolidays.h>
#include <core/use_cases/AddExtraWorkdays.h>
#include <core/use_cases/RemoveExtraHolidays.h>
#include <core/use_cases/RemoveExtraWorkdays.h>
#include <core/utils/WeekdaySelection.h>

#include <iostream>

namespace {

std::vector<dw::Date> expand(const dw::Date& startDate, int numDays)
{
    std::vector<dw::Date> days;
    days.reserve(numDays);
    auto consecutive_dates
        = [n = 0, &date = startDate]() mutable { return date + dw::Days{n++}; };
    std::generate_n(std::back_inserter(days), numDays, consecutive_dates);
    return days;
}

} // namespace

namespace sprint_timer::ui::qt_gui {

WorkdaysDialog::WorkdaysDialog(AddExceptionalDayDialog& addExcDayDialog_,
                               CommandInvoker& commandInvoker_,
                               IWorkingDaysWriter& workingDaysWriter_,
                               ExtraDayModel& holidayModel_,
                               ExtraDayModel& workdayModel_,
                               QDialog* parent_)
    : QDialog{parent_}
    , ui{std::make_unique<Ui::WorkdaysDialog>()}
    , pickDateDialog{addExcDayDialog_}
    , commandInvoker{commandInvoker_}
    , workingDaysWriter{workingDaysWriter_}
    , holidayModel{holidayModel_}
    , workdayModel{workdayModel_}
{
    ui->setupUi(this);
    ui->lvHolidays->setModel(&holidayModel);
    ui->lvWorkdays->setModel(&workdayModel);
    connect(ui->btnAddHolidays, &QPushButton::clicked, [&]() {
        connect(&pickDateDialog,
                &QDialog::accepted,
                this,
                &WorkdaysDialog::addHolidays);
        pickDateDialog.exec();
        disconnect(&pickDateDialog,
                   &QDialog::accepted,
                   this,
                   &WorkdaysDialog::addHolidays);
    });
    connect(ui->btnAddWorkdays, &QPushButton::clicked, [&]() {
        connect(&pickDateDialog,
                &QDialog::accepted,
                this,
                &WorkdaysDialog::addWorkdays);
        pickDateDialog.exec();
        disconnect(&pickDateDialog,
                   &QDialog::accepted,
                   this,
                   &WorkdaysDialog::addWorkdays);
    });
    connect(&holidayModel,
            &QAbstractListModel::rowsAboutToBeRemoved,
            this,
            &WorkdaysDialog::onHolidayRemoveRequested);
    connect(&workdayModel,
            &QAbstractListModel::rowsAboutToBeRemoved,
            this,
            &WorkdaysDialog::onWorkdayRemoveRequested);
    initializeDayBoxes();
}

WorkdaysDialog::~WorkdaysDialog() = default;

void WorkdaysDialog::accept()
{
    // tracker = WorkdayTracker{pollWorkdaysCode()};
    QDialog::accept();
}

void WorkdaysDialog::setWorkdayTracker(const WorkdayTracker& workdayTracker)
{
    tracker = workdayTracker;
    holidayModel.setExtraDayData(tracker.extraHolidays());
    workdayModel.setExtraDayData(tracker.extraWorkdays());
    initializeDayBoxes();
}

WorkdayTracker WorkdaysDialog::workdayTracker() const { return tracker; }

void WorkdaysDialog::initializeDayBoxes()
{
    using dw::DateTime;
    ui->mondayBox->setChecked(tracker.isWorkday(dw::Weekday::Monday));
    ui->tuesdayBox->setChecked(tracker.isWorkday(dw::Weekday::Tuesday));
    ui->wednesdayBox->setChecked(tracker.isWorkday(dw::Weekday::Wednesday));
    ui->thursdayBox->setChecked(tracker.isWorkday(dw::Weekday::Thursday));
    ui->fridayBox->setChecked(tracker.isWorkday(dw::Weekday::Friday));
    ui->saturdayBox->setChecked(tracker.isWorkday(dw::Weekday::Saturday));
    ui->sundayBox->setChecked(tracker.isWorkday(dw::Weekday::Sunday));
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

void WorkdaysDialog::addHolidays()
{
    using sprint_timer::ui::qt_gui::DateTimeConverter;
    using sprint_timer::use_cases::AddExtraHolidays;
    const dw::Date date{DateTimeConverter::date(pickDateDialog.startDate())};
    const std::vector<dw::Date> days{expand(date, pickDateDialog.numDays())};
    commandInvoker.executeCommand(
        std::make_unique<AddExtraHolidays>(workingDaysWriter, days));
    for (const auto& day : days)
        holidayModel.insert(day);
}

void WorkdaysDialog::addWorkdays()
{
    using sprint_timer::ui::qt_gui::DateTimeConverter;
    using sprint_timer::use_cases::AddExtraWorkdays;
    const dw::Date date{DateTimeConverter::date(pickDateDialog.startDate())};
    const std::vector<dw::Date> days{expand(date, pickDateDialog.numDays())};
    commandInvoker.executeCommand(
        std::make_unique<AddExtraWorkdays>(workingDaysWriter, days));
    for (const auto& day : days)
        workdayModel.insert(day);
}

void WorkdaysDialog::onHolidayRemoveRequested(const QModelIndex& parent,
                                              int row,
                                              int count)
{
    using sprint_timer::use_cases::RemoveExtraHolidays;
    // TODO now it is not used, but probably should handle all rows mentioned by
    // the signal
    commandInvoker.executeCommand(std::make_unique<RemoveExtraHolidays>(
        workingDaysWriter, std::vector<dw::Date>{holidayModel.dateAt(row)}));
}

void WorkdaysDialog::onWorkdayRemoveRequested(const QModelIndex& parent,
                                              int row,
                                              int count)
{
    // TODO now it is not used, but probably should handle all rows mentioned by
    // the signal
    using sprint_timer::use_cases::RemoveExtraWorkdays;
    commandInvoker.executeCommand(std::make_unique<RemoveExtraWorkdays>(
        workingDaysWriter, std::vector<dw::Date>{workdayModel.dateAt(row)}));
}

} // namespace sprint_timer::ui::qt_gui
