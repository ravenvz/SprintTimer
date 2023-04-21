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

std::vector<std::string_view> split(std::string_view str, char delimeter)
{
    std::vector<std::string_view> result;
    for (size_t left{0}, right{0}; right != std::string_view::npos;
         left = right + 1) {
        right = str.find_first_of(delimeter, left);
        result.push_back(str.substr(left, right - left));
    }
    return result;
}

auto split(std::string_view str, std::string_view delimiter) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    for (size_t left{0}, right{0}; right != str.npos; left = right + delimiter.size()) {
        right = str.find(delimiter, left);
        result.push_back(str.substr(left, right - left));
    }
    return result;
}

} // namespace sprint_timer::utils

