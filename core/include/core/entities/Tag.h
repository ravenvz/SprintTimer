#ifndef TAG_H_6HD0W9PY
#define TAG_H_6HD0W9PY

#include <string>

class Tag {
public:
    Tag(std::string&& name)
        : mName{std::move(name)}
    {
    }

    std::string name() const { return mName; }

private:
    static const char prefix{'#'};
    std::string mName;
};

#endif /* end of include guard: TAG_H_6HD0W9PY */
