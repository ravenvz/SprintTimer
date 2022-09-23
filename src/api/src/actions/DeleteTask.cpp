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

DeleteTask::DeleteTask(TaskStorage& taskStorage_, Task taskToRemove_)
    : taskStorage{taskStorage_}
    , task{std::move(taskToRemove_)}
{
}

auto DeleteTask::execute() -> void
{
    auto taskTree = taskStorage.taskTree();
    parent = taskTree.parent(task.uuid());
    position = taskTree.positionInChildren(task.uuid());
    subTree = taskTree.subTree(task.uuid());
    taskTree.removeNode(task.uuid());
    taskStorage.saveTree(taskTree);
    subTree.dfs([&](const auto& uuid, const auto& /*task*/) {
        taskStorage.remove(uuid);
    });
}

auto DeleteTask::undo() -> void
{
    auto taskTree = taskStorage.taskTree();
    taskTree.addSubtree(subTree, parent, position);
    taskStorage.saveTree(taskTree);
    subTree.dfs([&](const auto& /*key*/, const auto& payload) {
        taskStorage.save(payload);
    });
}

auto DeleteTask::describe() const -> std::string
{
    std::stringstream ss;
    ss << "Delete task '" << task << "'";
    return ss.str();
}

} // namespace sprint_timer::api::actions
