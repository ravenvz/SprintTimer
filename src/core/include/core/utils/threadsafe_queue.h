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
#ifndef THREADSAFE_QUEUE_H_W3FTOMHA
#define THREADSAFE_QUEUE_H_W3FTOMHA

#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>

namespace sprint_timer::utils {

template <typename T> class threadsafe_queue {
public:
    threadsafe_queue() = default;

    threadsafe_queue(const threadsafe_queue<T>& other)
    {
        std::lock_guard<std::mutex> lock{mut};
        data_queue = other.data_queue;
    }

    threadsafe_queue& operator=(const threadsafe_queue<T>&) = delete;

    void push(const T& value)
    {
        std::lock_guard<std::mutex> lock{mut};
        data_queue.push(value);
        data_cond.notify_one();
    }

    void push(T&& value)
    {
        std::lock_guard<std::mutex> lock{mut};
        data_queue.push(std::move(value));
        data_cond.notify_one();
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lock{mut};
        if (data_queue.empty()) {
            return false;
        }
        value = data_queue.front();
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lock{mut};
        if (data_queue.empty()) {
            return std::shared_ptr<T>{};
        }
        std::shared_ptr<T> result{std::make_shared<T>(data_queue.front())};
        data_queue.pop();
        return result;
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lock{mut};
        data_cond.wait(lock, [this] { return !data_queue.empty(); });
        value = data_queue.front();
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lock{mut};
        data_cond.wait(lock, [this] { return !data_queue.empty(); });
        std::shared_ptr<T> result{std::make_shared<T>(data_queue.front())};
        data_queue.pop();
        return result;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock{mut};
        return data_queue.empty();
    }

private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
};

} // namespace sprint_timer::utils

#endif /* end of include guard: THREADSAFE_QUEUE_H_W3FTOMHA */
