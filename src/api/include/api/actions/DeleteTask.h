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
#ifndef DELETETASK_H_07POXQ41
#define DELETETASK_H_07POXQ41

#include "api/Action.h"
#include "api/SprintStorage.h"
#include "api/TaskStorage.h"

namespace sprint_timer::api::actions {

class DeleteTask : public Action {
public:
    DeleteTask(TaskStorage& taskStorage, Task taskToRemove);

    auto execute() -> void final;

    auto undo() -> void final;

    [[nodiscard]] auto describe() const -> std::string final;

private:
    TaskStorage& taskStorage;
    Task task;
    TaskTree subTree;
    std::optional<std::string> parent;
    std::optional<int64_t> position;
};

} // namespace sprint_timer::api::actions

#endif /* end of include guard: DELETETASK_H_07POXQ41 */

