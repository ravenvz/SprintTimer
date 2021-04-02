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
#include "qt_gui/dialogs/WorkScheduleEditor.h"
#include "qt_gui/dialogs/AddExceptionalDayDialog.h"
#include "qt_gui/models/ExtraDayModel.h"
#include "qt_gui/models/WeekScheduleModel.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include "ui_workdays_dialog.h"
#include <QAbstractItemModel>

namespace {

/* Remove all rows from given model and fill it with data.
 *
 * Convinient function since models used in WorkScheduleEditor do have same
 * structure. In particular, it expects that template parameter has structure
 * T = QPair<QDate, A> where A is
 * any type registered withing Qt metatype system.
 * It should be noted that all model data is removed and refilled, one
 * should be careful to supress/disconnect signals it is required. */
template <typename First, typename Second>
void replaceModelContent(QAbstractItemModel& model,
                         const std::vector<std::pair<First, Second>>& data);

constexpr size_t daysInWeek{7};

} // namespace

namespace sprint_timer::ui::qt_gui {

WorkScheduleEditor::WorkScheduleEditor(QDialog* parent_)
    : DisplayableDialog{parent_}
    , ui{std::make_unique<Ui::WorkScheduleEditor>()}
    , exceptionalDaysModel{std::make_unique<ExtraDayModel>()}
    , roasterBufferModel{std::make_unique<WeekScheduleModel>()}
{
    ui->setupUi(this);

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
    connect(ui->btnAddExceptionalDay, &QPushButton::clicked, [this]() {
        if (auto p = presenter(); p) {
            p.value()->onAddExceptionalRequested();
        }
    });
    connect(exceptionalDaysModel.get(),
            &QAbstractItemModel::rowsAboutToBeRemoved,
            this,
            &WorkScheduleEditor::onExcDayAboutToBeRemoved);
    connect(ui->pushButtonAddSchedule,
            &QPushButton::clicked,
            this,
            &WorkScheduleEditor::addSchedule);
    ui->listViewSchedules->setModel(roasterBufferModel.get());
    ui->listViewExceptionalDays->setModel(exceptionalDaysModel.get());
}

WorkScheduleEditor::~WorkScheduleEditor() = default;

void WorkScheduleEditor::displayCurrentWeekSchedule(
    const std::vector<contracts::WorkScheduleEditor::DayAndGoal>& weekSchedule)
{
    std::array<QLabel*, daysInWeek> labels{ui->labelFirstDay,
                                           ui->labelSecondDay,
                                           ui->labelThirdDay,
                                           ui->labelFourthDay,
                                           ui->labelFifthDay,
                                           ui->labelSixthDay,
                                           ui->labelSeventhDay};
    std::array<QSpinBox*, daysInWeek> goals{ui->firstDayGoal,
                                            ui->secondDayGoal,
                                            ui->thirdDayGoal,
                                            ui->fourthDayGoal,
                                            ui->fifthDayGoal,
                                            ui->sixthDayGoal,
                                            ui->seventhDayGoal};
    const QLocale defaultLocale;
    for (size_t pos{0}; pos < labels.size(); ++pos) {
        const auto [dayNum, goal] = weekSchedule[pos];
        labels[pos]->setText(
            defaultLocale.dayName(dayNum, QLocale::FormatType::LongFormat));
        goals[pos]->setValue(goal);
    }
}

void WorkScheduleEditor::displayRoaster(
    const std::vector<contracts::WorkScheduleEditor::RoasterRow>& roaster)
{
    // How it works?
    //
    // The problem
    //
    // We connecting to `rowsAboutToBeRemoved` signal of roaster model to know
    // when user wants to delete some weekSchedule. Since WorkSchedule is
    // complicated object we prefer to not expose it to the view. Deleting some
    // week schedule might end up with merging some other week schedules -
    // something that WorkSchedule object knows how to deal with, and we do not
    // want duplicate this logic inside the view itself. Merging schedules might
    // leave us with partially correct representation if we only remove one row,
    // since potentially more than one row should be removed.
    //
    // So after mentioned signal fires, presenter ends up calling this
    // method again with properly merged weeks schedules. But we cannot replace
    // model content while `in the process` of handling signal from it, as since
    // we connecting to rowsAboutToBeRemoved signal, that row from the model
    // will be removed after that, but since we want to replace model content,
    // it might remove the wrong row thus again leaving user with confusing
    // representation.
    //
    // Ugly workaround
    //
    // Instead one model, two model objects are used. While dealing with
    // rowsAboutToBeRemoved signal from roasterBufferModel, we create new
    // roasterModel, update it with fresh data from presenter and set it to the
    // list view. We can't remove roasterBufferModel right away, since we are
    // `inside` it's signal's handler, so we swap roasterModel and
    // roasterBufferModel. This way, roasterBufferModel will finish removing
    // it's row (and we don't care about it's content anymore). It will be
    // replaced by a new instance at next call to this method (it will be
    // roasterBufferModel at time of this call).
    roasterModel = std::make_unique<WeekScheduleModel>();
    std::vector<std::pair<dw::Date, QString>> data;
    data.reserve(roaster.size());
    std::transform(cbegin(roaster),
                   cend(roaster),
                   std::back_inserter(data),
                   [](const auto& elem) -> std::pair<dw::Date, QString> {
                       auto [dwDate, scheduleString] = elem;
                       return {dwDate, QString::fromStdString(scheduleString)};
                   });
    replaceModelContent(*roasterModel, data);
    connect(roasterModel.get(),
            &QAbstractItemModel::rowsAboutToBeRemoved,
            this,
            &WorkScheduleEditor::onScheduleRemovedFromModel);
    ui->listViewSchedules->setModel(roasterModel.get());
    std::swap(roasterModel, roasterBufferModel);
}

void WorkScheduleEditor::displayExceptionalDays(
    const std::vector<WorkSchedule::DateGoal>& exceptionalDays)
{
    replaceModelContent(*exceptionalDaysModel, exceptionalDays);
}

void WorkScheduleEditor::displayAddExceptionalDaysDialog(
    dw::Weekday firstDayOfWeek, dw::Date preselectedDate)
{
    AddExceptionalDayDialog::OutputData outputData;
    AddExceptionalDayDialog dialog{firstDayOfWeek, preselectedDate, outputData};
    if (dialog.exec() == QDialog::Accepted) {
        auto [startDate, numDays, sprintsPerDay] = outputData;
        if (auto p = presenter(); p) {
            p.value()->onExceptionalDaysAdded(
                startDate, numDays, sprintsPerDay);
        }
    }
}

void WorkScheduleEditor::accept()
{
    if (auto p = presenter(); p) {
        p.value()->onScheduleChangeConfirmed();
    }
    QDialog::accept();
}

void WorkScheduleEditor::reject()
{
    if (auto p = presenter(); p) {
        p.value()->onRevertChanges();
    }
    QDialog::reject();
}

void WorkScheduleEditor::addSchedule()
{
    const auto date = utils::toDate(ui->dateEditScheduleDate->date());
    if (auto p = presenter(); p) {
        p.value()->onWeekScheduleAdded(pollSchedule(), date);
    }
}

std::vector<uint8_t> WorkScheduleEditor::pollSchedule() const
{
    std::vector<uint8_t> goals(daysInWeek, 0);
    std::array<QSpinBox*, daysInWeek> valueBoxes{ui->firstDayGoal,
                                                 ui->secondDayGoal,
                                                 ui->thirdDayGoal,
                                                 ui->fourthDayGoal,
                                                 ui->fifthDayGoal,
                                                 ui->sixthDayGoal,
                                                 ui->seventhDayGoal};
    std::transform(cbegin(valueBoxes),
                   cend(valueBoxes),
                   begin(goals),
                   [](auto* box) { return box->value(); });
    return goals;
}

void WorkScheduleEditor::onExcDayAboutToBeRemoved(const QModelIndex&,
                                                  int first,
                                                  int)
{
    if (auto p = presenter(); p) {
        auto* model = ui->listViewExceptionalDays->model();
        if (!model)
            return;
        const auto entry = model->data(model->index(first, 0), Qt::EditRole)
                               .value<QPair<QDate, int>>();
        QSignalBlocker signalBlocker{model};
        p.value()->onExceptionalDayRemoved(utils::toDate(entry.first));
    }
}

void WorkScheduleEditor::onScheduleRemovedFromModel(const QModelIndex&,
                                                    int first,
                                                    int)
{
    if (auto p = presenter(); p) {
        auto* model = ui->listViewSchedules->model();
        const auto entry = model->data(model->index(first, 0), Qt::EditRole)
                               .value<QPair<QDate, QString>>();
        p.value()->onWeekScheduleRemoved(utils::toDate(entry.first));
    }
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
}

} // namespace
