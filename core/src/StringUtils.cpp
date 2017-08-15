/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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
#include "core/StringUtils.h"
#include <regex>


std::string StringUtils::join(const std::vector<std::string>& vec,
                              const std::string& delimeter)
{
    return join(vec.cbegin(), vec.cend(), delimeter);
}

/* Given a string, return list of words in that string.
 * Word can contain letters, +, - and digits but no other symbols. */
std::list<std::string> StringUtils::parseWords(std::string text)
{
    std::regex word_regex{"[[:alnum:]+-]+"};
    std::sregex_iterator words_begin{text.begin(), text.end(), word_regex};
    std::sregex_iterator words_end;
    std::list<std::string> res;

    for (auto it = words_begin; it != words_end; ++it) {
        res.push_back(it->str());
    }
    return res;
}

bool StringUtils::startsWith(const std::string& str, const std::string& start)
{
	if (start.size() > str.size())
		return false;
    return std::equal(start.cbegin(), start.cend(), str.cbegin());
}

bool StringUtils::endsWith(const std::string& str, const std::string& end)
{
	if (end.size() > str.size())
		return false;
	return std::equal(end.crbegin(), end.crend(), str.crbegin());
}
