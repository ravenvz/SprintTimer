/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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

#include <algorithm>
#include <ostream>
#include <span>
#include <string>
#include <vector>

namespace sprint_timer {

class Tag {
public:
    Tag() = default;

    explicit Tag(std::string name);

    std::string name() const;

    void setName(const std::string& name);

    std::string nameWithPrefix() const;

    auto operator<=>(const Tag& other) const = default;

private:
    std::string name_;
};

std::ostream& operator<<(std::ostream& os, const Tag& tag);

/* Given container with tags, return string that contains
 * all tags with leading prefix separated by whitespace.*/
std::string prefixTags(std::span<const Tag> tags);

} // namespace sprint_timer

// TODO probably the only place where Tag used as key is in TagTop. Deal with
// hash here and do not reopen std namespace
namespace std {

template <> struct hash<sprint_timer::Tag> {
    size_t operator()(const sprint_timer::Tag& tag) const
    {
        return std::hash<std::string>()(tag.name());
    }
};

} // namespace std

#endif /* end of include guard: TAG_H_6HD0W9PY */
