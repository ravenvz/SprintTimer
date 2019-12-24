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
#ifndef TASKOUTLINE_H
#define TASKOUTLINE_H

#include "qt_gui/models/SprintModel.h"
#include "qt_gui/widgets/TaskView.h"
#include <QAbstractItemModel>
#include <QLineEdit>
#include <QWidget>
#include <core/ISprintStorageReader.h>
#include <core/QueryInvoker.h>
#include <memory>

#include "qt_gui/dialogs/AddTaskDialog.h"

namespace sprint_timer::ui::qt_gui {

class AddTaskDialog;

/* Responsible for providing user interface for interactive
 * task management, so that user could view current unfinished
 * tasks, add/edit and remove task and tags. */
class TaskOutline : public QWidget {
public:
    TaskOutline(QAbstractItemModel& taskModel,
                SprintModel& sprintModel,
                std::unique_ptr<TaskView> taskView,
                AddTaskDialog& addTaskDialog,
                QWidget* parent = nullptr);

    void onSprintSubmissionRequested(
        const std::vector<dw::DateTimeRange>& intervals);

private:
    QPointer<TagEditor> tagEditor;
    QAbstractItemModel& taskModel;
    SprintModel& sprintModel;
    TaskView* taskView;
    AddTaskDialog& addTaskDialog;
    QLineEdit* quickAddTask;

    void onAddTaskButtonPushed();

    void onQuickAddTodoReturnPressed();

    void insertTask(const entities::Task& task);
};

} // namespace sprint_timer::ui::qt_gui

#endif // TASKOUTLINE_H
