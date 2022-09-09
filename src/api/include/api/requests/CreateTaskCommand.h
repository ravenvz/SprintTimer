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
#ifndef CREATETASKCOMMAND_H_21OMMTXC
#define CREATETASKCOMMAND_H_21OMMTXC

#include "api/dtos/NoteDTO.h"
#include "api/dtos/TaskTimeframeDTO.h"
#include <cstdint>
#include <vector>

namespace sprint_timer::api {

struct CreateTaskCommand {

    using Result = void;

    std::string name;
    std::vector<std::string> tags;
    int32_t estimatedCost{0};
    std::optional<NoteDTO> notes{};
    std::optional<TaskTimeframeDTO> timeFrame{};

    friend bool operator==(const CreateTaskCommand&,
                           const CreateTaskCommand&) = default;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const CreateTaskCommand& command)
{
    os << "CreateTaskCommand{";
    os << command.name << ", ";
    for (const auto& tag : command.tags) {
        os << '#' << tag << ' ';
    }
    os << command.estimatedCost << ", ";
    if (auto notes = command.notes; notes) {
        os << "notes present, ";
    }
    if (auto frame = command.timeFrame; frame) {
        os << frame->frame << " ";
        if (auto remind = frame->remindAt; remind) {
            os << "reminder: " << *remind << " ";
        }
        if (auto recurrence = frame->recurrence; recurrence) {
            os << "recurrence: " << *recurrence;
        }
    }
    os << "}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: CREATETASKCOMMAND_H_21OMMTXC */

