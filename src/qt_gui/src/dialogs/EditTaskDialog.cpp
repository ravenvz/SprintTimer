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
#include "qt_gui/dialogs/EditTaskDialog.h"
#include "qt_gui/metatypes/TaskDTOMetatype.h"
#include "qt_gui/models/CustomRoles.h"

namespace sprint_timer::ui::qt_gui {

EditTaskDialog::EditTaskDialog(
    QAbstractItemModel& tagModel_,
    QAbstractItemModel& taskModel_,
    const TaskSelectionContext& taskSelectionContext_,
    QWidget* parent_)
    : TaskDialog{tagModel_, parent_}
    , taskSelectionContext{taskSelectionContext_}
    , taskModel{taskModel_}
{
    const auto ind = taskSelectionContext.taskIndex();
    if (!ind) {
        return;
    }
    const auto modelIndex = taskModel.index(*ind, 0);
    const auto item = taskModel.data(modelIndex, CustomRoles::ItemRole);
    const auto task = item.value<TaskDTO>();

    fillFormFields(task);
    setWindowTitle("Edit task");
}

void EditTaskDialog::accept()
{
    if (nameIsEmpty()) {
        markNameFieldRed();
        return;
    }
    const auto selectedTask = taskSelectionContext.taskIndex();
    if (!selectedTask) {
        return;
    }
    TaskDTO editedTask = parseFormFields();
    QVariant var;
    var.setValue(editedTask);
    taskModel.setData(
        taskModel.index(*selectedTask, 0), var, CustomRoles::ReplaceRole);
    QDialog::accept();
}

} // namespace sprint_timer::ui::qt_gui
