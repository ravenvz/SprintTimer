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
#ifndef CHANGEUNFINISHEDTASKSPRIORITYCOMMAND_H_HPVUFTQ8
#define CHANGEUNFINISHEDTASKSPRIORITYCOMMAND_H_HPVUFTQ8

#include "core/Command.h"
#include <string>
#include <vector>

namespace sprint_timer::use_cases {

struct ChangeActiveTasksPriorityCommand : public Command {
    ChangeActiveTasksPriorityCommand(std::vector<std::string> oldOrder,
                                     std::vector<std::string> newOrder);

    std::vector<std::string> oldOrder;
    std::vector<std::string> newOrder;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const ChangeActiveTasksPriorityCommand&)
{
    os << "ChangeActiveTasksPriorityCommand{}";
    return os;
}

} // namespace sprint_timer::use_cases

#endif /* end of include guard:                                                \
          CHANGEUNFINISHEDTASKSPRIORITYCOMMAND_H_HPVUFTQ8 */

