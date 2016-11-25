/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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

#include "core/external_io/CSVWriter.h"

namespace ExternalIO {

namespace CSV {

CSVWriter::CSVWriter(char delimiter)
    : delimiter{delimiter}
{
}

void CSVWriter::exportData(ExternalIO::ISink* sink, const std::vector<std::string>& data)
{
    if (data.empty())
        return;
    for (auto it = data.cbegin(); it != data.cend() - 1; ++it) {
        writeValue(sink, *it);
        sink->send(delimiter);
    }
    writeValue(sink, data.back());
    sink->send("\n");
}

void CSVWriter::writeValue(ExternalIO::ISink* sink, const std::string& value)
{
    for (char ch : value) {
        sink->send(ch);
        if (ch == '"')
            sink->send("\"");
    }
}

} // namespace CSV

} // namespace ExternalIO
