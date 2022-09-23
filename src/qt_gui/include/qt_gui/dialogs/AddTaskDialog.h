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
#ifndef ADDTASKDIALOG_H_TBSYZEDZ
#define ADDTASKDIALOG_H_TBSYZEDZ

#include "qt_gui/dialogs/TaskDialog.h"
#include "qt_gui/presentation/AddTaskDialogContract.h"

namespace sprint_timer::ui::qt_gui {

/* Provides front end to create or edit Task. */
class AddTaskDialog : public TaskDialog,
                      public contracts::AddTaskDialogContract::View {

public:
    explicit AddTaskDialog(dw::Weekday firstDayOfWeek,
                           QWidget* parent = nullptr);

    auto accept() -> void override;

    auto fillParentData(const std::optional<std::string>& parentUuid,
                        const std::optional<int64_t>& insertBeforePosition)
        -> void override;

    auto fillTags(std::span<const std::string> tags) -> void override;

private:
    std::optional<std::string> parent;
    std::optional<int64_t> insertBeforePos;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: ADDTASKDIALOG_H_TBSYZEDZ */
