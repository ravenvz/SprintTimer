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
#include "core/utils/StringUtils.h"
#include <iomanip>

namespace sprint_timer::utils {

std::string join(const std::vector<std::string>& vec,
                 const std::string& delimeter)
{
    return join(vec.cbegin(), vec.cend(), delimeter);
}

bool startsWith(const std::string& str, const std::string& start)
{
    if (start.size() > str.size())
        return false;
    return std::equal(start.cbegin(), start.cend(), str.cbegin());
}

bool endsWith(const std::string& str, const std::string& end)
{
    if (end.size() > str.size())
        return false;
    return std::equal(end.crbegin(), end.crend(), str.crbegin());
}

std::string formatDecimal(double value, int precision)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
}

} // namespace sprint_timer::utils

