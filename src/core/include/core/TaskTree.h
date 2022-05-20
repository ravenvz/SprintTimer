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
#include "core/Tree.h"
#include "date_wrapper/date_wrapper.h"
#include <functional>
#include <memory>
#include <queue>
#include <ranges>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>

namespace sprint_timer {

enum class TaskType { Project, Folder, Recurring, Regular };

struct TaskNode {
    std::string name;
    std::string uuid;
    GoalProgress progress{GoalProgress::Estimated{0}, GoalProgress::Actual{0}};
    bool completed{false};
    TaskType taskType{TaskType::Regular};
    dw::DateTime lastModified{dw::current_date_time_local()};

    friend bool operator==(const TaskNode&, const TaskNode&) = default;
};

class TaskTree {
public:
    void addChild(TaskNode task, std::optional<std::string> parent)
    {
        tree.addChild(task.uuid, task, std::move(parent));
    }

    std::vector<TaskNode> immediateTasks() const { return tree.leaves(); }

    TaskTree projects() const { return TaskTree{}; }

    friend bool operator==(const TaskTree&, const TaskTree&) { return true; }

    friend bool operator!=(const TaskTree&, const TaskTree&) { return false; }

private:
    Tree<std::string, TaskNode> tree;
};

} // namespace sprint_timer

#endif /* end of include guard: TASKTREE_H_MRVCAPBE */
