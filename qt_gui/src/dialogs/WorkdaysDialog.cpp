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
#include <QAbstractItemModel>

#include <iostream>

Q_DECLARE_METATYPE(sprint_timer::WeekSchedule)

namespace {

std::vector<QDate> expand(const QDate& startDate, int numDays);

/* Remove all rows from given model and fill it with data.
 *
 * Despite being template, it is not generic function; just a convinience one,
 * as models used in WorkdaysDialog do have same structure. In particular, it
 * expects that template parameter has structure T = QPair<QDate, A> where A is
 * any type registered withing Qt metatype system. */
template <typename T>
void replaceModelContent(QAbstractItemModel& model, const std::vector<T>& data);

} // namespace


namespace sprint_timer::ui::qt_gui {

WorkdaysDialog::WorkdaysDialog(AddExceptionalDayDialog& addExcDayDialog_,
                               QAbstractItemModel& exceptionalDaysModel_,
                               WorkdayTrackerModel& workdaysModel_,
                               QAbstractItemModel& scheduleModel_,
                               QDialog* parent_)
    : QDialog{parent_}
    , ui{std::make_unique<Ui::WorkdaysDialog>()}
    , pickDateDialog{addExcDayDialog_}
    , workdaysModel{workdaysModel_}
{
    ui->setupUi(this);
    ui->listViewExceptionalDays->setModel(&exceptionalDaysModel_);
    ui->listViewSchedules->setModel(&scheduleModel_);

    ui->dateEditScheduleDate->setDate(QDate::currentDate());
    ui->dateEditScheduleDate->setEnabled(false);

    connect(
        ui->checkBoxOverrideDate, &QCheckBox::stateChanged, [this](int state) {
            if (state == Qt::Unchecked) {
                ui->dateEditScheduleDate->setEnabled(false);
                ui->dateEditScheduleDate->setDate(QDate::currentDate());
            }
            else {
                ui->dateEditScheduleDate->setEnabled(true);
            }
        });
    connect(&workdaysModel,
            &WorkdayTrackerModel::workdaysChanged,
            [this](const WorkdayTracker& updatedTracker) {
                onWorkdayTrackerChanged(updatedTracker);
            });
    connect(ui->btnAddExceptionalDay, &QPushButton::clicked, [&]() {
        pickDateDialog.exec();
    });
    connect(&scheduleModel_,
            &QAbstractItemModel::rowsAboutToBeRemoved,
            this,
            &WorkdaysDialog::onScheduleRemovedFromModel);
    connect(&exceptionalDaysModel_,
            &QAbstractItemModel::rowsAboutToBeRemoved,
            this,
            &WorkdaysDialog::onExceptionalDayRemovedFromModel);
    connect(&pickDateDialog,
            &QDialog::accepted,
            this,
            &WorkdaysDialog::addExceptionalDay);
    connect(ui->pushButtonAddSchedule,
            &QPushButton::clicked,
            this,
            &WorkdaysDialog::addSchedule);
}

WorkdaysDialog::~WorkdaysDialog() = default;

void WorkdaysDialog::accept()
{
    workdaysModel.changeWorkdayData(candidateTracker);
    QDialog::accept();
}

void WorkdaysDialog::reject()
{
    candidateTracker = workdaysModel.workdayTracker();
    QDialog::reject();
}

void WorkdaysDialog::onWorkdayTrackerChanged(
    const WorkdayTracker& updatedTracker)
{
    candidateTracker = updatedTracker;
    updateWorkdaysView(candidateTracker);
    updateSchedulesView(candidateTracker);
    initializeDayBoxes(candidateTracker.currentSchedule());
}

void WorkdaysDialog::updateWorkdaysView(const WorkdayTracker& updatedTracker)
{
    auto* model = ui->listViewExceptionalDays->model();
    if (!model)
        return;
    disconnect(model,
               &QAbstractItemModel::rowsAboutToBeRemoved,
               this,
               &WorkdaysDialog::onExceptionalDayRemovedFromModel);
    const auto days = candidateTracker.exceptionalDays();
    replaceModelContent(*model, days);
    connect(model,
            &QAbstractItemModel::rowsAboutToBeRemoved,
            this,
            &WorkdaysDialog::onExceptionalDayRemovedFromModel);
}

void WorkdaysDialog::updateSchedulesView(const WorkdayTracker& updatedTracker)
{
    auto* model = ui->listViewSchedules->model();
    if (!model)
        return;
    disconnect(model,
               &QAbstractItemModel::rowsAboutToBeRemoved,
               this,
               &WorkdaysDialog::onScheduleRemovedFromModel);
    const auto schedules = candidateTracker.scheduleRoaster();
    replaceModelContent(*model, schedules);
    connect(model,
            &QAbstractItemModel::rowsAboutToBeRemoved,
            this,
            &WorkdaysDialog::onScheduleRemovedFromModel);
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

void WorkdaysDialog::addSchedule()
{
    const auto date = utils::toDate(ui->dateEditScheduleDate->date());
    const auto schedule = pollSchedule();
    candidateTracker.addWeekSchedule(date, schedule);
    auto* model = ui->listViewSchedules->model();
    if (!model)
        return;
    disconnect(model,
               &QAbstractItemModel::rowsAboutToBeRemoved,
               this,
               &WorkdaysDialog::onScheduleRemovedFromModel);
    replaceModelContent(*model, candidateTracker.scheduleRoaster());
    connect(model,
            &QAbstractItemModel::rowsAboutToBeRemoved,
            this,
            &WorkdaysDialog::onScheduleRemovedFromModel);
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
    using sprint_timer::ui::qt_gui::utils::toDate;
    const std::vector days{
        expand(pickDateDialog.startDate(), pickDateDialog.numDays())};
    const int goal{pickDateDialog.targetGoal()};
    for (const auto& day : days) {
        candidateTracker.addExceptionalDay(toDate(day), goal);
    }
    auto* model = ui->listViewExceptionalDays->model();
    disconnect(model,
               &QAbstractItemModel::rowsAboutToBeRemoved,
               this,
               &WorkdaysDialog::onExceptionalDayRemovedFromModel);
    replaceModelContent(*model, candidateTracker.exceptionalDays());
    connect(model,
            &QAbstractItemModel::rowsAboutToBeRemoved,
            this,
            &WorkdaysDialog::onExceptionalDayRemovedFromModel);
}

void WorkdaysDialog::onExceptionalDayRemovedFromModel(const QModelIndex&,
                                                      int first,
                                                      int)
{
    auto* model = ui->listViewExceptionalDays->model();
    if (!model)
        return;
    const auto entry = model->data(model->index(first, 0), Qt::EditRole)
                           .value<QPair<QDate, int>>();
    candidateTracker.removeExceptionalDay(utils::toDate(entry.first));
}

void WorkdaysDialog::onScheduleRemovedFromModel(const QModelIndex&,
                                                int first,
                                                int)
{
    auto* model = ui->listViewSchedules->model();
    const auto entry = model->data(model->index(first, 0), Qt::EditRole)
                           .value<QPair<QDate, WeekSchedule>>();
    candidateTracker.removeWeekSchedule(utils::toDate(entry.first));
}

} // namespace sprint_timer::ui::qt_gui


namespace {

template <typename T>
void replaceModelContent(QAbstractItemModel& model, const std::vector<T>& data)
{
    using sprint_timer::ui::qt_gui::utils::toQDate;
    model.removeRows(0, model.rowCount());
    model.insertRows(0, data.size());
    for (size_t row = 0; row < data.size(); ++row) {
        const auto& [date, payload] = data[row];
        QVariant entry;
        entry.setValue(QPair{toQDate(date), payload});
        model.setData(model.index(row, 0), entry);
    }
    model.sort(0, Qt::AscendingOrder);
}

// template <typename T>
// std::vector<std::pair<dw::Date, T>> pollModelData(QAbstractItemModel& model)
// {
//     using sprint_timer::ui::qt_gui::utils::toDate;
//     std::vector<std::pair<dw::Date, T>> data;
//     data.reserve(model.rowCount());
//
//     for (int row = 0; row < model.rowCount(); ++row) {
//         const auto index = model.index(row, 0);
//         const auto& [date, payload]
//             = model.data(index, Qt::EditRole).value<QPair<QDate, T>>();
//         data.push_back({toDate(date), payload});
//     }
//
//     return data;
// }

std::vector<QDate> expand(const QDate& startDate, int numDays)
{
    std::vector<QDate> days;
    days.reserve(numDays);
    auto consecutive_dates
        = [n = 0, &date = startDate]() mutable { return date.addDays(n++); };
    std::generate_n(std::back_inserter(days), numDays, consecutive_dates);
    return days;
}

// sprint_timer::WorkdayTracker buildTracker(
//     const std::vector<sprint_timer::WorkdayTracker::ScheduleData>& schedules,
//     const std::vector<sprint_timer::WorkdayTracker::DayData>&
//     exceptionalDays)
// {
//     sprint_timer::WorkdayTracker tracker;
//     for (const auto& [date, goal] : exceptionalDays)
//         tracker.addExceptionalDay(date, goal);
//     for (const auto& [date, schedule] : schedules)
//         tracker.addWeekSchedule(date, schedule);
//     return tracker;
// }

// template <typename T>
// void appendRow(QAbstractItemModel& model, T entry)
// {
//     QVariant ventry;
//     ventry.setValue(entry);
//     const int lastRow{model.rowCount()};
//     model.insertRows(lastRow, 1);
//     model.setData(model.index(lastRow, 0), ventry, Qt::EditRole);
//     model.sort(0, Qt::AscendingOrder);
// }

} // namespace
