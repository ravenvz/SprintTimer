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
#ifndef EXPORTDATACOMMAND_H_3YT9WPSL
#define EXPORTDATACOMMAND_H_3YT9WPSL

#include "core/Command.h"
#include "core/DataExporter.h"
#include <date_wrapper/date_wrapper.h>

namespace sprint_timer::use_cases {

struct ExportSprintsCommand : public Command {
    ExportSprintsCommand(const dw::DateRange& dateRange,
                         DataFormat format,
                         SinkType sinkType);

    dw::DateRange dateRange;
    DataFormat format;
    SinkType sinkType;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const ExportSprintsCommand& command)
{
    os << "ExportSprintsCommand{";
    os << command.dateRange << ", ";
    os << "format: DataFormat " << static_cast<int>(command.format) << ", ";
    os << "sink: SinkType " << static_cast<int>(command.sinkType) << "}";
    return os;
}

} // namespace sprint_timer::use_cases

#endif /* end of include guard: EXPORTDATACOMMAND_H_3YT9WPSL */
