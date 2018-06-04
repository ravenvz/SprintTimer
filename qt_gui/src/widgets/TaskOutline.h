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
#ifndef TASKOUTLINE_H
#define TASKOUTLINE_H

#include "core/ICoreService.h"
#include "dialogs/AddTaskDialog.h"
#include "models/TagModel.h"
#include "models/TaskModel.h"
#include "widgets/ReordableListView.h"
#include "widgets/TagEditor.h"
#include "widgets/TaskViewDelegate.h"
#include <QPointer>
#include <QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <memory>


namespace Ui {
class TaskOutline;
} // namespace Ui

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

    ~TaskOutline() override;

    QSize sizeHint() const override;

private:
    Ui::TaskOutline* ui;
    ICoreService& coreService;
    QPointer<TagEditor> tagEditor;
    TaskModel* taskModel;
    TagModel* tagModel;
    std::unique_ptr<AddTaskDialog> addTaskDialog;
    QPointer<TaskViewDelegate> taskViewDelegate;
    const QSize desiredSize{300, 200};

    void launchTagEditor();
    void removeTask();
    void launchTaskEditor();

signals:
    void taskSelected(int row);

private slots:
    void onAddTaskButtonPushed();
    void onQuickAddTodoReturnPressed();
    void toggleTaskCompleted();
    void showContextMenu(const QPoint& pos);
    void addNewTask();
};

#endif // TASKOUTLINE_H
