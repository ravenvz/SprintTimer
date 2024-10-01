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

namespace {

const auto id_projection = [](const auto& task) { return task.uuid(); };

} // namespace

namespace sprint_timer::api::actions {

// TODO start with adding uuid generator and dateTimeProvider
ToggleTaskCompleted::ToggleTaskCompleted(
    TaskStorage& taskStorage_,
    std::string uuid_,
    UUIDGenerator& uuidGenerator_,
    const DateTimeProvider& dateTimeProvider_)
    : storage{taskStorage_}
    , uuid{std::move(uuid_)}
    , uuidGenerator{uuidGenerator_}
    , dateTimeProvider{dateTimeProvider_}
    , oldTimeStamp{dateTimeProvider.dateLocalNow()}
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
    oldTimeStamp = task.lastModified();
    if (task.isCompleted() or not task.recurrence()) {
        storage.toggleCompleted(uuid, dateTimeProvider.dateTimeLocalNow());
        return;
    }

    auto taskTree = storage.taskTree();
    auto it = find_by_uuid(taskTree, uuid);
    auto pos = taskTree.position_in_children(it);
    auto parent = taskTree.parent(it);
    parentUuid = parent == taskTree.end()
                     ? std::optional<std::string>{}
                     : std::optional<std::string>{parent->uuid()};
    auto nextTask = task.nextRecurrent(uuidGenerator.generateUUID(),
                                       dateTimeProvider.dateTimeLocalNow());
    alg::inspect(nextTask, [&](const auto& t) {
        addedTasks.push_back(t);
        treeSnapshot = taskTree;
        storage.save(t);
        taskTree.insert(parent, t, ds::DestinationPosition{pos});
        storage.saveTree(taskTree);
    });
    storage.toggleCompleted(uuid, dateTimeProvider.dateTimeLocalNow());
}

auto ToggleTaskCompleted::undo() -> void
{
    storage.toggleCompleted(uuid, oldTimeStamp);
    std::ranges::for_each(
        addedTasks, [&](const auto& id) { storage.remove(id); }, id_projection);
    if (treeSnapshot) {
        storage.saveTree(*treeSnapshot);
    }
    treeSnapshot = std::nullopt;
    addedTasks.clear();
}

auto ToggleTaskCompleted::describe() const -> std::string
{
    std::stringstream ss;
    ss << "Toggle task completion for " << uuid;
    return ss.str();
}

} // namespace sprint_timer::api::actions
