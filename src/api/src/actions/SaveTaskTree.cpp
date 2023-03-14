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
#include "api/actions/SaveTaskTree.h"

namespace sprint_timer::api::actions {

SaveTaskTree::SaveTaskTree(TaskStorage& taskStorage_, TaskTree taskTree_)
    : taskStorage{taskStorage_}
    , taskTree{std::move(taskTree_)}
{
}

auto SaveTaskTree::execute() -> void
{
    auto previousTree = taskStorage.taskTree();
    taskStorage.saveTree(taskTree);
    taskTree = std::move(previousTree);
}

auto SaveTaskTree::undo() -> void { taskStorage.saveTree(taskTree); }

auto SaveTaskTree::describe() const -> std::string
{
    return "Saving task tree";
}

} // namespace sprint_timer::api::actions

