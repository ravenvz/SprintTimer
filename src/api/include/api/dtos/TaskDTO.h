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
#ifndef TASKDTO_H_SZ7KJM8B
#define TASKDTO_H_SZ7KJM8B

#include "api/dtos/NoteDTO.h"
#include "api/dtos/TaskTimeframeDTO.h"
#include "api/dtos/TaskTypeDTO.h"
#include "cpp_utils/algorithms/optional_ext.h"
#include "date_wrapper/date_wrapper.h"
#include <optional>
#include <string>
#include <vector>

namespace sprint_timer::api {

struct TaskDTO {
    std::string uuid;
    std::vector<std::string> tags;
    std::string name;
    int expectedCost{0};
    std::vector<dw::DateTimeRange> sprints;
    bool finished{false};
    dw::DateTime modificationStamp{dw::current_date_time_local()};
    std::optional<NoteDTO> notes{std::nullopt};
    TaskTimeframeDTO timeFrame;
    TaskTypeDTO kind{TaskTypeDTO::Regular};

    friend auto operator==(const TaskDTO&, const TaskDTO&) -> bool = default;
};

template <class CharT, class Traits>
auto operator<<(std::basic_ostream<CharT, Traits>& os, const TaskDTO& task)
    -> std::basic_ostream<CharT, Traits>&
{
    os << "TaskDTO{" << task.uuid << ", ";
    for (const auto& element : task.tags) {
        os << '#' << element << ' ';
    }
    os << task.name << ", ";
    os << task.sprints.size() << '/' << task.expectedCost << ", ";
    os << (task.finished ? "finished, " : "pending, ");
    os << "type: " << static_cast<int>(task.kind) << ", ";
    alg::inspect(task.notes,
                 [&](const auto& note) { os << '"' << note.text << "\", "; });
    os << " start: " << task.timeFrame.start << ", due: ";
    if (auto due = task.timeFrame.due; due) {
        os << *due;
    }
    else {
        os << "unlimited ";
    }
    // os << frame.due.value_or(std::string{"unlimited"});
    alg::inspect(task.timeFrame.remindAt, [&](const auto& reminder) {
        os << " reminder: " << reminder << ", ";
    });
    alg::inspect(task.timeFrame.recurrence, [&](const auto& recurrence) {
        os << " recurrence: " << '"' << recurrence << "\", ";
    });
    os << task.modificationStamp << '}';
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: TASKDTO_H_SZ7KJM8B */
