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
#ifndef SUBMITCOMMAND_H_GW3S7HKH
#define SUBMITCOMMAND_H_GW3S7HKH

#include <ostream>

namespace sprint_timer::api {

struct SubmitCommand {
    using Result = void;

    std::string taskUuid;

    friend auto operator==(const SubmitCommand&,
                           const SubmitCommand&) -> bool = default;
};

template <class CharT, class Traits>
auto operator<<(std::basic_ostream<CharT, Traits>& os,
                const SubmitCommand& command)
    -> std::basic_ostream<CharT, Traits>&
{
    os << "SubmitCommand{" << command.taskUuid << "}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: SUBMITCOMMAND_H_GW3S7HKH */
