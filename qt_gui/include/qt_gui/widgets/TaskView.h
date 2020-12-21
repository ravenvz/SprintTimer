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
#ifndef TASKVIEW_H_AC0ZCTZN
#define TASKVIEW_H_AC0ZCTZN

#include "qt_gui/presentation/UnfinishedTasksContract.h"
#include "qt_gui/widgets/ReordableListView.h"
#include <optional>

namespace sprint_timer::ui::qt_gui {

class TaskSprintsView;
class AddTaskDialog;
class TagEditor;

class TaskView : public ReordableListView,
                 public contracts::UnfinishedTasksContract::View {
    Q_OBJECT

public:
    TaskView(contracts::UnfinishedTasksContract::Presenter& presenter,
             TaskSprintsView& sprintsForTaskView,
             AddTaskDialog& editTaskDialog,
             std::unique_ptr<QWidget> tagEditor,
             QAbstractItemModel& taskModel,
             QWidget* parent = nullptr);

    ~TaskView() override;

    void displayTasks(const std::vector<entities::Task>& tasks) override;

    void selectTask(size_t taskIndex) override;

private:
    contracts::UnfinishedTasksContract::Presenter& presenter;
    TaskSprintsView& sprintsForTaskView;
    AddTaskDialog& editTaskDialog;
    std::unique_ptr<QWidget> tagEditor;

    void showSprintsForTask() const;

    void showContextMenu(const QPoint& pos) const;

    void launchTaskEditor() const;

    void launchTagEditor() const;

    void deleteSelectedTask() const;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: TASKVIEW_H_AC0ZCTZN */
