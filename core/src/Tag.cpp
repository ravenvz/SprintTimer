/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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


Tag::Tag(std::string name)
    : aName{std::move(name)}
{
}

std::string Tag::name() const { return aName; }

void Tag::setName(const std::string& name) { aName = name; }

std::string Tag::nameWithPrefix() const
{
    if (name().empty())
        return "";
    return prefix + name();
}

/* static */
std::string Tag::prefix = std::string("#");

std::ostream& operator<<(std::ostream& os, const Tag& tag)
{
    os << tag.aName;
    return os;
}
