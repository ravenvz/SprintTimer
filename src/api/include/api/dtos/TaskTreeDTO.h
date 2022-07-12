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
#ifndef TASKTREEDTO_H_JN159NM6
#define TASKTREEDTO_H_JN159NM6

#include "api/dtos/TaskDTO.h"
#include "core/Tree.h"

namespace sprint_timer::api {

enum class TaskTypeDTO { Project, Folder, Recurring, Regular };

struct TaskNodeDTO {
    TaskDTO task;
    TaskTypeDTO type;
    std::optional<dw::DateTime> dueTime;
    std::optional<dw::DateTime> reminder;
    std::string notes;

    // TODO use default implementation when task notes are supported in storage
    // friend bool operator==(const TaskNodeDTO&, const TaskNodeDTO&) = default;
    friend bool operator==(const TaskNodeDTO& lhs, const TaskNodeDTO& rhs)
    {
        return lhs.task == rhs.task and lhs.type == rhs.type and
               lhs.dueTime == rhs.dueTime and lhs.reminder == rhs.reminder;
    }
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const TaskNodeDTO& taskNode)
{
    os << "TaskNode{";
    using enum TaskTypeDTO;
    switch (taskNode.type) {
    case Project:
        os << "Project";
        break;
    case Folder:
        os << "Folder";
        break;
    case Recurring:
        os << "Recurring";
        break;
    case Regular:
        os << "Regular";
    }
    os << ", ";
    if (auto dt = taskNode.dueTime; dt) {
        os << "due: " << *dt;
    }
    else {
        os << ", null";
    }
    if (auto rmd = taskNode.reminder; rmd) {
        os << ", remind: " << *rmd;
    }
    else {
        os << ", null";
    }
    os << ", notes: " << taskNode.notes << "}";

    return os;
}

using TaskTreeDTO = Tree<std::string, TaskNodeDTO>;

} // namespace sprint_timer::api

#endif /* end of include guard: TASKTREEDTO_H_JN159NM6 */
