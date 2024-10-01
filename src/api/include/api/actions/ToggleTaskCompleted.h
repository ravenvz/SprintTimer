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
#ifndef TOGGLETASKCOMPLETED_H_BSLZSITB
#define TOGGLETASKCOMPLETED_H_BSLZSITB

#include "api/DateTimeProvider.h"
#include "api/TaskStorage.h"
#include "api/UUIDGenerator.h"
#include <optional>

namespace sprint_timer::api::actions {

class ToggleTaskCompleted {
public:
    ToggleTaskCompleted(TaskStorage& taskStorage,
                        std::string uuid,
                        UUIDGenerator& uuidGenerator,
                        const DateTimeProvider& dateTimeProvider);

    auto execute() -> void;

    auto undo() -> void;

    [[nodiscard]] auto describe() const -> std::string;

private:
    TaskStorage& storage;
    std::string uuid;
    UUIDGenerator& uuidGenerator;
    const DateTimeProvider& dateTimeProvider;
    dw::DateTime oldTimeStamp;
    std::optional<std::string> recurringTaskUuid;
    std::optional<std::string> parentUuid;
    std::optional<TaskTree> treeSnapshot;
    std::vector<Task> addedTasks;
};

} // namespace sprint_timer::api::actions

#endif /* end of include guard: TOGGLETASKCOMPLETED_H_BSLZSITB */

