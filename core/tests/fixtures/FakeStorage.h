#ifndef FAKEPOMODOROSTORAGE_H_K56HOST8
#define FAKEPOMODOROSTORAGE_H_K56HOST8

#include <unordered_map>

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

    size_t size() const { return storage.size(); }

private:
    std::unordered_map<std::string, Entity> storage;
};


#endif /* end of include guard: FAKEPOMODOROSTORAGE_H_K56HOST8 */
