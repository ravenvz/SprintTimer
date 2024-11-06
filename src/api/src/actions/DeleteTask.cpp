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
#include "api/actions/DeleteTask.h"

namespace sprint_timer::api::actions {

DeleteTask::DeleteTask(TaskStorage& taskStorage_,
                       SprintStorageWriter& sprintWriter_,
                       Task taskToRemove_)
    : taskStorage{taskStorage_}
    , sprintWriter{sprintWriter_}
    , task{std::move(taskToRemove_)}
{
}

auto DeleteTask::execute() -> void
{
    auto taskTree = taskStorage.taskTree();
    auto it = find_by_uuid(taskTree, task.uuid());
    auto parent_it = taskTree.parent(it);
    parent = parent_it == taskTree.end()
                 ? std::optional<std::string>{}
                 : std::optional<std::string>{parent_it->uuid()};
    position = taskTree.position_in_children(it);
    subTree = taskTree.take_subtree(it);
    taskStorage.saveTree(taskTree);
    // NOTE that sprints are marked as deleted by taskStorage::remove
    std::ranges::for_each(
        subTree, [&](const auto& id) { taskStorage.remove(id); }, &Task::uuid);
}

auto DeleteTask::undo() -> void
{
    auto taskTree = taskStorage.taskTree();
    auto parent_it = parent
                         .transform([&](const auto& uuid) {
                             return find_by_uuid(taskTree, uuid);
                         })
                         .value_or(taskTree.end());
    taskTree.insert_subtree(
        parent_it, subTree, position.transform([](auto pos) {
            return ds::DestinationPosition{pos};
        }));
    taskStorage.saveTree(taskTree);

    // NOTE that for now those two operations are not run in single transaction,
    // that can potentially cause problems. TaskStorage::restore should take
    // (const Task&) as parameter instead of uuid to be able to restore correct
    // sprints.
    std::ranges::for_each(
        subTree, [this](const auto& payload) { taskStorage.restore(payload); });
}

auto DeleteTask::describe() const -> std::string
{
    std::stringstream ss;
    ss << "Delete task '" << task << "'";
    return ss.str();
}

} // namespace sprint_timer::api::actions
