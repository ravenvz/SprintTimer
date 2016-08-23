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
#ifndef TASKVIEW_H_KN6BYIAY
#define TASKVIEW_H_KN6BYIAY


#include "dialogs/AddTaskDialog.h"
#include "dialogs/ConfirmationDialog.h"
#include "models/TagModel.h"
#include "models/TaskModel.h"
#include "widgets/TaskViewDelegate.h"
#include "widgets/TagEditor.h"
#include <QDropEvent>
#include <QListView>
#include <QMenu>
#include <QPointer>


class TaskView : public QListView {

public:
    explicit TaskView(QWidget* parent);

    QSize sizeHint() const override;

    // Overriding to catch dropEvent and prevent model start removing rows
    // attempting drag and drop by calling another method instead.
    void dropEvent(QDropEvent* event) final;

    void setModels(TaskModel* taskModel, TagModel* tagModel);

private:
    QPointer<TaskViewDelegate> taskViewDelegate;
    TagModel* tagModel;
    QPointer<TagEditorWidget> tagEditor;

    void editTask();

    void removeTask();

    void launchTagEditor();

public slots:

    void showTaskContextMenu(const QPoint& pos);
};


#endif /* end of include guard: TASKVIEW_H_KN6BYIAY */
