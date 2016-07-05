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
#include "TaskView.h"
#include <QDebug>

TaskView::TaskView(QWidget* parent)
    : QListView(parent)
{
    taskViewDelegate = new TaskViewDelegate(this);
    setItemDelegate(taskViewDelegate);

    connect(this,
            &QListView::customContextMenuRequested,
            this,
            &TaskView::showTaskContextMenu);
}

void TaskView::dropEvent(QDropEvent* event)
{
    int rowMovedFrom = currentIndex().row();
    int rowMovedTo = indexAt(event->pos()).row();
    model()->moveRows(
        QModelIndex(), rowMovedFrom, 1, QModelIndex(), rowMovedTo);
}

void TaskView::setModels(TaskModel* taskModel, TagModel* tagModel)
{
    setModel(taskModel);
    this->tagModel = tagModel;
}

void TaskView::editTask()
{
    if (tagModel == nullptr) {
        // TODO log error
        qDebug() << "Passing null tagModel to TaskView";
        return;
    }
    QModelIndex index = currentIndex();
    // TODO provide proper implementation of data() and setData() in
    // TaskModel, so that dynamic casts like that could be avoided
    const auto itemToEdit
        = dynamic_cast<TaskModel*>(model())->itemAt(index.row());
    AddTaskDialog dialog{tagModel};
    dialog.setWindowTitle("Edit Task");
    dialog.fillItemData(itemToEdit);
    if (dialog.exec()) {
        Task updatedItem = dialog.constructedTask();
        updatedItem.setSpentPomodoros(itemToEdit.spentPomodoros());
        updatedItem.setCompleted(itemToEdit.isCompleted());
        dynamic_cast<TaskModel*>(model())->replaceItemAt(index.row(),
                                                         updatedItem);
    }
}

void TaskView::removeTask()
{
    QModelIndex index = currentIndex();
    const auto task = dynamic_cast<TaskModel*>(model())->itemAt(index.row());
    ConfirmationDialog dialog;
    QString description;
    if (task.spentPomodoros() > 0) {
        description
            = "WARNING! This todo item has pomodoros associated with it "
              "and they will be removed permanently along with this item.";
    }
    else {
        description = "This will delete todo item permanently!";
    }
    dialog.setActionDescription(description);
    if (dialog.exec()) {
        dynamic_cast<TaskModel*>(model())->remove(index);
    }
}

void TaskView::launchTagEditor()
{
    if (!tagEditor) {
        tagEditor = new TagEditorWidget{tagModel};
        tagEditor->show();
    }
    else {
        tagEditor->raise();
        tagEditor->activateWindow();
        tagEditor->showNormal();
    }
}

void TaskView::showTaskContextMenu(const QPoint& pos)
{
    QPoint globalPos = mapToGlobal(pos);

    QMenu taskMenu;
    // Note QMenu takes ownership of Action
    taskMenu.addAction("Edit");
    taskMenu.addAction("Delete");
    taskMenu.addAction("Tag editor");

    QAction* selectedItem = taskMenu.exec(globalPos);

    if (selectedItem && selectedItem->text() == "Edit")
        editTask();
    if (selectedItem && selectedItem->text() == "Delete")
        removeTask();
    if (selectedItem && selectedItem->text() == "Tag editor")
        launchTagEditor();
}
