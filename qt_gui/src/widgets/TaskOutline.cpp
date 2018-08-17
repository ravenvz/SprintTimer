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
#include "qt_gui/widgets/TaskOutline.h"
#include "qt_gui/delegates/TaskItemDelegate.h"
#include "qt_gui/dialogs/AddTaskDialog.h"
#include "qt_gui/dialogs/ConfirmationDialog.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include "qt_gui/utils/MouseRightReleaseEater.h"
#include "qt_gui/utils/WidgetUtils.h"
#include "qt_gui/widgets/TaskSprintsView.h"
#include "ui_task_outline.h"
#include <QMenu>
#include <QStandardItemModel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <core/use_cases/RequestSprintsForTask.h>

namespace sprint_timer::ui::qt_gui {

using namespace entities;
using use_cases::RequestSprintsForTask;

TaskOutline::TaskOutline(ISprintStorageReader& sprintReader,
                         QueryExecutor& queryExecutor,
                         TaskModel& taskModel,
                         TagModel& tagModel,
                         SprintModel& sprintModel,
                         TaskSprintsView& taskSprintsView,
                         QWidget* parent)
    : QWidget{parent}
    , ui{std::make_unique<Ui::TaskOutline>()}
    , sprintReader{sprintReader}
    , queryExecutor{queryExecutor}
    , taskModel{taskModel}
    , tagModel{tagModel}
    , sprintModel{sprintModel}
    , taskSprintsView{taskSprintsView}
{
    ui->setupUi(this);

    ui->lvTaskView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->lvTaskView->setModel(&taskModel);
    ui->lvTaskView->setItemDelegate(new TaskItemDelegate{this});

    connect(ui->pbAddTask,
            &QPushButton::clicked,
            this,
            &TaskOutline::onAddTaskButtonPushed);
    connect(ui->leQuickAddTask,
            &QLineEdit::returnPressed,
            this,
            &TaskOutline::onQuickAddTodoReturnPressed);
    connect(ui->lvTaskView, &QListView::clicked, [&](const QModelIndex& index) {
        selectedTaskRow = index.row();
        emit taskSelected(*selectedTaskRow);
    });
    connect(ui->lvTaskView,
            &QListView::customContextMenuRequested,
            this,
            &TaskOutline::showContextMenu);
    connect(ui->lvTaskView,
            &QListView::doubleClicked,
            this,
            &TaskOutline::toggleTaskCompleted);
    connect(&taskModel,
            &QAbstractItemModel::rowsRemoved,
            this,
            &TaskOutline::onTaskRemoved);
}

TaskOutline::~TaskOutline() { delete tagEditor; }

void TaskOutline::onQuickAddTodoReturnPressed()
{
    std::string encodedDescription = ui->leQuickAddTask->text().toStdString();
    ui->leQuickAddTask->clear();
    if (!encodedDescription.empty()) {
        Task item{std::move(encodedDescription)};
        taskModel.insert(item);
    }
}

void TaskOutline::onAddTaskButtonPushed()
{
    AddTaskDialog addTaskDialog{tagModel};
    connect(&addTaskDialog, &QDialog::accepted, [&]() {
        taskModel.insert(addTaskDialog.constructedTask());
    });
    addTaskDialog.exec();
}

void TaskOutline::toggleTaskCompleted()
{
    taskModel.toggleCompleted(ui->lvTaskView->currentIndex());
}

void TaskOutline::showContextMenu(const QPoint& pos)
{
    QPoint globalPos = mapToGlobal(pos);
    QMenu contextMenu;
    contextMenu.installEventFilter(new MouseRightReleaseEater(&contextMenu));
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
        removeTask();
    if (selectedEntry && selectedEntry->text() == tagEditorEntry)
        launchTagEditor();
    if (selectedEntry && selectedEntry->text() == viewSprintsEntry)
        showSprintsForTask();
}

void TaskOutline::launchTagEditor()
{
    if (!tagEditor) {
        // No memory leak here as TagEditor has Qt::WA_DeleteOnClose set
        tagEditor = new TagEditor{tagModel};
        tagEditor->show();
    }
    else {
        WidgetUtils::bringToForeground(tagEditor);
    }
}

void TaskOutline::removeTask()
{
    const QModelIndex index = ui->lvTaskView->currentIndex();
    taskModel.remove(index);
}

void TaskOutline::launchTaskEditor()
{
    QModelIndex index = ui->lvTaskView->currentIndex();
    const auto itemToEdit = taskModel.itemAt(index.row());

    AddTaskDialog dialog{tagModel};
    dialog.setWindowTitle("Edit task");
    dialog.fillItemData(itemToEdit);
    if (dialog.exec()) {
        Task updatedItem = dialog.constructedTask();
        updatedItem.setActualCost(itemToEdit.actualCost());
        updatedItem.setCompleted(itemToEdit.isCompleted());
        taskModel.replaceItemAt(index.row(), updatedItem);
    }
}

void TaskOutline::showSprintsForTask()
{
    QModelIndex index = ui->lvTaskView->currentIndex();
    const auto task = taskModel.itemAt(index.row());

    queryExecutor.executeQuery(std::make_unique<RequestSprintsForTask>(
        sprintReader, task.uuid(), [&](const std::vector<Sprint>& sprints) {
            onSprintsForTaskFetched(sprints);
        }));
}

void TaskOutline::onSprintsForTaskFetched(const std::vector<Sprint>& sprints)
{
    taskSprintsView.setData(sprints);
    taskSprintsView.show();
}

void TaskOutline::onTaskSelectionChanged(int row)
{
    selectedTaskRow = taskModel.index(row, 0).row();
}

void TaskOutline::onSprintSubmissionRequested(
    const std::vector<dw::TimeSpan>& intervals)
{
    if (!selectedTaskRow)
        return;
    for (const auto& timeSpan : intervals) {
        sprintModel.insert(timeSpan, taskModel.itemAt(*selectedTaskRow).uuid());
    }
}

void TaskOutline::onTaskRemoved(const QModelIndex&, int first, int last)
{
    if (selectedTaskRow
        && (first <= selectedTaskRow && selectedTaskRow <= last)) {
        selectedTaskRow = std::nullopt;
        emit taskSelected(-1);
    }
}

} // namespace sprint_timer::ui::qt_gui
