#include "core/StringUtils.h"
#include <iostream>
#include <regex>


std::string StringUtils::join(const std::vector<std::string>& vec,
                              const std::string& delimeter)
{
    std::string res;
    for (const auto& word : vec) {
        res += word;
        res += delimeter;
    }
    res.pop_back();
    return res;
}

std::list<std::string> StringUtils::parseWords(std::string text)
{
    std::regex word_regex{"[a-zA-Z0-9]+"};
    std::sregex_iterator words_begin{text.begin(), text.end(), word_regex};
    std::sregex_iterator words_end;
    std::list<std::string> res;

    for (auto it = words_begin; it != words_end; ++it) {
        res.push_back(it->str());
    }
    return res;
}
