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

#include <QDebug>

namespace {

const QString addTaskDialogTitle{"Add new task"};

std::vector<sprint_timer::entities::Sprint>
buildFromIntervals(const std::string& taskUuid,
                   const std::vector<dw::DateTimeRange>& intervals);

} // namespace

namespace sprint_timer::ui::qt_gui {

using namespace entities;

TaskOutline::TaskOutline(CommandHandler<use_cases::RegisterSprintBulkCommand>&
                             registerSprintBulkHandler_,
                         std::unique_ptr<QAbstractItemView> taskView_,
                         AddTaskDialog& addTaskDialog_,
                         QWidget* parent_)
    : QWidget{parent_}
    , registerSprintBulkHandler{registerSprintBulkHandler_}
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
    const QModelIndex index = taskView->currentIndex();
    if (!index.isValid())
        return;
    const auto taskUuid =
        index.data(static_cast<int>(TaskModelRoles::GetIdRole))
            .toString()
            .toStdString();
    registerSprintBulkHandler.handle(use_cases::RegisterSprintBulkCommand{
        buildFromIntervals(taskUuid, intervals)});
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

} // namespace sprint_timer::ui::qt_gui

namespace {

std::vector<sprint_timer::entities::Sprint>
buildFromIntervals(const std::string& taskUuid,
                   const std::vector<dw::DateTimeRange>& intervals)
{
    std::vector<sprint_timer::entities::Sprint> sprints;
    sprints.reserve(intervals.size());

    std::transform(
        intervals.cbegin(),
        intervals.cend(),
        std::back_inserter(sprints),
        [&taskUuid](const auto& interval) {
            return sprint_timer::entities::Sprint{taskUuid, interval};
        });

    return sprints;
}

} // namespace
