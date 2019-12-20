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
#include "qt_gui/widgets/TaskOutline.h"
#include "qt_gui/dialogs/AddTaskDialog.h"
#include "qt_gui/metatypes/TaskMetaType.h"
#include "qt_gui/models/TaskModelRoles.h"
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

TaskOutline::TaskOutline(QAbstractItemModel& taskModel_,
                         SprintModel& sprintModel_,
                         std::unique_ptr<TaskView> taskView_,
                         AddTaskDialog& addTaskDialog_,
                         QWidget* parent_)
    : QWidget{parent_}
    , taskModel{taskModel_}
    , sprintModel{sprintModel_}
    , taskView{taskView_.release()}
    , addTaskDialog{addTaskDialog_}
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
    quickAddTask_->setPlaceholderText("QuickAdd task #tag *estimate");
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
        const Task item{std::move(encodedDescription)};
        insertTask(item);
    }
}

void TaskOutline::onAddTaskButtonPushed()
{
    connect(&addTaskDialog, &QDialog::accepted, [&]() {
        insertTask(addTaskDialog.constructedTask());
    });
    addTaskDialog.setWindowTitle(addTaskDialogTitle);
    addTaskDialog.exec();
    addTaskDialog.disconnect();
}

void TaskOutline::onSprintSubmissionRequested(
    const std::vector<dw::DateTimeRange>& intervals)
{
    if (!taskView->currentlySelectedRow())
        return;
    std::vector<Sprint> sprints;
    sprints.reserve(intervals.size());

    const auto taskUuid = taskModel.index(*taskView->currentlySelectedRow(), 0)
                              .data(static_cast<int>(TaskModelRoles::GetIdRole))
                              .toString()
                              .toStdString();
    std::transform(intervals.cbegin(),
                   intervals.cend(),
                   std::back_inserter(sprints),
                   [&taskUuid](const auto& interval) {
                       return Sprint{taskUuid, interval};
                   });
    sprintModel.insert(sprints);
}

void TaskOutline::insertTask(const Task& task)
{
    QVariant var;
    var.setValue(task);
    taskModel.insertRow(taskModel.rowCount());
    taskModel.setData(taskModel.index(taskModel.rowCount() - 1, 0),
                      var,
                      static_cast<int>(TaskModelRoles::Insert));
}

} // namespace sprint_timer::ui::qt_gui
