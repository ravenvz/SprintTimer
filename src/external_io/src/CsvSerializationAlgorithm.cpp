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

#include "external_io/CsvSerializationAlgorithm.h"

namespace {

void writeValue(const std::string& value, std::stringstream& ss);

} // namespace

namespace sprint_timer::external_io {

std::string encode(const std::vector<std::string>& records, char delimiter)
{
    if (records.empty())
        return "";
    std::stringstream ss;
    for (auto it = records.cbegin(); it != records.cend() - 1; ++it) {
        writeValue(*it, ss);
        ss << delimiter;
    }
    writeValue(records.back(), ss);
    return ss.str();
}

} // namespace sprint_timer::external_io

namespace {

void writeValue(const std::string& value, std::stringstream& ss)
{
    for (char ch : value) {
        ss << ch;
        if (ch == '"')
            ss << "\"";
    }
}

} // namespace
