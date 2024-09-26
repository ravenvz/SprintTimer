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

#include <utility>

#include "api/actions/ToggleTaskCompleted.h"

using dw::DateTime;

namespace sprint_timer::api::actions {

// TODO start with adding uuid generator and dateTimeProvider
ToggleTaskCompleted::ToggleTaskCompleted(
    TaskStorage& taskStorage_,
    std::string uuid_,
    dw::DateTime lastModified_,
    UUIDGenerator& uuidGenerator_,
    const DateTimeProvider& dateTimeProvider_)
    : storage{taskStorage_}
    , uuid{std::move(uuid_)}
    , oldTimeStamp{lastModified_}
    , uuidGenerator{uuidGenerator_}
    , dateTimeProvider{dateTimeProvider_}
{
}

auto ToggleTaskCompleted::execute() -> void
{
    auto matchingUuid = storage.findByUuid(uuid);
    if (matchingUuid.empty()) {
        // TODO throw proper exception
        throw std::runtime_error{"Can't find task with given uuid"};
    }
    const auto task = matchingUuid.front();
    if (task.isCompleted()) {
        storage.toggleCompleted(uuid, dateTimeProvider.dateTimeLocalNow());
        return;
    }
    if (not task.recurrence()) {
        storage.toggleCompleted(uuid, dateTimeProvider.dateTimeLocalNow());
        return;
    }

    auto taskTree = storage.taskTree();
    auto it = find_by_uuid(taskTree, uuid);
    auto parent = taskTree.parent(it);
    parentUuid = parent == taskTree.end()
                     ? std::optional<std::string>{}
                     : std::optional<std::string>{parent->uuid()};
    auto nextTask = task.nextRecurrence(uuidGenerator.generateUUID(),
                                        dateTimeProvider.dateTimeLocalNow());
    auto pos = taskTree.position_in_children(it);
    if (nextTask) {
        storage.save(*nextTask);
        taskTree.insert(parent, *nextTask, ds::DestinationPosition{pos});
        storage.saveTree(taskTree);
    }
    storage.toggleCompleted(uuid, dateTimeProvider.dateTimeLocalNow());
}

auto ToggleTaskCompleted::undo() -> void
{
    storage.toggleCompleted(uuid, oldTimeStamp);
}

auto ToggleTaskCompleted::describe() const -> std::string
{
    std::stringstream ss;
    ss << "Toggle task completion for " << uuid;
    return ss.str();
}

} // namespace sprint_timer::api::actions
