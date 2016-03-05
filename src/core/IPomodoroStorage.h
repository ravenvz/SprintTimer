#ifndef IPOMODOROSTORAGE_H_OFBQEAXD
#define IPOMODOROSTORAGE_H_OFBQEAXD

#include <vector>
#include <string>
#include "core/entities/Pomodoro.h"

class IPomodoroStorage {
public:
    IPomodoroStorage() = delete;
    ~IPomodoroStorage() = default;

    IPomodoroStorage(IPomodoroStorage&&) = delete;
    IPomodoroStorage& operator=(IPomodoroStorage&&) = delete;

    IPomodoroStorage(const IPomodoroStorage&) = delete;
    IPomodoroStorage& operator=(const IPomodoroStorage&) = delete;

    virtual bool insert(const long long assocTaskId,
                        const TimeInterval& interval)
        = 0;

    virtual bool remove(int row) = 0;

    virtual std::vector<Pomodoro> items() = 0;

    virtual std::vector<Pomodoro> items(const DateInterval& interval) = 0;

    virtual std::vector<std::string> yearRange() = 0;
};

#endif /* end of include guard: IPOMODOROSTORAGE_H_OFBQEAXD */
