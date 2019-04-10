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
#include "core/entities/Tag.h"

namespace sprint_timer::entities {

Tag::Tag(std::string name)
    : name_{std::move(name)}
{
}

std::string Tag::name() const { return name_; }

void Tag::setName(const std::string& name) { name_ = name; }

std::string Tag::nameWithPrefix() const
{
    if (name().empty())
        return "";
    return prefix + name();
}

/* static */
std::string const Tag::prefix = std::string("#");

std::ostream& operator<<(std::ostream& os, const Tag& tag)
{
    os << tag.name();
    return os;
}

bool operator<(const Tag& lhs, const Tag& rhs)
{
    return lhs.name() < rhs.name();
}

bool operator==(const Tag& lhs, const Tag& rhs)
{
    return lhs.name() == rhs.name();
}

} // namespace sprint_timer::entities
