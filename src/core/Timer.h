#ifndef TIMER_H_Z8WLMNH0
#define TIMER_H_Z8WLMNH0

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>


class Timer {
public:
    using Interval = std::chrono::milliseconds;
    using Tick = std::chrono::milliseconds;

    Timer(std::function<void(void)> tickCallback,
          std::chrono::milliseconds interval);

    ~Timer();

    Timer(Timer&&) = default;
    Timer& operator=(Timer&&) = default;

    Timer(const Timer&) = default;
    Timer& operator=(const Timer&) = default;

    void start();

    void stop();

private:
    std::function<void(void)> onTickCallback;
    std::chrono::milliseconds interval;
    std::atomic<bool> running{false};
    std::thread tr;
};

#endif
