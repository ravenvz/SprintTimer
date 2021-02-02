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
#ifndef STRINGUTILS_H_Y89VODRG
#define STRINGUTILS_H_Y89VODRG

#include <list>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

namespace sprint_timer::utils {

template <class ForwardIterator>
std::string join(ForwardIterator first,
                 ForwardIterator last,
                 const std::string& delimeter,
                 typename std::enable_if<!std::is_same<
                     typename std::iterator_traits<ForwardIterator>::value_type,
                     std::string>::value>::type* dummy = 0)
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
std::string join(
    ForwardIterator first,
    ForwardIterator last,
    const std::string& delimeter,
    typename std::enable_if<
        std::is_same<typename std::iterator_traits<ForwardIterator>::value_type,
                     std::string>::value>::type* dummy = 0)
{
    std::ostringstream res;
    for (; first != last; ++first) {
        res << *first;
        if (!first->empty() && std::distance(first, last) > 1)
            res << delimeter;
    }
    return res.str();
}

template <class ForwardIt, class UnaryOperation>
std::string transformJoin(
    ForwardIt first,
    ForwardIt last,
    const std::string& delimeter,
    UnaryOperation op,
    typename std::enable_if<
        !std::is_same<typename std::iterator_traits<ForwardIt>::value_type,
                      std::string>::value>::type* dummy = 0)
{
    std::ostringstream res;
    for (; first != last; ++first) {
        res << op(*first);
        if (std::distance(first, last) > 1)
            res << delimeter;
    }
    return res.str();
}

// Template overload for std::string type
// Skips empty strings
template <class ForwardIt, class UnaryOperation>
std::string transformJoin(
    ForwardIt first,
    ForwardIt last,
    const std::string& delimeter,
    UnaryOperation op,
    typename std::enable_if<
        std::is_same<typename std::iterator_traits<ForwardIt>::value_type,
                     std::string>::value>::type* dummy = 0)
{
    std::ostringstream res;
    for (; first != last; ++first) {
        if (first->empty())
            continue;
        res << op(*first);
        if (!first->empty() && std::distance(first, last) > 1)
            res << delimeter;
    }
    return res.str();
}

std::string join(const std::vector<std::string>& vec,
                 const std::string& delimeter);

template <class UnaryOperation>
std::string transformJoin(const std::vector<std::string>& vec,
                          const std::string& delimeter,
                          UnaryOperation op)
{
    return transformJoin(cbegin(vec), cend(vec), delimeter, op);
}

/* Given a text as string, populate provided iterator with all words in text.
   Words are defined by expr regex. Default regex allow words to contain
   letters, digits, '+', '-'. */
template <class BidirIt, class OutputIt>
OutputIt parseWords(BidirIt first,
                    BidirIt last,
                    OutputIt out,
                    std::regex expr = std::regex{"[[:alnum:]+-]+"})
{
    std::sregex_iterator words_begin{first, last, expr};
    std::sregex_iterator words_end;

    for (auto it = words_begin; it != words_end; ++it) {
        *out++ = it->str();
    }

    return out;
}

bool startsWith(const std::string& str, const std::string& start);

bool endsWith(const std::string& str, const std::string& end);

std::string formatDecimal(double value, int precision = 2);

} // namespace sprint_timer::utils

#endif /* end of include guard: STRINGUTILS_H_Y89VODRG */
