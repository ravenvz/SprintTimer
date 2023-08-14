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
#include "api/actions/CreateTask.h"
#include "api/GatewayException.h"
#include "cpp_utils/algorithms/optional_ext.h"

namespace sprint_timer::api::actions {

CreateTask::CreateTask(TaskStorage& taskStorage_,
                       Task task_,
                       std::optional<std::string> parent_,
                       std::optional<int64_t> beforePosition_)
    : taskStorage{taskStorage_}
    , task{std::move(task_)}
    , parent{std::move(parent_)}
    , beforePosition{beforePosition_}
{
}

auto CreateTask::execute() -> void
{
    auto taskTree = taskStorage.taskTree();
    auto parent_it = parent ? find_by_uuid(taskTree, *parent) : taskTree.end();
    taskTree.insert(parent_it, task, beforePosition.transform([](auto pos) {
        return ds::DestinationPosition{pos};
    }));
    taskStorage.saveTree(taskTree);
    taskStorage.save(task);
}

auto CreateTask::undo() -> void
{
    auto taskTree = taskStorage.taskTree();
    taskTree.erase(find_by_uuid(taskTree, task.uuid()));
    taskStorage.saveTree(taskTree);
    taskStorage.remove(task.uuid());
}

auto CreateTask::describe() const -> std::string
{
    std::stringstream ss;
    ss << "Creating task action: " << task;
    alg::inspect(parent, [&](const auto& uuid) { ss << ", uuid: " << uuid; });
    alg::inspect(beforePosition,
                 [&](auto pos) { ss << ", beforePos: " << pos; });
    return ss.str();
}

} // namespace sprint_timer::api::actions
