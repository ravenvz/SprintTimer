#ifndef STRINGUTILS_H_Y89VODRG
#define STRINGUTILS_H_Y89VODRG

#include <iostream>
#include <list>
#include <string>
#include <vector>

namespace StringUtils {
std::string join(const std::vector<std::string>& vec,
                 const std::string& delimeter);

template <class ForwardIterator>
std::string
join(ForwardIterator first, ForwardIterator last, const std::string& delimeter)
{
    std::string res;
    for (auto it = first; it != last; ++it) {
        res += *it;
        res += delimeter;
    }
    return res;
}

/* Given a text as string, return list<string> that contains all words in text.
 */
std::list<std::string> parseWords(std::string text);

} // namespace StringUtils

#endif /* end of include guard: STRINGUTILS_H_Y89VODRG */
