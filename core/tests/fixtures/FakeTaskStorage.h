#ifndef FAKETASKSTORAGE_H_ZXTHRYBL
#define FAKETASKSTORAGE_H_ZXTHRYBL

#include <unordered_map>

class FakeTaskStorage {
public:
    void store(const TodoItem& task)
    {
        storage.insert(std::make_pair(task.uuid(), task));
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
    std::unordered_map<std::string, TodoItem> storage;
};

#endif /* end of include guard: FAKETASKSTORAGE_H_ZXTHRYBL */
