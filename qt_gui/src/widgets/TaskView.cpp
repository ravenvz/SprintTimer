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
#include "qt_gui/metatypes/TaskMetaType.h"
#include "qt_gui/models/TaskModelRoles.h"
#include "qt_gui/utils/MouseRightReleaseEater.h"
#include <QMenu>

namespace sprint_timer::ui::qt_gui {

TaskView::TaskView(contracts::TaskViewContract::Presenter& presenter_,
                   StandaloneDisplayable& sprintsForTaskView_,
                   Displayable& editTaskDialog_,
                   StandaloneDisplayable& tagEditor_,
                   QAbstractItemModel& taskModel_,
                   QWidget* parent_)
    : ReordableListView{parent_}
    , presenter{presenter_}
    , sprintsForTaskView{sprintsForTaskView_}
    , editTaskDialog{editTaskDialog_}
    , tagEditor{tagEditor_}
{
    setModel(&taskModel_);
    connect(this,
            &QListView::customContextMenuRequested,
            this,
            &TaskView::showContextMenu);
    connect(this, &QListView::doubleClicked, [this, &taskModel_]() {
        if (currentIndex().row() < model()->rowCount()) {
            taskModel_.setData(
                taskModel_.index(currentIndex().row(), 0),
                QVariant{},
                static_cast<int>(CustomRoles::ToggleCheckedRole));
        }
    });
    connect(this, &QListView::pressed, [this]() {
        const auto var = model()->data(currentIndex(), CustomRoles::IdRole);
        const auto uuid = var.value<QString>();
        presenter.changeTaskSelection(currentIndex().row(), uuid.toStdString());
    });
    setWordWrap(true);
    setVerticalScrollMode(ScrollMode::ScrollPerPixel);
    presenter.attachView(*this);
}

TaskView::~TaskView() { presenter.detachView(*this); }

void TaskView::selectTask(std::optional<size_t> taskIndex)
{
    setCurrentIndex(taskIndex ? model()->index(*taskIndex, 0) : QModelIndex{});
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

    if (!selectedEntry) {
        return;
    }

    if (selectedEntry->text() == editEntry)
        launchTaskEditor();
    if (selectedEntry->text() == deleteEntry)
        deleteSelectedTask();
    if (selectedEntry->text() == tagEditorEntry)
        launchTagEditor();
    if (selectedEntry->text() == viewSprintsEntry)
        showSprintsForTask();
}

void TaskView::launchTaskEditor() const { editTaskDialog.display(); }

void TaskView::deleteSelectedTask() const
{
    model()->removeRow(currentIndex().row());
}

void TaskView::launchTagEditor() const { tagEditor.display(); }

void TaskView::showSprintsForTask() const { sprintsForTaskView.display(); }

} // namespace sprint_timer::ui::qt_gui
