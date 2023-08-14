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
#include "api/dtos/TaskTypeDTO.h"
#include "cpp_utils/algorithms/optional_ext.h"
#include <cstdint>
#include <vector>

namespace sprint_timer::api {

struct CreateTaskCommand {

    using Result = void;

    std::string name;
    std::vector<std::string> tags;
    int32_t estimatedCost{0};
    TaskTypeDTO type{TaskTypeDTO::Regular};
    std::optional<std::string> parent;
    std::optional<int64_t> insertBeforePos;
    std::optional<NoteDTO> notes{};
    TaskTimeframeDTO timeFrame{};

    friend auto operator==(const CreateTaskCommand&, const CreateTaskCommand&)
        -> bool = default;
};

template <class CharT, class Traits>
auto operator<<(std::basic_ostream<CharT, Traits>& os,
                const CreateTaskCommand& command)
    -> std::basic_ostream<CharT, Traits>&
{
    os << "CreateTaskCommand{";
    os << command.name << ", ";
    for (const auto& tag : command.tags) {
        os << '#' << tag << ' ';
    }
    os << command.estimatedCost << ", ";

    os << "type: " << static_cast<int>(command.type) << ", ";

    alg::inspect(command.notes,
                 [&](const auto& /* dto */) { os << "notes present, "; });

    alg::inspect(command.parent,
                 [&](const auto& uuid) { os << "parent: " << uuid << ", "; });
    alg::inspect(command.insertBeforePos,
                 [&](auto pos) { os << ", pos: " << pos << ", "; });

    const auto& frame = command.timeFrame;
    os << "start: " << frame.start;
    alg::inspect(frame.due, [&](dw::DateTime due) { os << " due: " << due; });
    alg::inspect(frame.remindAt, [&](const auto& remind) {
        os << " reminder: " << remind << ", ";
    });
    alg::inspect(frame.recurrence, [&](const auto& recurrence) {
        os << " recurrence: " << recurrence;
    });

    os << "}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: CREATETASKCOMMAND_H_21OMMTXC */

