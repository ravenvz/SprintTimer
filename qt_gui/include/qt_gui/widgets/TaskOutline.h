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

#include "qt_gui/delegates/HistoryItemDelegate.h"
#include "qt_gui/delegates/TaskItemDelegate.h"
#include "qt_gui/dialogs/AddTaskDialog.h"
#include "qt_gui/models/HistoryModel.h"
#include "qt_gui/models/TagModel.h"
#include "qt_gui/models/TaskModel.h"
#include "qt_gui/widgets/ReordableListView.h"
#include "qt_gui/widgets/TagEditor.h"
#include "qt_gui/widgets/TaskSprintsView.h"
#include <QPointer>
#include <QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <core/ICoreService.h>
#include <memory>

namespace Ui {
class TaskOutline;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

/* Responsible for providing user interface for interactive
 * task management, so that user could view current unfinished
 * tasks, add/edit and remove task and tags. */
class TaskOutline : public QWidget {

    Q_OBJECT

public:
    TaskOutline(ICoreService& coreService,
                TaskModel& taskModel,
                TagModel& tagModel,
                QWidget* parent);

    ~TaskOutline() override;

    QSize sizeHint() const override;

private:
    std::unique_ptr<Ui::TaskOutline> ui;
    ICoreService& coreService;
    QPointer<TagEditor> tagEditor;
    TaskModel& taskModel;
    TagModel& tagModel;
    std::unique_ptr<AddTaskDialog> addTaskDialog;
    std::unique_ptr<TaskItemDelegate> taskItemDelegate
        = std::make_unique<TaskItemDelegate>();
    const QSize desiredSize{300, 200};
    QPointer<TaskSprintsView> taskSprintsView;
    std::unique_ptr<HistoryModel> taskSprintsModel;
    std::unique_ptr<HistoryItemDelegate> taskSprintViewDelegate
        = std::make_unique<HistoryItemDelegate>();

    void launchTagEditor();
    void removeTask();
    void launchTaskEditor();
    void showSprintsForTask();
    void onSprintsForTaskFetched(const std::vector<entities::Sprint>&);

signals:
    void taskSelected(int row);

private slots:
    void onAddTaskButtonPushed();
    void onQuickAddTodoReturnPressed();
    void toggleTaskCompleted();
    void showContextMenu(const QPoint& pos);
    void addNewTask();
};

} // namespace sprint_timer::ui::qt_gui


#endif // TASKOUTLINE_H
