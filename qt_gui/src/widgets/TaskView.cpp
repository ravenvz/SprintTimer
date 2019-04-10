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

#include "qt_gui/widgets/TaskView.h"
#include "qt_gui/dialogs/AddTaskDialog.h"
#include "qt_gui/models/TaskModel.h"
#include "qt_gui/utils/MouseRightReleaseEater.h"
#include "qt_gui/utils/WidgetUtils.h"
#include "qt_gui/widgets/TagEditor.h"
#include "qt_gui/widgets/TaskSprintsView.h"
#include <QMenu>
#include <core/use_cases/RequestSprintsForTask.h>

namespace {

const QString editTaskDialogTitle{"Edit task"};

} // namespace

namespace sprint_timer::ui::qt_gui {

using entities::Sprint;
using entities::Task;
using use_cases::RequestSprintsForTask;

TaskView::TaskView(TaskModel& taskModel,
                   ISprintStorageReader& sprintReader,
                   QueryInvoker& queryInvoker,
                   TaskSprintsView& sprintsForTaskView,
                   AddTaskDialog& editTaskDialog,
                   std::unique_ptr<TagEditor> tagEditor,
                   QStyledItemDelegate& delegate,
                   QWidget* parent)
    : ReordableListView{parent}
    , taskModel{taskModel}
    , sprintReader{sprintReader}
    , queryInvoker{queryInvoker}
    , sprintsForTaskView{sprintsForTaskView}
    , editTaskDialog{editTaskDialog}
    , tagEditor{tagEditor.release()}
{
    setModel(&taskModel);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setItemDelegate(&delegate);

    connect(this,
            &QListView::customContextMenuRequested,
            this,
            &TaskView::showContextMenu);
    connect(this, &QListView::doubleClicked, [this, &taskModel]() {
        taskModel.toggleCompleted(currentIndex());
    });
    connect(&taskModel,
            &QAbstractItemModel::rowsRemoved,
            this,
            &TaskView::onTaskRemoved);
    connect(this, &QListView::clicked, [&](const QModelIndex& index) {
        selectedRow = index.row();
        emit taskSelected(*selectedRow);
    });
}

std::optional<int> TaskView::currentlySelectedRow() const
{
    return selectedRow;
}

void TaskView::onTaskSelectionChanged(int taskRow)
{
    selectedRow = taskModel.index(taskRow, 0).row();
}

void TaskView::showSprintsForTask() const
{
    const auto task = taskModel.itemAt(currentIndex().row());

    queryInvoker.execute(std::make_unique<RequestSprintsForTask>(
        sprintReader, task.uuid(), [&](const std::vector<Sprint>& sprints) {
            sprintsForTaskView.setData(sprints);
            sprintsForTaskView.show();
        }));
}

void TaskView::showContextMenu(const QPoint& pos) const
{
    QPoint globalPos = mapToGlobal(pos);
    QMenu contextMenu;
    contextMenu.installEventFilter(
        std::make_unique<MouseRightReleaseEater>(&contextMenu).release());
    const auto editEntry = "Edit";
    const auto deleteEntry = "Delete";
    const auto tagEditorEntry = "Tag editor";
    const auto viewSprintsEntry = "View sprints";
    contextMenu.addAction(editEntry);
    contextMenu.addSeparator();
    contextMenu.addAction(deleteEntry);
    contextMenu.addSeparator();
    contextMenu.addAction(tagEditorEntry);
    contextMenu.addSeparator();
    contextMenu.addAction(viewSprintsEntry);

    QAction* selectedEntry = contextMenu.exec(globalPos);

    if (selectedEntry && selectedEntry->text() == editEntry)
        launchTaskEditor();
    if (selectedEntry && selectedEntry->text() == deleteEntry)
        deleteSelectedTask();
    if (selectedEntry && selectedEntry->text() == tagEditorEntry)
        launchTagEditor();
    if (selectedEntry && selectedEntry->text() == viewSprintsEntry)
        showSprintsForTask();
}

void TaskView::launchTaskEditor() const
{
    QModelIndex index = currentIndex();
    const auto itemToEdit = taskModel.itemAt(index.row());

    editTaskDialog.setWindowTitle(editTaskDialogTitle);
    editTaskDialog.fillItemData(itemToEdit);
    connect(&editTaskDialog, &QDialog::accepted, [&]() {
        Task updatedItem = editTaskDialog.constructedTask();
        updatedItem.setActualCost(itemToEdit.actualCost());
        updatedItem.setCompleted(itemToEdit.isCompleted());
        taskModel.replaceItemAt(index.row(), updatedItem);
    });
    editTaskDialog.exec();
    editTaskDialog.disconnect();
}

void TaskView::launchTagEditor() const
{
    if (tagEditor->isVisible())
        WidgetUtils::bringToForeground(tagEditor);
    else
        tagEditor->show();
}

void TaskView::deleteSelectedTask() const { taskModel.remove(currentIndex()); }

void TaskView::onTaskRemoved(const QModelIndex&, int first, int last)
{
    auto deletedRowWasSelected = [&](const std::optional<int>& row) {
        return row && (first <= row && row <= last);
    };
    if (deletedRowWasSelected(selectedRow)) {
        selectedRow = std::nullopt;
        emit taskSelected(-1);
    }
}

} // namespace sprint_timer::ui::qt_gui
