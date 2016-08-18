/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "dialogs/AddSprintDialog.h"
#include "models/SprintModel.h"
#include "utils/DateTimeConverter.h"
#include "ui_add_sprint_dialog.h"


AddSprintDialog::AddSprintDialog(SprintModel* sprintModel,
                                                 TaskModel* taskModel,
                                                 int sprintDuration,
                                                 QDialog* parent)
    : QDialog(parent)
    , ui(new Ui::AddSprintDialog)
    , sprintModel(sprintModel)
    , taskModel(taskModel)
    , sprintDuration(sprintDuration)
{
    ui->setupUi(this);
    setData();
    connectSlots();
}

AddSprintDialog::~AddSprintDialog() { delete ui; }

void AddSprintDialog::setData()
{
    ui->cbPickTask->setModel(taskModel);
    ui->dateEditSprintDate->setDate(QDate::currentDate());
}

void AddSprintDialog::connectSlots()
{
    connect(ui->timeEditSprintStartTime,
            SIGNAL(timeChanged(QTime)),
            this,
            SLOT(autoAdjustFinishTime()));
}

void AddSprintDialog::autoAdjustFinishTime()
{
    QDateTime adjustedTime = ui->timeEditSprintStartTime->dateTime().addSecs(
        sprintDuration * 60);
    ui->timeEditSprintFinishTime->setDateTime(adjustedTime);
}

void AddSprintDialog::accept()
{
    QDateTime startTime
        = ui->timeEditSprintStartTime->dateTime().toTimeSpec(Qt::LocalTime);
    QDateTime finishTime
        = ui->timeEditSprintFinishTime->dateTime().toTimeSpec(Qt::LocalTime);
    if (startTime >= finishTime) {
        autoAdjustFinishTime();
    }

    const std::string taskUuid
        = taskModel->itemAt(ui->cbPickTask->currentIndex())
              .uuid();
    sprintModel->insert(TimeSpan{DateTimeConverter::dateTime(startTime),
                                   DateTimeConverter::dateTime(finishTime)},
                          taskUuid);
    QDialog::accept();
}
