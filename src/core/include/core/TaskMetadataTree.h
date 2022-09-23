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
#ifndef TASKMETADATATREE_H_N9EAZDOO
#define TASKMETADATATREE_H_N9EAZDOO

#include "core/TaskType.h"
#include "core/Tree.h"
#include "date_wrapper/date_wrapper.h"

namespace sprint_timer {

struct TaskMetadata {
    std::string uuid{};
    TaskType taskType{TaskType::Regular};
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const TaskMetadata& data)
{
    auto opt_h = []() { return std::optional<std::string>{"null"}; };
    os << "TaskMetadata{";
    os << "uuid: " << data.uuid << ", ";
    os << "type: " << static_cast<int>(data.taskType) << "}";

    return os;
}

using TaskMetadataTree = Tree<std::string, TaskMetadata>;

} // namespace sprint_timer

#endif /* end of include guard: TASKMETADATATREE_H_N9EAZDOO */

