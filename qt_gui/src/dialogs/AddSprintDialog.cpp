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
#include "qt_gui/dialogs/AddSprintDialog.h"
#include "qt_gui/models/TaskModelRoles.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include "ui_add_sprint_dialog.h"
#include <core/use_cases/RegisterNewSprintBulk.h>

namespace {

std::vector<sprint_timer::entities::Sprint>
generateConsecutiveSprints(const QDateTime& initialStartTime,
                           std::chrono::minutes sprintDuration,
                           const std::string& taskUuid,
                           int numSprints);

} // namespace

namespace sprint_timer::ui::qt_gui {

AddSprintDialog::AddSprintDialog(const IConfig& applicationSettings_,
                                 ISprintStorageWriter& sprintWriter_,
                                 CommandInvoker& commandInvoker_,
                                 std::unique_ptr<QComboBox> taskSelector_,
                                 QDialog* parent_)
    : QDialog{parent_}
    , ui{std::make_unique<Ui::AddSprintDialog>()}
    , datePicker{std::make_unique<QCalendarWidget>()}
    , applicationSettings{applicationSettings_}
    , sprintWriter{sprintWriter_}
    , commandInvoker{commandInvoker_}
    , taskSelector{taskSelector_.get()}
{
    ui->setupUi(this);

    ui->gridLayout->addWidget(taskSelector_.release(), 1, 0, 1, 3);

    datePicker->setMaximumDate(QDate::currentDate());
    if (applicationSettings.firstDayOfWeek() == dw::Weekday::Monday)
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
    return ui->sbNumSprints->value() *
           duration_cast<seconds>(applicationSettings.sprintDuration());
}

void AddSprintDialog::accept()
{
    if (taskSelector->currentIndex() == -1)
        return;

    const auto initialStartTime =
        ui->timeEditSprintStartTime->dateTime().toTimeSpec(Qt::LocalTime);
    const std::string taskUuid{
        taskSelector->currentData(static_cast<int>(TaskModelRoles::GetIdRole))
            .toString()
            .toStdString()};
    const auto sprintDuration = applicationSettings.sprintDuration();

    const auto sprints = generateConsecutiveSprints(
        initialStartTime, sprintDuration, taskUuid, ui->sbNumSprints->value());

    commandInvoker.executeCommand(
        std::make_unique<use_cases::RegisterNewSprintBulk>(sprintWriter,
                                                           sprints));
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
    using sprint_timer::ui::qt_gui::utils::toDateTime;

    std::vector<Sprint> sprints;
    for (int i = 0; i < numSprints; ++i) {
        const auto startTime = initialStartTime.addSecs(
            i * duration_cast<seconds>(sprintDuration).count());
        const auto finishTime =
            startTime.addSecs(duration_cast<seconds>(sprintDuration).count());
        sprints.push_back(Sprint{
            taskUuid,
            dw::DateTimeRange{toDateTime(startTime), toDateTime(finishTime)}});
    }

    return sprints;
}

} // namespace
