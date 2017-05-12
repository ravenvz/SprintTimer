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
#ifndef STRINGUTILS_H_Y89VODRG
#define STRINGUTILS_H_Y89VODRG

#include <list>
#include <sstream>
#include <string>
#include <vector>

namespace StringUtils {

template <class ForwardIterator>
std::string join(ForwardIterator first,
                 ForwardIterator last,
                 const std::string& delimeter,
                 typename std::enable_if<!std::is_same<
                     typename std::iterator_traits<ForwardIterator>::value_type,
                     std::string>::value>::type* dummy
                 = 0)
{
    std::ostringstream res;
    for (auto it = first; it != last; ++it) {
        res << *it;
        if (std::distance(it, last) > 1)
            res << delimeter;
    }
    return res.str();
}

// Template overload for std::string type
// Skips empty strings
template <class ForwardIterator>
std::string join(ForwardIterator first,
                 ForwardIterator last,
                 const std::string& delimeter,
                 typename std::enable_if<std::is_same<
                     typename std::iterator_traits<ForwardIterator>::value_type,
                     std::string>::value>::type* dummy
                 = 0)
{
    std::ostringstream res;
    for (auto it = first; it != last; ++it) {
        res << *it;
        if (!(*it).empty() && std::distance(it, last) > 1)
            res << delimeter;
    }
    return res.str();
}


std::string join(const std::vector<std::string>& vec,
                 const std::string& delimeter);

/* Given a text as string, return list<string> that contains all words in text.
 */
std::list<std::string> parseWords(std::string text);

bool startsWith(const std::string& str, const std::string& start);

bool endsWith(const std::string& str, const std::string& end);

} // namespace StringUtils

#endif /* end of include guard: STRINGUTILS_H_Y89VODRG */
