/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include "qt_gui/StandaloneDisplayable.h"
#include "qt_gui/dialogs/DisplayableDialog.h"
#include "qt_gui/metatypes/TaskDTOMetatype.h"
#include "qt_gui/models/CustomRoles.h"
#include "qt_gui/utils/MouseRightReleaseEater.h"
#include <QMenu>

namespace sprint_timer::ui::qt_gui {

TaskView::TaskView(StandaloneDisplayable& sprintsForTaskView_,
                   Displayable& editTaskDialog_,
                   StandaloneDisplayable& tagEditor_,
                   QAbstractItemModel& taskModel_,
                   QStyledItemDelegate& delegate_,
                   QWidget* parent_)
    : ReordableListView{parent_}
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
        utils::inspect(presenter(), [&](auto* presenter) {
            const auto var =
                model()->data(currentIndex(), CustomRoles::ItemRole);
            presenter->changeTaskSelection(var.value<api::TaskDTO>());
        });
    });
    setWordWrap(true);
    setVerticalScrollMode(ScrollMode::ScrollPerPixel);
    setItemDelegate(&delegate_);
    setContextMenuPolicy(Qt::CustomContextMenu);
}

void TaskView::selectTask(const std::optional<std::string>& uuid)
{
    // TODO repeats TimerView::SelectTask
    auto findIndex = [&](const std::string& id) {
        const auto matches = model()->match(model()->index(0, 0),
                                            CustomRoles::IdRole,
                                            QString::fromStdString(id));
        return matches.isEmpty() ? QModelIndex{} : matches.front();
    };
    setCurrentIndex(uuid.transform(findIndex).value_or(QModelIndex{}));
}

void TaskView::showContextMenu(const QPoint& pos) const
{
    QPoint globalPos = mapToGlobal(pos);
    QMenu contextMenu;
    contextMenu.installEventFilter(
        std::make_unique<MouseRightReleaseEater>(&contextMenu).release());

    auto* editAction = contextMenu.addAction("Edit");
    contextMenu.addSeparator();
    auto* deleteAction = contextMenu.addAction("Delete");
    contextMenu.addSeparator();
    auto* launchTagEditorAction = contextMenu.addAction("Launch Tag Editor");
    contextMenu.addSeparator();
    auto* displaySprintsAction = contextMenu.addAction("View Sprints");

    QAction* selectedEntry = contextMenu.exec(globalPos);

    if (!selectedEntry) {
        return;
    }

    if (selectedEntry == editAction) {
        launchTaskEditor();
    }
    if (selectedEntry == deleteAction) {
        deleteSelectedTask();
    }
    if (selectedEntry == launchTagEditorAction) {
        launchTagEditor();
    }
    if (selectedEntry == displaySprintsAction) {
        showSprintsForTask();
    }
}

void TaskView::launchTaskEditor() const { editTaskDialog.display(); }

void TaskView::deleteSelectedTask() const
{
    model()->removeRow(currentIndex().row());
}

void TaskView::launchTagEditor() const { tagEditor.display(); }

void TaskView::showSprintsForTask() const { sprintsForTaskView.display(); }

} // namespace sprint_timer::ui::qt_gui
