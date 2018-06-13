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

#include "widgets/TaskOutline.h"
#include "dialogs/AddTaskDialog.h"
#include "dialogs/ConfirmationDialog.h"
#include "ui_task_outline.h"
#include "utils/MouseRightReleaseEater.h"
#include "utils/WidgetUtils.h"
#include <QMenu>

namespace qt_gui {

TaskOutline::TaskOutline(ICoreService& coreService,
                         TaskModel* taskModel,
                         TagModel* tagModel,
                         QWidget* parent)
    : ui{new Ui::TaskOutline}
    , coreService{coreService}
    , taskModel{taskModel}
    , tagModel{tagModel}
    , QWidget{parent}
{
    ui->setupUi(this);

    ui->lvTaskView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->lvTaskView->setModel(taskModel);
    ui->lvTaskView->setItemDelegate(taskItemDelegate.get());

    connect(ui->pbAddTask,
            &QPushButton::clicked,
            this,
            &TaskOutline::onAddTaskButtonPushed);
    connect(ui->leQuickAddTask,
            &QLineEdit::returnPressed,
            this,
            &TaskOutline::onQuickAddTodoReturnPressed);
    connect(ui->lvTaskView, &QListView::clicked, [&](const QModelIndex& index) {
        emit taskSelected(index.row());
    });
    connect(ui->lvTaskView,
            &QListView::customContextMenuRequested,
            this,
            &TaskOutline::showContextMenu);
    connect(ui->lvTaskView,
            &QListView::doubleClicked,
            this,
            &TaskOutline::toggleTaskCompleted);
}

TaskOutline::~TaskOutline()
{
    delete tagEditor;
    delete ui;
}

void TaskOutline::onQuickAddTodoReturnPressed()
{
    std::string encodedDescription = ui->leQuickAddTask->text().toStdString();
    ui->leQuickAddTask->clear();
    if (!encodedDescription.empty()) {
        Task item{std::move(encodedDescription)};
        taskModel->insert(item);
    }
}

void TaskOutline::onAddTaskButtonPushed()
{
    addTaskDialog.reset(new AddTaskDialog{tagModel});
    connect(
        &*addTaskDialog, &QDialog::accepted, this, &TaskOutline::addNewTask);
    addTaskDialog->setModal(true);
    addTaskDialog->show();
}

void TaskOutline::addNewTask()
{
    taskModel->insert(addTaskDialog->constructedTask());
}

void TaskOutline::toggleTaskCompleted()
{
    taskModel->toggleCompleted(ui->lvTaskView->currentIndex());
}

QSize TaskOutline::sizeHint() const { return desiredSize; }

void TaskOutline::showContextMenu(const QPoint& pos)
{
    QPoint globalPos = mapToGlobal(pos);
    QMenu contextMenu;
    contextMenu.installEventFilter(new MouseRightReleaseEater(&contextMenu));
    const auto editEntry = "Edit";
    const auto deleteEntry = "Delete";
    const auto tagEditorEntry = "Tag editor";
    contextMenu.addAction(editEntry);
    contextMenu.addSeparator();
    contextMenu.addAction(deleteEntry);
    contextMenu.addSeparator();
    contextMenu.addAction(tagEditorEntry);

    QAction* selectedEntry = contextMenu.exec(globalPos);

    if (selectedEntry && selectedEntry->text() == editEntry)
        launchTaskEditor();
    if (selectedEntry && selectedEntry->text() == deleteEntry)
        removeTask();
    if (selectedEntry && selectedEntry->text() == tagEditorEntry)
        launchTagEditor();
}

void TaskOutline::launchTagEditor()
{
    if (!tagEditor) {
        tagEditor = new TagEditor(tagModel);
        tagEditor->show();
    }
    else {
        WidgetUtils::bringToForeground(tagEditor);
    }
}

void TaskOutline::removeTask()
{
    QModelIndex index = ui->lvTaskView->currentIndex();
    const auto task = taskModel->itemAt(index.row());

    if (task.actualCost() == 0) {
        taskModel->remove(index);
        return;
    }

    ConfirmationDialog dialog;
    QString description
        = "WARNING! This task has sprints associated with it "
          "and they will be removed permanently along with this item.";
    dialog.setActionDescription(description);
    if (dialog.exec())
        taskModel->remove(index);
}

void TaskOutline::launchTaskEditor()
{
    QModelIndex index = ui->lvTaskView->currentIndex();
    const auto itemToEdit = taskModel->itemAt(index.row());
    AddTaskDialog dialog{tagModel};
    dialog.setWindowTitle("Edit task");
    dialog.fillItemData(itemToEdit);
    if (dialog.exec()) {
        Task updatedItem = dialog.constructedTask();
        updatedItem.setActualCost(itemToEdit.actualCost());
        updatedItem.setCompleted(itemToEdit.isCompleted());
        taskModel->replaceItemAt(index.row(), updatedItem);
    }
}

} // namespace qt_gui
