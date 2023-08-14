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
#include "qt_gui/dialogs/EditTaskDialog.h"
#include "cpp_utils/algorithms/optional_ext.h"
#include "qt_gui/metatypes/TaskDTOMetatype.h"
#include "qt_gui/models/CustomRoles.h"

namespace sprint_timer::ui::qt_gui {

EditTaskDialog::EditTaskDialog(dw::Weekday firstDayOfWeek_, QWidget* parent_)
    : TaskDialog{firstDayOfWeek_, parent_}
{
    setWindowTitle("Edit task");
}

void EditTaskDialog::accept()
{
    if (nameIsEmpty()) {
        markNameFieldRed();
        return;
    }

    alg::inspect(presenter(), [&](auto* presenter) {
        auto editedTask = parseFormFields();
        presenter->onEditTaskAccepted(parseFormFields());
    });

    QDialog::accept();
}

auto EditTaskDialog::fillTaskDetails(const api::TaskDTO& original) -> void
{
    fillFormFields(original);
}

auto EditTaskDialog::fillTags(std::span<const std::string> tags) -> void
{
    fillTagField(tags);
}

} // namespace sprint_timer::ui::qt_gui
