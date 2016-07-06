/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
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
