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
#include "qt_gui/metatypes/TaskDTOMetatype.h"
#include "qt_gui/models/CustomRoles.h"
#include "qt_gui/widgets/TaskView.h"
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>

namespace {

const QString addTaskDialogTitle{"Add new task"};

} // namespace

namespace sprint_timer::ui::qt_gui {

using namespace entities;

TaskOutline::TaskOutline(ui::contracts::AddTaskControl::Presenter& presenter_,
                         std::unique_ptr<QWidget> taskView_,
                         Displayable& addTaskDialog_,
                         QWidget* parent_)
    : QWidget{parent_}
    , presenter{presenter_}
    , addTaskDialog{addTaskDialog_}
{
    auto layout = std::make_unique<QVBoxLayout>(this);

    auto addTaskButton = std::make_unique<QPushButton>("Add task");
    connect(addTaskButton.get(),
            &QPushButton::clicked,
            this,
            &TaskOutline::onAddTaskButtonPushed);
    layout->addWidget(addTaskButton.release());
    layout->addWidget(taskView_.release());

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

TaskOutline::~TaskOutline() { }

void TaskOutline::onQuickAddTodoReturnPressed()
{
    const std::string encodedDescription = quickAddTask->text().toStdString();
    quickAddTask->clear();
    if (!encodedDescription.empty()) {
        presenter.addTask(encodedDescription);
    }
}

void TaskOutline::onAddTaskButtonPushed()
{
    addTaskDialog.display();
    // AddTaskDialog dialog{tagModel};
    // dialog.setWindowTitle(addTaskDialogTitle);
    // if (dialog.exec() == QDialog::Accepted) {
    //     const auto task = dialog.constructedTask();
    //     const auto t = task.tags();
    //     std::vector<std::string> tagStr(t.size(), "");
    //     std::transform(cbegin(t), cend(t), begin(tagStr), [](const auto&
    //     elem) {
    //         return elem.name();
    //     });
    //     ui::contracts::AddTaskControl::TaskDetails details{
    //         task.name(), tagStr, task.estimatedCost(), task.actualCost()};
    //     presenter.onTaskAddConfirmed(details);
    // }
}

} // namespace sprint_timer::ui::qt_gui
