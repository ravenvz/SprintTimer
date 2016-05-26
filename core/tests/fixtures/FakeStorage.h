#ifndef FAKEPOMODOROSTORAGE_H_K56HOST8
#define FAKEPOMODOROSTORAGE_H_K56HOST8

#include "core/TimeSpan.h"
#include <algorithm>
#include <experimental/optional>
#include <unordered_map>

using std::experimental::optional;
using std::experimental::make_optional;

template <class Entity>
class FakeStorage {
public:
    void store(const Entity& item)
    {
        storage.insert(std::make_pair(item.uuid(), item));
    }

    bool contains(const std::string& uuid)
    {
        auto found = storage.find(uuid);
        return found != cend(storage);
    }

    void remove(const std::string& uuid)
    {
        auto found = storage.find(uuid);
        if (found != cend(storage)) {
            storage.erase(found);
        }
    }

    void
    itemsInTimeRange(const TimeSpan& timeSpan,
                     std::function<void(const std::vector<Entity>&)> callback)
    {
        std::vector<Entity> result;
        std::vector<Entity> values;
        std::transform(storage.cbegin(),
                       storage.cend(),
                       std::back_inserter(values),
                       [](const auto& elem) { return elem.second; });
        std::copy_if(cbegin(values),
                     cend(values),
                     std::back_inserter(result),
                     [&timeSpan](const auto& elem) {
                         return startDateAbsDiff(timeSpan, elem.timeSpan())
                             == 0;
                     });
        callback(result);
    }

    optional<Entity> getItem(const std::string& uuid)
    {
        auto found = storage.find(uuid);
        if (found != cend(storage)) {
            return make_optional(found->second);
        }
        return optional<Entity>();
    }

    Entity& itemRef(const std::string& uuid) { return storage.at(uuid); }

    size_t size() const { return storage.size(); }

private:
    std::unordered_map<std::string, Entity> storage;
};


#endif /* end of include guard: FAKEPOMODOROSTORAGE_H_K56HOST8 */
