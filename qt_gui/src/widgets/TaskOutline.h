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
#ifndef TASKOUTLINE_H
#define TASKOUTLINE_H

#include <QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include "core/ICoreService.h"
#include "models/TaskModel.h"
#include "models/TagModel.h"
#include "widgets/ReordableListView.h"
#include <QPointer>
#include "widgets/TagEditor.h"
#include "widgets/TaskViewDelegate.h"


namespace Ui {
class TaskOutline;
}

/* Responsible for providing user interface for interactive
 * task management, so that user could view current unfinished
 * tasks, add/edit and remove task and tags. */
class TaskOutline : public QWidget {

    Q_OBJECT

public:
    TaskOutline(ICoreService& coreService,
                TaskModel* taskModel,
                TagModel* tagModel,
                QWidget* parent);

    ~TaskOutline();

    QSize sizeHint() const override;

private:
    Ui::TaskOutline* ui;
    ICoreService& coreService;
    QPointer<TagEditorWidget> tagEditor;
    TaskModel* taskModel;
    TagModel* tagModel;
    QPointer<TaskViewDelegate> taskViewDelegate;
    const QSize desiredSize{300, 200};

    void launchTagEditor();
    void removeTask();
    void launchTaskEditor();

signals:
    void taskSelected(const int row);

private slots:
    void onAddTaskButtonPushed();
    void onQuickAddTodoReturnPressed();
    void toggleTaskCompleted();
    void showContextMenu(const QPoint& pos);
};

#endif // TASKOUTLINE_H
