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

namespace {

const QString editTaskDialogTitle{"Edit task"};

} // namespace

namespace sprint_timer::ui::qt_gui {

using entities::Sprint;
using entities::Task;

TaskView::TaskView(
    TaskSprintsView& sprintsForTaskView_,
    AddTaskDialog& editTaskDialog_,
    std::unique_ptr<QWidget> tagEditor_,
    IndexChangedReemitter& selectedTaskRowReemitter_,
    QueryHandler<use_cases::SprintsForTaskQuery, std::vector<entities::Sprint>>&
        sprintsForTaskHandler_,
    QWidget* parent_)
    : ReordableListView{parent_}
    , sprintsForTaskView{sprintsForTaskView_}
    , editTaskDialog{editTaskDialog_}
    , tagEditor{std::move(tagEditor_)}
    , sprintsForTaskHandler{sprintsForTaskHandler_}
{
    connect(this,
            &QAbstractItemView::clicked,
            [&selectedTaskRowReemitter_](const QModelIndex& index) {
                selectedTaskRowReemitter_.onRowChanged(index.row());
            });
    connect(&selectedTaskRowReemitter_,
            &IndexChangedReemitter::currentRowChanged,
            this,
            &TaskView::onTaskSelectionChanged);
    connect(this,
            &QListView::customContextMenuRequested,
            this,
            &TaskView::showContextMenu);
    connect(this, &QListView::doubleClicked, [this]() {
        if (!model())
            return;
        model()->setData(currentIndex(),
                         QVariant{},
                         static_cast<int>(TaskModelRoles::ToggleCompletion));
    });
    setWordWrap(true);
    setVerticalScrollMode(ScrollMode::ScrollPerPixel);
}

void TaskView::onTaskSelectionChanged(int taskRow)
{
    // Prevents signal emission if row hasn't changed
    if (currentIndex().row() != taskRow)
        setCurrentIndex(model()->index(taskRow, 0));
}

void TaskView::showSprintsForTask() const
{
    const auto taskUUID = currentIndex()
                              .data(static_cast<int>(TaskModelRoles::GetIdRole))
                              .toString()
                              .toStdString();

    sprintsForTaskView.setData(sprintsForTaskHandler.handle(
        use_cases::SprintsForTaskQuery{std::string{taskUUID}}));
    sprintsForTaskView.show();
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

} // namespace sprint_timer::ui::qt_gui
