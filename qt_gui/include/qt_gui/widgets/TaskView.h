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
#ifndef TASKVIEW_H_AC0ZCTZN
#define TASKVIEW_H_AC0ZCTZN

#include "qt_gui/widgets/ReordableListView.h"
#include <QStyledItemDelegate>
#include <optional>

#ifdef _MSC_VER
#include "qt_gui/WinExport.h"
#endif // _MSC_VER

namespace sprint_timer::ui::qt_gui {

class TaskSprintsView;
class AddTaskDialog;
class TagEditor;

#ifdef _MSC_VER
class GLIB_EXPORT TaskView : public ReordableListView {
#else
class TaskView : public ReordableListView {
#endif // _MSC_VER
    Q_OBJECT

public:
    TaskView(TaskModel& taskModel,
             ISprintStorageReader& sprintReader,
             QueryInvoker& queryInvoker,
             TaskSprintsView& sprintsForTaskView,
             AddTaskDialog& editTaskDialog,
             std::unique_ptr<TagEditor> tagEditor,
             QStyledItemDelegate& delegate,
             QWidget* parent = nullptr);

    std::optional<int> currentlySelectedRow() const;

public slots:
    void onTaskSelectionChanged(int taskRow);

signals:
    void taskSelected(int taskRow);

private:
    TaskModel& taskModel;
    ISprintStorageReader& sprintReader;
    QueryInvoker& queryInvoker;
    TaskSprintsView& sprintsForTaskView;
    AddTaskDialog& editTaskDialog;
    TagEditor* tagEditor;
    std::optional<int> selectedRow;

    void showSprintsForTask() const;

    void showContextMenu(const QPoint& pos) const;

    void launchTaskEditor() const;

    void launchTagEditor() const;

    void deleteSelectedTask() const;

    void onTaskRemoved(const QModelIndex&, int first, int last);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: TASKVIEW_H_AC0ZCTZN */
