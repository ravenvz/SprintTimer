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
#include "qt_gui/dialogs/AddSprintDialog.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include "ui_add_sprint_dialog.h"

namespace {

std::vector<sprint_timer::entities::Sprint>
generateConsecutiveSprints(const QDateTime& initialStartTime,
                           std::chrono::minutes sprintDuration,
                           const std::string& taskUuid,
                           int numSprints);

} // namespace

namespace sprint_timer::ui::qt_gui {

AddSprintDialog::AddSprintDialog(const IConfig& applicationSettings,
                                 SprintModel& sprintModel,
                                 TaskModel& taskModel,
                                 QDialog* parent)
    : QDialog{parent}
    , ui{std::make_unique<Ui::AddSprintDialog>()}
    , datePicker{std::make_unique<QCalendarWidget>()}
    , applicationSettings{applicationSettings}
    , sprintModel{sprintModel}
    , taskModel{taskModel}
{
    ui->setupUi(this);

    ui->cbPickTask->setModel(&taskModel);
    ui->cbPickTask->setItemDelegate(submissionItemDelegate.get());

    datePicker->setMaximumDate(QDate::currentDate());
    if (applicationSettings.firstDayOfWeek() == FirstDayOfWeek::Monday)
        datePicker->setFirstDayOfWeek(Qt::Monday);
    datePicker->setWindowModality(Qt::ApplicationModal);

    connect(ui->timeEditSprintStartTime,
            &QTimeEdit::dateTimeChanged,
            this,
            &AddSprintDialog::adjustFinishTime);
    connect(ui->sbNumSprints,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &AddSprintDialog::adjustFinishTime);
    connect(ui->timeEditSprintFinishTime,
            &QTimeEdit::dateTimeChanged,
            this,
            &AddSprintDialog::adjustStartTime);
    connect(ui->pushButtonPickDate, &QPushButton::clicked, [this]() {
        datePicker->show();
    });
    connect(
        datePicker.get(), &QCalendarWidget::clicked, [this](const QDate& date) {
            ui->dateEditSprintDate->setDate(date);
            datePicker->close();
        });
    resetDataFields();
}

AddSprintDialog::~AddSprintDialog() = default;

void AddSprintDialog::adjustFinishTime()
{
    ui->timeEditSprintFinishTime->setDateTime(
        ui->timeEditSprintStartTime->dateTime().addSecs(
            totalSprintLength().count()));
}

void AddSprintDialog::adjustStartTime()
{
    ui->timeEditSprintStartTime->setDateTime(
        ui->timeEditSprintFinishTime->dateTime().addSecs(
            -totalSprintLength().count()));
}

std::chrono::seconds AddSprintDialog::totalSprintLength() const
{
    using namespace std::chrono;
    return ui->sbNumSprints->value()
        * duration_cast<seconds>(applicationSettings.sprintDuration());
}

void AddSprintDialog::accept()
{
    if (ui->cbPickTask->currentIndex() == -1)
        return;

    const auto initialStartTime
        = ui->timeEditSprintStartTime->dateTime().toTimeSpec(Qt::LocalTime);
    const std::string taskUuid
        = taskModel.itemAt(ui->cbPickTask->currentIndex()).uuid();
    const auto sprintDuration = applicationSettings.sprintDuration();

    auto sprints = generateConsecutiveSprints(
        initialStartTime, sprintDuration, taskUuid, ui->sbNumSprints->value());

    sprintModel.insert(sprints);
    resetDataFields();
    QDialog::accept();
}

void AddSprintDialog::resetDataFields()
{
    ui->dateEditSprintDate->setDate(QDate::currentDate());
    adjustFinishTime();
    ui->sbNumSprints->setValue(1);
}

} // namespace sprint_timer::ui::qt_gui

namespace {

std::vector<sprint_timer::entities::Sprint>
generateConsecutiveSprints(const QDateTime& initialStartTime,
                           std::chrono::minutes sprintDuration,
                           const std::string& taskUuid,
                           int numSprints)
{
    using namespace std::chrono;
    using sprint_timer::entities::Sprint;
    using sprint_timer::ui::qt_gui::DateTimeConverter;

    std::vector<Sprint> sprints;
    for (int i = 0; i < numSprints; ++i) {
        const auto startTime = initialStartTime.addSecs(
            i * duration_cast<seconds>(sprintDuration).count());
        const auto finishTime
            = startTime.addSecs(duration_cast<seconds>(sprintDuration).count());
        sprints.push_back(
            Sprint{taskUuid,
                   dw::DateTimeRange{DateTimeConverter::dateTime(startTime),
                                     DateTimeConverter::dateTime(finishTime)}});
    }

    return sprints;
}

} // namespace
