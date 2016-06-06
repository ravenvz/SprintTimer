#ifndef FAKEPOMODOROSTORAGE_H_K56HOST8
#define FAKEPOMODOROSTORAGE_H_K56HOST8

#include "core/Distribution.h"
#include "core/TimeSpan.h"
#include "core/entities/Pomodoro.h"
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
        std::vector<Entity> emptyResult;
        callback(emptyResult);
    }

    void requestDailyDistribution(
        const TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)> callback)
    {
        Distribution<int> emptyDistribution{0};
        callback(emptyDistribution);
    }

    void editTag(const std::string& oldName, const std::string& newName)
    {
        for (const auto& pair : storage) {
            for (const auto& tag : pair.second.tags()) {
                if (tag == oldName) {
                    std::list<std::string> oldTags = pair.second.tags();
                    oldTags.remove_if(
                        [&](const auto& elem) { return elem == oldName; });
                    oldTags.push_back(newName);
                    storage.at(pair.first).setTags(oldTags);
                }
            }
        }
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

    void clear() { storage.clear(); }

private:
    std::unordered_map<std::string, Entity> storage;
};


#endif /* end of include guard: FAKEPOMODOROSTORAGE_H_K56HOST8 */
