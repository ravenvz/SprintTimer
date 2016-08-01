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
#include "core/entities/Tag.h"

Tag::Tag() {}

Tag::Tag(std::string&& name)
    : mName(std::move(name))
{
}

Tag::Tag(const std::string& name)
    : mName{name}
{
}

// Tag::Tag(std::string name)
//     : mName{name}
// {
// }

std::string Tag::name() const { return mName; }

void Tag::setName(const std::string& name) { mName = name; }

std::string Tag::nameWithPrefix() const { return prefix + name(); }

/* static */
std::string Tag::prefix = std::string("#");

// bool operator<(const Tag& lhs, const Tag& rhs)
// {
//     return lhs.name() < rhs.name();
// }
//
// bool operator==(const Tag& lhs, const Tag& rhs)
// {
//     return lhs.name() == rhs.name();
// }