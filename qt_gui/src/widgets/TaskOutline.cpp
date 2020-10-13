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

namespace {

const QString addTaskDialogTitle{"Add new task"};

} // namespace

namespace sprint_timer::ui::qt_gui {

using namespace entities;

TaskOutline::TaskOutline(
    contracts::UnfinishedTasksContract::Presenter& presenter_,
    std::unique_ptr<QAbstractItemView> taskView_,
    AddTaskDialog& addTaskDialog_,
    QAbstractItemModel& taskModel_,
    QWidget* parent_)
    : QWidget{parent_}
    , presenter{presenter_}
    , taskModel{taskModel_}
    , taskView{taskView_.release()}
    , addTaskDialog{addTaskDialog_}
{
    taskView->setModel(&taskModel);
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

    connect(
        taskView, &QAbstractItemView::clicked, [&](const QModelIndex& index) {
            presenter.onTaskSelectionChanged(*this, index.row());
        });

    setLayout(layout.release());

    presenter.attachView(*this);
}

TaskOutline::~TaskOutline() { presenter.detachView(*this); }

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

void TaskOutline::insertTask(const Task& task)
{
    QVariant var;
    var.setValue(task);
    QAbstractItemModel* model{taskView->model()};
    model->insertRow(model->rowCount());
    model->setData(model->index(model->rowCount() - 1, 0),
                   var,
                   static_cast<int>(TaskModelRoles::Insert));
}

void TaskOutline::displayTasks(const std::vector<entities::Task>& tasks)
{
    auto* model = taskView->model();
    model->removeRows(0, model->rowCount());
    for (const auto& task : tasks) {
        QVariant var;
        var.setValue(task);
        model->insertRow(model->rowCount());
        model->setData(model->index(model->rowCount() - 1, 0),
                       var,
                       static_cast<int>(TaskModelRoles::Insert));
    }
}

void TaskOutline::selectTask(size_t taskIndex)
{
    taskView->setCurrentIndex(taskView->model()->index(taskIndex, 0));
}

} // namespace sprint_timer::ui::qt_gui
