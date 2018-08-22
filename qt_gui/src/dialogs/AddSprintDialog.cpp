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

namespace sprint_timer::ui::qt_gui {

namespace {
    constexpr int secondsInMinute{60};
}

AddSprintDialog::AddSprintDialog(IConfig& applicationSettings,
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
    ui->dateEditSprintDate->setDate(QDate::currentDate());

    datePicker->setMaximumDate(QDate::currentDate());
    // TODO make this configurable with settings.
    // See also DateRangePickDialog's related TODO
    datePicker->setFirstDayOfWeek(Qt::Monday);
    datePicker->setWindowModality(Qt::ApplicationModal);

    connect(ui->timeEditSprintStartTime,
            &QTimeEdit::dateTimeChanged,
            this,
            &AddSprintDialog::autoAdjustFinishTime);
    connect(ui->sbNumSpints,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &AddSprintDialog::autoAdjustFinishTime);
    connect(ui->timeEditSprintFinishTime,
            &QTimeEdit::dateTimeChanged,
            this,
            &AddSprintDialog::autoAdjustStartTime);
    connect(ui->pushButtonPickDate, &QPushButton::clicked, [this]() {
        datePicker->show();
    });
    connect(
        datePicker.get(), &QCalendarWidget::clicked, [this](const QDate& date) {
            ui->dateEditSprintDate->setDate(date);
            datePicker->close();
        });
}

AddSprintDialog::~AddSprintDialog() = default;

void AddSprintDialog::autoAdjustFinishTime()
{
    ui->timeEditSprintFinishTime->setDateTime(
        ui->timeEditSprintStartTime->dateTime().addSecs(totalSprintLength()));
}

void AddSprintDialog::autoAdjustStartTime()
{
    ui->timeEditSprintStartTime->setDateTime(
        ui->timeEditSprintFinishTime->dateTime().addSecs(-totalSprintLength()));
}

int AddSprintDialog::totalSprintLength() const
{
    return ui->sbNumSpints->value() * applicationSettings.sprintDuration()
        * secondsInMinute;
}

void AddSprintDialog::accept()
{
    if (ui->cbPickTask->currentIndex() == -1)
        return;

    auto initialStartTime
        = ui->timeEditSprintStartTime->dateTime().toTimeSpec(Qt::LocalTime);

    const std::string taskUuid
        = taskModel.itemAt(ui->cbPickTask->currentIndex()).uuid();
    std::vector<entities::Sprint> sprints;

    const auto sprintDuration = applicationSettings.sprintDuration();

    for (int i = 0; i < ui->sbNumSpints->value(); ++i) {
        auto startTime
            = initialStartTime.addSecs(i * sprintDuration * secondsInMinute);
        auto finishTime = startTime.addSecs(sprintDuration * secondsInMinute);
        entities::Sprint sprint{
            taskUuid,
            TimeSpan{DateTimeConverter::dateTime(startTime),
                     DateTimeConverter::dateTime(finishTime)}};
        sprints.push_back(sprint);
    }

    sprintModel.insert(sprints);
    QDialog::accept();
}

} // namespace sprint_timer::ui::qt_gui
