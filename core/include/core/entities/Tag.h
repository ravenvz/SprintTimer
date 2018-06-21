/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#ifndef TAG_H_6HD0W9PY
#define TAG_H_6HD0W9PY

#include "core/utils/StringUtils.h"
#include <algorithm>
#include <ostream>
#include <string>
#include <vector>

namespace sprint_timer::entities {

class Tag {
public:
    Tag() = default;

    Tag(std::string name);

    static std::string prefix;

    std::string name() const;

    void setName(const std::string& name);

    std::string nameWithPrefix() const;

    friend std::ostream& operator<<(std::ostream& os, const Tag& tag);

private:
    std::string aName;
};

inline bool operator<(const Tag& lhs, const Tag& rhs)
{
    return lhs.name() < rhs.name();
}

inline bool operator==(const Tag& lhs, const Tag& rhs)
{
    return lhs.name() == rhs.name();
}

/* Given container with tags, return string that containes
 * all tags with leading prefix separated by whitespace.*/
template <typename TagContainer>
std::string prefixTags(const TagContainer& tags)
{
    if (tags.empty())
        return "";
    std::vector<Tag> prefixedTags;
    std::transform(tags.cbegin(),
                   tags.cend(),
                   std::back_inserter(prefixedTags),
                   [](const auto& tag) { return tag.nameWithPrefix(); });
    return utils::join(prefixedTags.cbegin(), prefixedTags.cend(), " ");
}

} // namespace sprint_timer::entities

namespace std {

template <>
struct hash<sprint_timer::entities::Tag> {
    size_t operator()(const sprint_timer::entities::Tag& tag) const
    {
        return std::hash<std::string>()(tag.name());
    }
};

} // namespace std

#endif /* end of include guard: TAG_H_6HD0W9PY */
