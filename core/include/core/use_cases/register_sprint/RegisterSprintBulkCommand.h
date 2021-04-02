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
#ifndef REGISTERSPRINTBULKCOMMAND_H_E07Q6KCM
#define REGISTERSPRINTBULKCOMMAND_H_E07Q6KCM

#include "core/Command.h"
#include "core/entities/Sprint.h"

namespace sprint_timer::use_cases {

struct RegisterSprintBulkCommand : public Command {
    explicit RegisterSprintBulkCommand(std::vector<entities::Sprint>&& sprints);

    const std::vector<entities::Sprint> sprints;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const RegisterSprintBulkCommand& command)
{
    os << "Register sprint bulk:\n";
    const auto& sprints = command.sprints;
    std::copy(sprints.cbegin(),
              sprints.cend(),
              std::ostream_iterator<entities::Sprint>(os, "\n"));
    os << "}\n";
    return os;
}

bool operator==(const RegisterSprintBulkCommand& lhs,
                const RegisterSprintBulkCommand& rhs);

} // namespace sprint_timer::use_cases

#endif /* end of include guard: REGISTERSPRINTBULKCOMMAND_H_E07Q6KCM */
