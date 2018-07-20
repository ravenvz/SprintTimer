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
#include "qt_gui/dialogs/ConfirmationDialog.h"
#include "qt_gui/models/HistoryModel.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include "qt_gui/utils/MouseRightReleaseEater.h"
#include "qt_gui/utils/WidgetUtils.h"
#include "ui_task_outline.h"
#include <QMenu>
#include <QStandardItemModel>

namespace {

using sprint_timer::entities::Sprint;
using sprint_timer::ui::qt_gui::HistoryModel;

HistoryModel::HistoryData
transformToHistoryData(const std::vector<Sprint>& sprints);

QString sprintToString(const Sprint&);

} // namespace

namespace sprint_timer::ui::qt_gui {

using namespace entities;

TaskOutline::TaskOutline(ICoreService& coreService,
                         TaskModel& taskModel,
                         TagModel& tagModel,
                         QWidget* parent)
    : QWidget{parent}
    , ui{std::make_unique<Ui::TaskOutline>()}
    , coreService{coreService}
    , taskModel{taskModel}
    , tagModel{tagModel}
{
    ui->setupUi(this);

    ui->lvTaskView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->lvTaskView->setModel(&taskModel);
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
    delete taskSprintsView;
}

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
    addTaskDialog.reset(new AddTaskDialog{tagModel});
    connect(
        &*addTaskDialog, &QDialog::accepted, this, &TaskOutline::addNewTask);
    addTaskDialog->setModal(true);
    addTaskDialog->show();
}

void TaskOutline::addNewTask()
{
    taskModel.insert(addTaskDialog->constructedTask());
}

void TaskOutline::toggleTaskCompleted()
{
    taskModel.toggleCompleted(ui->lvTaskView->currentIndex());
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

    coreService.requestSprintsForTask(task.uuid(),
                                      [&](const std::vector<Sprint>& sprints) {
                                          onSprintsForTaskFetched(sprints);
                                      });
}

void TaskOutline::onSprintsForTaskFetched(const std::vector<Sprint>& sprints)
{

    if (taskSprintsView)
        taskSprintsView->close();
    auto taskSprintsHistory = transformToHistoryData(sprints);
    taskSprintsView = new TaskSprintsView;
    taskSprintsView->setAttribute(Qt::WA_DeleteOnClose);
    taskSprintsModel = std::make_unique<HistoryModel>();
    taskSprintsModel->fill(taskSprintsHistory);
    taskSprintsView->setDelegate(taskSprintViewDelegate.get());
    taskSprintsView->setModel(taskSprintsModel.get());
    taskSprintsView->show();
}

} // namespace sprint_timer::ui::qt_gui

namespace {

HistoryModel::HistoryData
transformToHistoryData(const std::vector<Sprint>& sprints)
{
    using sprint_timer::ui::qt_gui::DateTimeConverter;
    HistoryModel::HistoryData taskSprintsHistory;
    taskSprintsHistory.reserve(sprints.size());
    std::transform(cbegin(sprints),
                   cend(sprints),
                   std::back_inserter(taskSprintsHistory),
                   [](const auto& sprint) {
                       return std::make_pair(
                           DateTimeConverter::qDate(sprint.startTime()),
                           sprintToString(sprint));
                   });
    return taskSprintsHistory;
}

QString sprintToString(const Sprint& sprint)
{
    return QString("%1 - %2 %3 %4")
        .arg(QString::fromStdString(sprint.startTime().toString("hh:mm")))
        .arg(QString::fromStdString(sprint.finishTime().toString("hh:mm")))
        .arg(QString::fromStdString(prefixTags(sprint.tags())))
        .arg(QString::fromStdString(sprint.name()));
}

} // namespace
