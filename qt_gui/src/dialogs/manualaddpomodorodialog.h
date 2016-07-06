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
#ifndef MANUALADDPOMODORODIALOG_H
#define MANUALADDPOMODORODIALOG_H

#include "models/PomodoroModel.h"
#include "src/models/TaskModel.h"
#include <QDialog>
#include <QPointer>

namespace Ui {
class PomodoroManualAddDialog;
}

class PomodoroManualAddDialog : public QDialog {
    Q_OBJECT

public:
    PomodoroManualAddDialog(PomodoroModel* pomodoroModel,
                            TaskModel* taskModel,
                            int pomodoroDuration,
                            QDialog* parent = nullptr);
    ~PomodoroManualAddDialog();
    void accept() override;

private slots:
    void autoAdjustFinishTime();

private:
    Ui::PomodoroManualAddDialog* ui;
    QPointer<PomodoroModel> pomodoroModel;
    QPointer<TaskModel> taskModel;
    int pomodoroDuration;

    void setData();
    void connectSlots();
};

#endif // MANUALADDPOMODORODIALOG_H
