#include "StringUtils.h"
#include <iostream>


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
