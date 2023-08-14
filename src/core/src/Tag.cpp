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
#include "core/Tag.h"
#include "cpp_utils/algorithms/string_ext.h"

namespace {

constexpr std::string_view prefix{"#"};

} // namespace

namespace sprint_timer {

Tag::Tag(std::string name)
    : name_{std::move(name)}
{
}

std::string Tag::name() const { return name_; }

void Tag::setName(const std::string& name) { name_ = name; }

std::string prefixTags(std::span<const Tag> tags)
{
    if (tags.empty()) {
        return "";
    }
    std::vector<std::string> prefixedTags(tags.size());
    std::ranges::transform(
        tags, std::back_inserter(prefixedTags), [](const auto& tag) {
            return tag.nameWithPrefix();
        });
    return alg::join(prefixedTags.cbegin(), prefixedTags.cend(), " ");
}

std::string Tag::nameWithPrefix() const
{
    return name().empty() ? std::string{} : std::string{prefix} + name();
}

std::ostream& operator<<(std::ostream& os, const Tag& tag)
{
    os << tag.name();
    return os;
}

} // namespace sprint_timer
