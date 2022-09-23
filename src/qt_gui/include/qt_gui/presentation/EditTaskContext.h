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
#ifndef EDITTASKCONTEXT_H_YUAALUYC
#define EDITTASKCONTEXT_H_YUAALUYC

#include "api/dtos/TaskDTO.h"

namespace sprint_timer::ui {

class EditTaskContext {
public:
    EditTaskContext();

    explicit EditTaskContext(api::TaskDTO&& task);

    [[nodiscard]] auto task() const -> const api::TaskDTO&;

    friend auto operator==(const EditTaskContext&, const EditTaskContext&)
        -> bool = default;

private:
    api::TaskDTO editedTask;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: EDITTASKCONTEXT_H_YUAALUYC */
