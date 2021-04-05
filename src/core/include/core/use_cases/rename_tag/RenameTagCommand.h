/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#ifndef RENAMETAGCOMMAND_H_NLT2ZXRP
#define RENAMETAGCOMMAND_H_NLT2ZXRP

#include "core/Command.h"
#include <string>

namespace sprint_timer::use_cases {

struct RenameTagCommand : public Command {
    RenameTagCommand(std::string oldName, std::string newName);

    const std::string oldName;
    const std::string newName;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const RenameTagCommand& command)
{
    os << "RenameTagCommand{" << command.oldName << " -> " << command.newName
       << "}";
    return os;
}

} // namespace sprint_timer::use_cases

#endif /* end of include guard: RENAMETAGCOMMAND_H_NLT2ZXRP */
