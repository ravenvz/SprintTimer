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

} // namespace StringUtils

#endif /* end of include guard: STRINGUTILS_H_Y89VODRG */
