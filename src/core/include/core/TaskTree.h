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
#include "core/TaskType.h"
#include "core/Tree.h"
#include "core/entities/Task.h"
#include "date_wrapper/date_wrapper.h"
#include <vector>

namespace sprint_timer {

struct TaskNode {
    entities::Task task;
    TaskType type{TaskType::Regular};
    std::optional<dw::DateTime> dueTime;
    std::optional<dw::DateTime> reminder;
    std::string notes;

    // TODO use default implementation when task notes are supported in storage
    // friend bool operator==(const TaskNode&, const TaskNode&) = default;
    friend bool operator==(const TaskNode& lhs, const TaskNode& rhs)
    {
        return lhs.task == rhs.task && lhs.type == rhs.type &&
               lhs.dueTime == rhs.dueTime && lhs.reminder == rhs.reminder;
    }
};

using TaskTree = Tree<std::string, TaskNode>;

inline std::vector<TaskNode> immediateTasks(const TaskTree& taskTree)
{
    return taskTree.leaves();
}

inline TaskTree projects(const TaskTree& taskTree) { return TaskTree{}; }

inline void insertTask(TaskTree& taskTree,
                       const TaskNode& taskNode,
                       const std::optional<std::string>& parent = std::nullopt)
{
    taskTree.addChild(taskNode.task.uuid(), taskNode, parent);
}

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
