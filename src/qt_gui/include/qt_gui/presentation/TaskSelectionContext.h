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
#ifndef TASKSELECTIONCONTEXT_H_4NHWBLM6
#define TASKSELECTIONCONTEXT_H_4NHWBLM6

#include "api/dtos/TaskDTO.h"
#include <optional>
#include <string>

namespace sprint_timer::ui {

class TaskSelectionContext {
public:
    virtual ~TaskSelectionContext() = default;

    [[nodiscard]] virtual auto taskUuid() const -> std::optional<std::string> = 0;

    [[nodiscard]] virtual auto currentSelection() const
        -> const std::optional<api::TaskDTO>& = 0;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: TASKSELECTIONCONTEXT_H_4NHWBLM6 */
