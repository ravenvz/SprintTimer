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
