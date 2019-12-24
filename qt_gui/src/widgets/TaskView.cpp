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
#include "qt_gui/metatypes/TaskMetaType.h"
#include "qt_gui/models/TaskModelRoles.h"
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

TaskView::TaskView(QAbstractItemModel& taskModel_,
                   ISprintStorageReader& sprintReader_,
                   QueryInvoker& queryInvoker_,
                   TaskSprintsView& sprintsForTaskView_,
                   AddTaskDialog& editTaskDialog_,
                   std::unique_ptr<QWidget> tagEditor_,
                   QStyledItemDelegate& delegate_,
                   QWidget* parent_)
    : ReordableListView{parent_}
    , sprintReader{sprintReader_}
    , queryInvoker{queryInvoker_}
    , sprintsForTaskView{sprintsForTaskView_}
    , editTaskDialog{editTaskDialog_}
    , tagEditor{std::move(tagEditor_)}
{
    setModel(&taskModel_);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setItemDelegate(&delegate_);

    connect(this,
            &QListView::customContextMenuRequested,
            this,
            &TaskView::showContextMenu);
    connect(this, &QListView::doubleClicked, [this, &taskModel_]() {
        taskModel_.setData(currentIndex(),
                           QVariant{},
                           static_cast<int>(TaskModelRoles::ToggleCompletion));
    });
    connect(&taskModel_,
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
    selectedRow = model()->index(taskRow, 0).row();
}

void TaskView::showSprintsForTask() const
{
    const auto taskUUID = currentIndex()
                              .data(static_cast<int>(TaskModelRoles::GetIdRole))
                              .toString()
                              .toStdString();

    queryInvoker.execute(std::make_unique<RequestSprintsForTask>(
        sprintReader, taskUUID, [&](const std::vector<Sprint>& sprints) {
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
    const QModelIndex index = currentIndex();
    const auto itemToEdit = currentIndex()
                                .data(static_cast<int>(TaskModelRoles::GetItem))
                                .value<Task>();

    editTaskDialog.setWindowTitle(editTaskDialogTitle);
    editTaskDialog.fillItemData(itemToEdit);
    connect(&editTaskDialog, &QDialog::accepted, [&]() {
        Task updatedItem = editTaskDialog.constructedTask();
        updatedItem.setActualCost(itemToEdit.actualCost());
        updatedItem.setCompleted(itemToEdit.isCompleted());
        QVariant var;
        var.setValue(updatedItem);
        model()->setData(index, var, static_cast<int>(TaskModelRoles::Replace));
    });
    editTaskDialog.exec();
    editTaskDialog.disconnect();
}

void TaskView::launchTagEditor() const
{
    if (!tagEditor)
        return;
    if (tagEditor->isVisible())
        WidgetUtils::bringToForeground(tagEditor.get());
    else
        tagEditor->show();
}

void TaskView::deleteSelectedTask() const
{
    model()->removeRow(currentIndex().row());
}

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
