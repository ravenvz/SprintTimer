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
#ifndef TAG_H_6HD0W9PY
#define TAG_H_6HD0W9PY

#include <string>

class Tag {
public:

    Tag() = default;

    Tag(std::string name);

    static std::string prefix;

    std::string name() const;

    void setName(const std::string& name);

    std::string nameWithPrefix() const;

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

namespace std {
template <>
struct hash<Tag> {
    size_t operator()(const Tag& tag) const
    {
        return std::hash<std::string>()(tag.name());
    }
};
}

#endif /* end of include guard: TAG_H_6HD0W9PY */
