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
#include "qt_gui/presentation/TaskSelectionMediator.h"

namespace sprint_timer::ui {

auto TaskSelectionMediator::changeSelection(TaskSelectionColleague* caller,
                                            std::optional<api::TaskDTO>&& task)
    -> void
{
    selection = std::move(task);
    mediate(caller,
            [](auto* colleague) { colleague->onTaskSelectionChanged(); });
}

auto TaskSelectionMediator::currentSelection() const
    -> const std::optional<api::TaskDTO>&
{
    return selection;
}

auto TaskSelectionMediator::taskUuid() const -> std::optional<std::string>
{
    return utils::transform(selection,
                            [](const auto& task) { return task.uuid; });
}

} // namespace sprint_timer::ui
