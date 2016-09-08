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
#ifndef ADDSPRINTDIALOG_H
#define ADDSPRINTDIALOG_H

#include "models/SprintModel.h"
#include "src/models/TaskModel.h"
#include <QDialog>
#include <QPointer>
#include <QtWidgets/QCalendarWidget>

namespace Ui {
class AddSprintDialog;
}

class AddSprintDialog : public QDialog {
    Q_OBJECT

public:
    AddSprintDialog(SprintModel* sprintModel,
                            TaskModel* taskModel,
                            int sprintDuration,
                            QDialog* parent = nullptr);

    ~AddSprintDialog();

    void accept() override;

private slots:
    void autoAdjustFinishTime(const QDateTime& dateTime);
    void autoAdjustStartTime(const QDateTime& dateTime);

private:
    Ui::AddSprintDialog* ui;
    QCalendarWidget* datePicker;
    QPointer<SprintModel> sprintModel;
    QPointer<TaskModel> taskModel;
    int sprintDuration;

    void setData();
};

#endif // ADDSPRINTDIALOG_H
