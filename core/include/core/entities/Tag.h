#ifndef TAG_H_6HD0W9PY
#define TAG_H_6HD0W9PY

#include <string>

class Tag {
public:
    Tag();

    Tag(std::string&& name);

    Tag(const std::string& name);

    // Tag(std::string name);

    static std::string prefix;

    std::string name() const;

    void setName(const std::string& name);

    std::string nameWithPrefix() const;

private:
    std::string mName;
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
