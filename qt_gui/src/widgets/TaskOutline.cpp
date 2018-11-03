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
#include "qt_gui/dialogs/AddTaskDialog.h"
#include "qt_gui/widgets/TaskView.h"
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>
#include <core/use_cases/RequestSprintsForTask.h>

namespace {

const QString addTaskDialogTitle{"Add new task"};

} // namespace

namespace sprint_timer::ui::qt_gui {

using namespace entities;
using use_cases::RequestSprintsForTask;

TaskOutline::TaskOutline(TaskModel& taskModel,
                         SprintModel& sprintModel,
                         std::unique_ptr<TaskView> taskView_,
                         AddTaskDialog& addTaskDialog,
                         QWidget* parent)
    : QWidget{parent}
    , taskModel{taskModel}
    , sprintModel{sprintModel}
    , taskView{taskView_.release()}
    , addTaskDialog{addTaskDialog}
{
    auto layout = std::make_unique<QVBoxLayout>(this);

    auto addTaskButton = std::make_unique<QPushButton>("Add task");
    connect(addTaskButton.get(),
            &QPushButton::clicked,
            this,
            &TaskOutline::onAddTaskButtonPushed);
    layout->addWidget(addTaskButton.release());

    layout->addWidget(taskView);

    auto quickAddTask_ = std::make_unique<QLineEdit>();
    quickAddTask_->setPlaceholderText("QuickAdd task");
    quickAddTask = quickAddTask_.release();
    connect(quickAddTask,
            &QLineEdit::returnPressed,
            this,
            &TaskOutline::onQuickAddTodoReturnPressed);
    layout->addWidget(quickAddTask);

    setLayout(layout.release());
}

void TaskOutline::onQuickAddTodoReturnPressed()
{
    std::string encodedDescription = quickAddTask->text().toStdString();
    quickAddTask->clear();
    if (!encodedDescription.empty()) {
        Task item{std::move(encodedDescription)};
        taskModel.insert(item);
    }
}

void TaskOutline::onAddTaskButtonPushed()
{
    connect(&addTaskDialog, &QDialog::accepted, [&]() {
        taskModel.insert(addTaskDialog.constructedTask());
    });
    addTaskDialog.setWindowTitle(addTaskDialogTitle);
    addTaskDialog.exec();
    addTaskDialog.disconnect();
}

void TaskOutline::onSprintSubmissionRequested(
    const std::vector<dw::TimeSpan>& intervals)
{
    if (!taskView->currentlySelectedRow())
        return;
    for (const auto& timeSpan : intervals) {
        sprintModel.insert(
            timeSpan,
            taskModel.itemAt(*taskView->currentlySelectedRow()).uuid());
    }
}

} // namespace sprint_timer::ui::qt_gui
