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
#ifndef TOGGLEZONECOMMAND_H_KBTALFDN
#define TOGGLEZONECOMMAND_H_KBTALFDN

#include <ostream>

namespace sprint_timer::api {

struct ToggleZoneCommand {
    using Result = void;

    friend auto operator==(const ToggleZoneCommand&,
                           const ToggleZoneCommand&) -> bool = default;
};

template <class CharT, class Traits>
auto operator<<(std::basic_ostream<CharT, Traits>& os,
                const ToggleZoneCommand& /*command*/)
    -> std::basic_ostream<CharT, Traits>&
{
    os << "ToggleZoneCommand{}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: TOGGLEZONECOMMAND_H_KBTALFDN */
