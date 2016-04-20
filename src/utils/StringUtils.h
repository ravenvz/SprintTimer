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

// std::string join(std::vector<std::string>&& vec, const std::string&
// delimeter)
// {
//     std::cout << "Move overload called" << std::endl;
//     std::string res;
//     std::for_each(std::make_move_iterator(vec.begin()),
//                   std::make_move_iterator(vec.end()),
//                   [&](const auto& elem) {
//                       res += elem;
//                       res += delimeter;
//                   });
//     res.pop_back();
//     return res;
// }

#endif /* end of include guard: STRINGUTILS_H_Y89VODRG */
