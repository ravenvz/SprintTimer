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
#include "qt_gui/dialogs/WorkdaysDialog.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include "ui_workdays_dialog.h"
#include <core/use_cases/ChangeWorkingDays.h>

namespace {

void addExtraDays(
    QAbstractItemModel& model,
    const sprint_timer::ui::qt_gui::AddExceptionalDayDialog& pickDateDialog);

std::vector<QDate> expand(const QDate& startDate, int numDays);

} // namespace


namespace sprint_timer::ui::qt_gui {

WorkdaysDialog::WorkdaysDialog(AddExceptionalDayDialog& addExcDayDialog_,
                               QAbstractItemModel& exceptionalDaysModel_,
                               WorkdayTrackerModel& workdaysModel_,
                               QDialog* parent_)
    : QDialog{parent_}
    , ui{std::make_unique<Ui::WorkdaysDialog>()}
    , pickDateDialog{addExcDayDialog_}
    , exceptionalDaysModel{exceptionalDaysModel_}
    , workdaysModel{workdaysModel_}
{
    ui->setupUi(this);
    ui->listViewExceptionalDays->setModel(&exceptionalDaysModel);

    connect(&workdaysModel,
            &WorkdayTrackerModel::workdaysChanged,
            [this](const WorkdayTracker& updatedTracker) {
                onWorkdayTrackerChanged(updatedTracker);
            });
    connect(ui->btnAddExceptionalDay, &QPushButton::clicked, [&]() {
        pickDateDialog.exec();
    });
    connect(&pickDateDialog,
            &QDialog::accepted,
            this,
            &WorkdaysDialog::addExceptionalDay);
}

WorkdaysDialog::~WorkdaysDialog() = default;

void WorkdaysDialog::accept()
{
    std::vector<WorkdayTracker::DayData> exceptionalDays;
    exceptionalDays.reserve(exceptionalDaysModel.rowCount());

    for (int row = 0; row < exceptionalDaysModel.rowCount(QModelIndex{});
         ++row) {
        const auto index = exceptionalDaysModel.index(row, 0);
        const auto& [date, goal]
            = exceptionalDaysModel.data(index, Qt::EditRole)
                  .value<QPair<QDate, int>>();
        exceptionalDays.push_back({DateTimeConverter::date(date), goal});
    }

    workdaysModel.changeWorkdayData(pollSchedule(), exceptionalDays);
    QDialog::accept();
}

void WorkdaysDialog::onWorkdayTrackerChanged(
    const WorkdayTracker& updatedTracker)
{
    using sprint_timer::ui::qt_gui::DateTimeConverter;
    const auto days = updatedTracker.exceptionalDays();
    exceptionalDaysModel.removeRows(0, exceptionalDaysModel.rowCount());
    exceptionalDaysModel.insertRows(0, days.size());
    for (size_t row = 0; row < days.size(); ++row) {
        const auto& [date, goal] = days[row];
        QVariant entry;
        entry.setValue(QPair{DateTimeConverter::qDate(date), goal});
        exceptionalDaysModel.setData(exceptionalDaysModel.index(row, 0), entry);
    }
    exceptionalDaysModel.sort(0, Qt::AscendingOrder);
    initializeDayBoxes(updatedTracker.currentSchedule());
}

void WorkdaysDialog::initializeDayBoxes(const WeekSchedule& schedule)
{
    using namespace dw;
    ui->mondayBox->setValue(schedule.targetGoal(Weekday::Monday));
    ui->tuesdayBox->setValue(schedule.targetGoal(Weekday::Tuesday));
    ui->wednesdayBox->setValue(schedule.targetGoal(Weekday::Wednesday));
    ui->thursdayBox->setValue(schedule.targetGoal(Weekday::Thursday));
    ui->fridayBox->setValue(schedule.targetGoal(Weekday::Friday));
    ui->saturdayBox->setValue(schedule.targetGoal(Weekday::Saturday));
    ui->sundayBox->setValue(schedule.targetGoal(Weekday::Sunday));
}

WeekSchedule WorkdaysDialog::pollSchedule() const
{
    using namespace dw;

    WeekSchedule schedule;

    schedule.setTargetGoal(Weekday::Monday, ui->mondayBox->value());
    schedule.setTargetGoal(Weekday::Tuesday, ui->tuesdayBox->value());
    schedule.setTargetGoal(Weekday::Wednesday, ui->wednesdayBox->value());
    schedule.setTargetGoal(Weekday::Thursday, ui->thursdayBox->value());
    schedule.setTargetGoal(Weekday::Friday, ui->fridayBox->value());
    schedule.setTargetGoal(Weekday::Saturday, ui->saturdayBox->value());
    schedule.setTargetGoal(Weekday::Sunday, ui->sundayBox->value());

    return schedule;
}

void WorkdaysDialog::addExceptionalDay()
{
    addExtraDays(exceptionalDaysModel, pickDateDialog);
}

} // namespace sprint_timer::ui::qt_gui


namespace {

void addExtraDays(
    QAbstractItemModel& model,
    const sprint_timer::ui::qt_gui::AddExceptionalDayDialog& pickDateDialog)
{
    const std::vector days{
        expand(pickDateDialog.startDate(), pickDateDialog.numDays())};
    const int goal{pickDateDialog.targetGoal()};
    int startingRow = model.rowCount(QModelIndex{});
    model.insertRows(startingRow, days.size(), QModelIndex{});
    for (size_t i = 0; i < days.size(); ++i) {
        QPair<QDate, int> entry{days[i], goal};
        QVariant ventry;
        ventry.setValue(entry);
        model.setData(model.index(startingRow + i, 0), ventry, Qt::EditRole);
    }
}

std::vector<QDate> expand(const QDate& startDate, int numDays)
{
    std::vector<QDate> days;
    days.reserve(numDays);
    auto consecutive_dates
        = [n = 0, &date = startDate]() mutable { return date.addDays(n++); };
    std::generate_n(std::back_inserter(days), numDays, consecutive_dates);
    return days;
}

} // namespace
