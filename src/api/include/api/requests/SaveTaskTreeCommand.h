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
#ifndef SAVETASKTREE_H_N4GBFEPQ
#define SAVETASKTREE_H_N4GBFEPQ

#include "api/dtos/TaskTreeDTO.h"

namespace sprint_timer::api {

struct SaveTaskTreeCommand {

    using Result = void;

    TaskTreeDTO taskTree;

    friend bool operator==(const SaveTaskTreeCommand&,
                           const SaveTaskTreeCommand&) = default;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const SaveTaskTreeCommand& command)
{
    // os << "SaveTaskTreeCommand{**tree omitted from output**}";
    os << "SaveTaskTreeCommand{" << command.taskTree << "}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: SAVETASKTREE_H_N4GBFEPQ */
