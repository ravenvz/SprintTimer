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
#ifndef ADDSPRINTDIALOG_H
#define ADDSPRINTDIALOG_H

#include "delegates/SubmissionItemDelegate.h"
#include "models/SprintModel.h"
#include "models/TaskModel.h"
#include <QDialog>
#include <QPointer>
#include <QtWidgets/QCalendarWidget>
#include <memory>

namespace Ui {
class AddSprintDialog;
} // namespace Ui

class AddSprintDialog : public QDialog {
    Q_OBJECT

public:
    AddSprintDialog(SprintModel* sprintModel,
                    TaskModel* taskModel,
                    int sprintDuration,
                    QDialog* parent = nullptr);

    ~AddSprintDialog() override;

    void accept() override;

private slots:
    void autoAdjustFinishTime();
    void autoAdjustStartTime();

private:
    Ui::AddSprintDialog* ui;
    QPointer<QCalendarWidget> datePicker;
    QPointer<SprintModel> sprintModel;
    QPointer<TaskModel> taskModel;
    const int sprintDuration;
    std::unique_ptr<SubmissionItemDelegate> submissionItemDelegate
        = std::make_unique<SubmissionItemDelegate>();

    void setData();

    int totalSprintLength() const;
};

#endif // ADDSPRINTDIALOG_H
