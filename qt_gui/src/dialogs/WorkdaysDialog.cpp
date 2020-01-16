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
#include <QAbstractItemModel>

Q_DECLARE_METATYPE(sprint_timer::WeekSchedule)

namespace {

std::vector<QDate> expand(const QDate& startDate, int numDays);

/* Remove all rows from given model and fill it with data.
 *
 * Despite being template, it is not generic function; just a convinience one,
 * as models used in WorkdaysDialog do have same structure. In particular, it
 * expects that template parameter has structure T = QPair<QDate, A> where A is
 * any type registered withing Qt metatype system. */
template <typename First, typename Second>
void replaceModelContent(QAbstractItemModel& model,
                         const std::vector<std::pair<First, Second>>& data);

} // namespace

namespace sprint_timer::ui::qt_gui {

WorkdaysDialog::WorkdaysDialog(AddExceptionalDayDialog& addExcDayDialog_,
                               QAbstractItemModel& exceptionalDaysModel_,
                               WorkScheduleWrapper& workScheduleWrapper_,
                               QAbstractItemModel& weekScheduleModel_,
                               QDialog* parent_)
    : QDialog{parent_}
    , ui{std::make_unique<Ui::WorkdaysDialog>()}
    , pickDateDialog{addExcDayDialog_}
    , workScheduleWrapper{workScheduleWrapper_}
{
    ui->setupUi(this);
    ui->listViewExceptionalDays->setModel(&exceptionalDaysModel_);
    ui->listViewSchedules->setModel(&weekScheduleModel_);

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
    connect(&workScheduleWrapper,
            &WorkScheduleWrapper::workScheduleChanged,
            [this](const WorkSchedule& updatedWorkSchedule) {
                onWorkScheduleChanged(updatedWorkSchedule);
            });
    connect(ui->btnAddExceptionalDay, &QPushButton::clicked, [&]() {
        pickDateDialog.exec();
    });
    connect(&weekScheduleModel_,
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
    workScheduleWrapper.changeSchedule(candidateSchedule);
    QDialog::accept();
}

void WorkdaysDialog::reject()
{
    candidateSchedule = workScheduleWrapper.workSchedule();
    QDialog::reject();
}

void WorkdaysDialog::onWorkScheduleChanged(
    const WorkSchedule& updatedWorkSchedule)
{
    candidateSchedule = updatedWorkSchedule;
    updateWorkdaysView(candidateSchedule);
    updateSchedulesView(candidateSchedule);
    initializeDayBoxes(candidateSchedule.currentWeekSchedule());
}

void WorkdaysDialog::updateWorkdaysView(const WorkSchedule& updatedWorkSchedule)
{
    auto* model = ui->listViewExceptionalDays->model();
    if (!model)
        return;
    disconnect(model,
               &QAbstractItemModel::rowsAboutToBeRemoved,
               this,
               &WorkdaysDialog::onExceptionalDayRemovedFromModel);
    const auto days = candidateSchedule.exceptionalDays();
    replaceModelContent(*model, days);
    connect(model,
            &QAbstractItemModel::rowsAboutToBeRemoved,
            this,
            &WorkdaysDialog::onExceptionalDayRemovedFromModel);
}

void WorkdaysDialog::updateSchedulesView(
    const WorkSchedule& updatedWorkSchedule)
{
    auto* model = ui->listViewSchedules->model();
    if (!model)
        return;
    disconnect(model,
               &QAbstractItemModel::rowsAboutToBeRemoved,
               this,
               &WorkdaysDialog::onScheduleRemovedFromModel);
    const auto schedules = candidateSchedule.roaster();
    replaceModelContent(*model, schedules);
    connect(model,
            &QAbstractItemModel::rowsAboutToBeRemoved,
            this,
            &WorkdaysDialog::onScheduleRemovedFromModel);
}

void WorkdaysDialog::initializeDayBoxes(const WeekSchedule& weekSchedule)
{
    using namespace dw;
    ui->mondayBox->setValue(weekSchedule.targetGoal(Weekday::Monday));
    ui->tuesdayBox->setValue(weekSchedule.targetGoal(Weekday::Tuesday));
    ui->wednesdayBox->setValue(weekSchedule.targetGoal(Weekday::Wednesday));
    ui->thursdayBox->setValue(weekSchedule.targetGoal(Weekday::Thursday));
    ui->fridayBox->setValue(weekSchedule.targetGoal(Weekday::Friday));
    ui->saturdayBox->setValue(weekSchedule.targetGoal(Weekday::Saturday));
    ui->sundayBox->setValue(weekSchedule.targetGoal(Weekday::Sunday));
}

void WorkdaysDialog::addSchedule()
{
    const auto date = utils::toDate(ui->dateEditScheduleDate->date());
    const auto weekSchedule = pollSchedule();
    candidateSchedule.addWeekSchedule(date, weekSchedule);
    auto* model = ui->listViewSchedules->model();
    if (!model)
        return;
    disconnect(model,
               &QAbstractItemModel::rowsAboutToBeRemoved,
               this,
               &WorkdaysDialog::onScheduleRemovedFromModel);
    replaceModelContent(*model, candidateSchedule.roaster());
    connect(model,
            &QAbstractItemModel::rowsAboutToBeRemoved,
            this,
            &WorkdaysDialog::onScheduleRemovedFromModel);
}

WeekSchedule WorkdaysDialog::pollSchedule() const
{
    using namespace dw;

    WeekSchedule weekSchedule;

    weekSchedule.setTargetGoal(Weekday::Monday, ui->mondayBox->value());
    weekSchedule.setTargetGoal(Weekday::Tuesday, ui->tuesdayBox->value());
    weekSchedule.setTargetGoal(Weekday::Wednesday, ui->wednesdayBox->value());
    weekSchedule.setTargetGoal(Weekday::Thursday, ui->thursdayBox->value());
    weekSchedule.setTargetGoal(Weekday::Friday, ui->fridayBox->value());
    weekSchedule.setTargetGoal(Weekday::Saturday, ui->saturdayBox->value());
    weekSchedule.setTargetGoal(Weekday::Sunday, ui->sundayBox->value());

    return weekSchedule;
}

void WorkdaysDialog::addExceptionalDay()
{
    using sprint_timer::ui::qt_gui::utils::toDate;
    const std::vector days{
        expand(pickDateDialog.startDate(), pickDateDialog.numDays())};
    const int goal{pickDateDialog.targetGoal()};
    for (const auto& day : days) {
        candidateSchedule.addExceptionalDay(toDate(day), goal);
    }
    auto* model = ui->listViewExceptionalDays->model();
    disconnect(model,
               &QAbstractItemModel::rowsAboutToBeRemoved,
               this,
               &WorkdaysDialog::onExceptionalDayRemovedFromModel);
    replaceModelContent(*model, candidateSchedule.exceptionalDays());
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
    candidateSchedule.removeExceptionalDay(utils::toDate(entry.first));
}

void WorkdaysDialog::onScheduleRemovedFromModel(const QModelIndex&,
                                                int first,
                                                int)
{
    auto* model = ui->listViewSchedules->model();
    const auto entry = model->data(model->index(first, 0), Qt::EditRole)
                           .value<QPair<QDate, WeekSchedule>>();
    candidateSchedule.removeWeekSchedule(utils::toDate(entry.first));
}

} // namespace sprint_timer::ui::qt_gui

namespace {

template <typename First, typename Second>
void replaceModelContent(QAbstractItemModel& model,
                         const std::vector<std::pair<First, Second>>& data)
{
    using sprint_timer::ui::qt_gui::utils::toQDate;
    model.removeRows(0, model.rowCount());
    model.insertRows(0, data.size());
    for (size_t row = 0; row < data.size(); ++row) {
        const auto& [date, payload] = data[row];
        QVariant entry;
        entry.setValue(QPair<QDate, Second>{toQDate(date), payload});
        model.setData(model.index(row, 0), entry);
    }
    model.sort(0, Qt::AscendingOrder);
}

std::vector<QDate> expand(const QDate& startDate, int numDays)
{
    std::vector<QDate> days;
    days.reserve(numDays);
    auto consecutive_dates = [n = 0, &date = startDate]() mutable {
        return date.addDays(n++);
    };
    std::generate_n(std::back_inserter(days), numDays, consecutive_dates);
    return days;
}

} // namespace
