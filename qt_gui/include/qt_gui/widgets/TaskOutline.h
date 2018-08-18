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

#include "qt_gui/models/SprintModel.h"
#include "qt_gui/models/TagModel.h"
#include "qt_gui/models/TaskModel.h"
#include "qt_gui/widgets/TagEditor.h"
#include <QPointer>
#include <QWidget>
#include <core/ISprintStorageReader.h>
#include <core/QueryInvoker.h>
#include <memory>
#include <optional>

namespace Ui {
class TaskOutline;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class TaskSprintsView;

/* Responsible for providing user interface for interactive
 * task management, so that user could view current unfinished
 * tasks, add/edit and remove task and tags. */
class TaskOutline : public QWidget {

    Q_OBJECT

public:
    TaskOutline(ISprintStorageReader& sprintReader,
                QueryInvoker& queryInvoker,
                TaskModel& taskModel,
                TagModel& tagModel,
                SprintModel& sprintModel,
                TaskSprintsView& taskSprintsView,
                QWidget* parent = nullptr);

    ~TaskOutline() override;

private:
    std::unique_ptr<Ui::TaskOutline> ui;
    ISprintStorageReader& sprintReader;
    QueryInvoker& queryInvoker;
    QPointer<TagEditor> tagEditor;
    TaskModel& taskModel;
    TagModel& tagModel;
    SprintModel& sprintModel;
    TaskSprintsView& taskSprintsView;
    std::optional<int> selectedTaskRow;

    void launchTagEditor();
    void removeTask();
    void launchTaskEditor();
    void showSprintsForTask();
    void onSprintsForTaskFetched(const std::vector<entities::Sprint>&);

signals:
    void taskSelected(int row);

public slots:
    void onTaskSelectionChanged(int taskRow);
    void
    onSprintSubmissionRequested(const std::vector<dw::TimeSpan>& intervals);

private slots:
    void onAddTaskButtonPushed();
    void onQuickAddTodoReturnPressed();
    void toggleTaskCompleted();
    void showContextMenu(const QPoint& pos);
    void onTaskRemoved(const QModelIndex&, int first, int last);
};

} // namespace sprint_timer::ui::qt_gui


#endif // TASKOUTLINE_H
