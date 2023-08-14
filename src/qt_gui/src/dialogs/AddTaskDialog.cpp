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
#include "qt_gui/dialogs/AddTaskDialog.h"
#include "cpp_utils/algorithms/optional_ext.h"

namespace sprint_timer::ui::qt_gui {

AddTaskDialog::AddTaskDialog(dw::Weekday firstDayOfWeek_, QWidget* parent_)
    : TaskDialog{firstDayOfWeek_, parent_}
{
    setWindowTitle("Add new task");
}

auto AddTaskDialog::accept() -> void
{
    if (nameIsEmpty()) {
        markNameFieldRed();
        return;
    }
    alg::inspect(presenter(), [this](auto* presenter) {
        auto taskDto = parseFormFields();
        presenter->onTaskCreationAccepted(std::move(taskDto.name),
                                          std::move(taskDto.tags),
                                          taskDto.expectedCost,
                                          taskDto.kind,
                                          std::move(parent),
                                          insertBeforePos,
                                          std::move(taskDto.notes),
                                          std::move(taskDto.timeFrame));
    });
    QDialog::accept();
}

auto AddTaskDialog::fillParentData(
    const std::optional<std::string>& parentUuid,
    const std::optional<int64_t>& insertBeforePosition) -> void
{
    parent = parentUuid;
    insertBeforePos = insertBeforePosition;
}

auto AddTaskDialog::fillTags(std::span<const std::string> tags) -> void
{
    fillTagField(tags);
}

} // namespace sprint_timer::ui::qt_gui

