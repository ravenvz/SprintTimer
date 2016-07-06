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
#include "dialogs/manualaddpomodorodialog.h"
#include "models/PomodoroModel.h"
#include "utils/DateTimeConverter.h"
#include "ui_manual_add_pomodoro_dialog.h"


PomodoroManualAddDialog::PomodoroManualAddDialog(PomodoroModel* pomodoroModel,
                                                 TaskModel* taskModel,
                                                 int pomodoroDuration,
                                                 QDialog* parent)
    : QDialog(parent)
    , ui(new Ui::PomodoroManualAddDialog)
    , pomodoroModel(pomodoroModel)
    , taskModel(taskModel)
    , pomodoroDuration(pomodoroDuration)
{
    ui->setupUi(this);
    setData();
    connectSlots();
}

PomodoroManualAddDialog::~PomodoroManualAddDialog() { delete ui; }

void PomodoroManualAddDialog::setData()
{
    ui->cbPickTask->setModel(taskModel);
    ui->dateEditPomodoroDate->setDate(QDate::currentDate());
}

void PomodoroManualAddDialog::connectSlots()
{
    connect(ui->timeEditPomodoroStartTime,
            SIGNAL(timeChanged(QTime)),
            this,
            SLOT(autoAdjustFinishTime()));
}

void PomodoroManualAddDialog::autoAdjustFinishTime()
{
    QDateTime adjustedTime = ui->timeEditPomodoroStartTime->dateTime().addSecs(
        pomodoroDuration * 60);
    ui->timeEditPomodoroFinishTime->setDateTime(adjustedTime);
}

void PomodoroManualAddDialog::accept()
{
    QDateTime startTime
        = ui->timeEditPomodoroStartTime->dateTime().toTimeSpec(Qt::LocalTime);
    QDateTime finishTime
        = ui->timeEditPomodoroFinishTime->dateTime().toTimeSpec(Qt::LocalTime);
    if (startTime >= finishTime) {
        autoAdjustFinishTime();
    }

    const std::string taskUuid
        = taskModel->itemAt(ui->cbPickTask->currentIndex())
              .uuid();
    pomodoroModel->insert(TimeSpan{DateTimeConverter::dateTime(startTime),
                                   DateTimeConverter::dateTime(finishTime)},
                          taskUuid);
    QDialog::accept();
}
