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

#include "core/utils/CSVEncoder.h"

namespace ExternalIO {

namespace CSV {

CSVEncoder::CSVEncoder(char delimiter)
    : delimiter{delimiter}
{

}

void CSVEncoder::encodeRow(const std::vector<std::string>& row)
{
    if (row.empty())
        return;
    for (auto it = row.cbegin(); it != row.cend() - 1; ++it) {
        writeValue(*it);
        ss << delimiter;
    }
    writeValue(row.back());
    ss << "\n";
}

std::string CSVEncoder::encode(const std::vector<CSVEncoder::Data>& data)
{
    if (data.empty())
        return "";
    for (const auto& row : data) {
        encodeRow(row);
    }
    std::string result = ss.str();
    ss.str("");
    return result;
}

void CSVEncoder::writeValue(const std::string& value)
{
    for (char ch : value) {
        ss << ch;
        if (ch == '"')
            ss << "\"";
    }
}

} // namespace CSV

} // namespace ExternalIO
