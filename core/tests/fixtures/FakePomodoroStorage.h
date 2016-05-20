#ifndef FAKEPOMODOROSTORAGE_H_K56HOST8
#define FAKEPOMODOROSTORAGE_H_K56HOST8

#include <unordered_map>

class FakePomodoroStorage {
public:
    void store(const Pomodoro& pomodoro)
    {
        storage.insert(std::make_pair(pomodoro.uuid(), pomodoro));
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
    std::unordered_map<std::string, Pomodoro> storage;
};


#endif /* end of include guard: FAKEPOMODOROSTORAGE_H_K56HOST8 */
