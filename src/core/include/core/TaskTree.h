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
#ifndef TASKTREE_H_MRVCAPBE
#define TASKTREE_H_MRVCAPBE

#include "core/GoalProgress.h"
#include "core/Task.h"
#include "core/TaskType.h"
#include "core/TreeType.h"
#include "date_wrapper/date_wrapper.h"
#include <functional>
#include <iterator>
#include <vector>

namespace sprint_timer {

// class TaskTree {
// public:
//     explicit TaskTree(ds::Tree<Task>&& tree_)
//         : tree{std::move(tree)}
//     {
//     }
//
// private:
//     ds::Tree<Task> tree;
// };

using TaskTree = TreeType<Task>;

inline auto tree_handle = [](auto&& tree) {
    return [&](auto func, auto&&... args) mutable {
        return func(tree, std::forward<decltype(args)>(args)...);
    };
};

inline auto find_by_uuid(const TaskTree& tree, const std::string& uuid)
    -> TaskTree::const_iterator
{
    return std::ranges::find(
        tree, uuid, [](const auto& node) { return node.uuid(); });
}

inline auto find_by_uuid(TaskTree& tree, const std::string& uuid)
    -> TaskTree::iterator
{
    return std::ranges::find_if(
        tree, [&](const auto& node) { return node.uuid() == uuid; });
}

inline auto subtree(const TaskTree& tree, const std::string& uuid) -> TaskTree
{
    return tree.transform(find_by_uuid(tree, uuid), std::identity{});
}

// inline auto immediateTasks(const TaskTree& taskTree) -> std::vector<Task>
// {
//     return taskTree.leaves();
// }

// inline auto projects(const TaskTree& taskTree) -> TaskTree
// {
//     TaskTree subtree;
//     std::optional<std::string> parent;
//     auto fun = [&](const auto& key, const auto& payload) {
//         if (payload.kind() == TaskType::Project) {
//             subtree.addSubtree(taskTree.subTree(key), parent, std::nullopt);
//         }
//     };
//     taskTree.dfs(fun);
//     return subtree;
// }

// inline auto insertTask(TaskTree& taskTree,
//                        const Task& task,
//                        const std::optional<std::string>& parent =
//                        std::nullopt)
//     -> void
// {
//     taskTree.addChild(task.uuid(), task, parent);
// }

// class TaskTree {
// public:
//     void insertTask(const TaskNode& taskNode,
//                     const std::optional<std::string>& parent)
//     {
//         tree.addChild(taskNode.task.uuid(), taskNode, parent);
//     }
//
//     std::vector<TaskNode> immediateTasks() const { return tree.leaves(); }
//
//     TaskTree projects() const { return TaskTree{}; }
//
//     friend bool operator==(const TaskTree&, const TaskTree&) { return true; }
//
// private:
//     Tree<std::string, TaskNode> tree;
// };

} // namespace sprint_timer

#endif /* end of include guard: TASKTREE_H_MRVCAPBE */
