#ifndef TIMER_H_Z8WLMNH0
#define TIMER_H_Z8WLMNH0

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>


/* Countdown timer that runs in background thread
 * and executes callback function each tick. */
class Timer {
public:
    using Interval = std::chrono::milliseconds;
    using Tick = std::chrono::milliseconds;

    Timer(std::function<void(void)> tickCallback,
          std::chrono::milliseconds tickPeriod);

    ~Timer();

    Timer(Timer&&) = default;
    Timer& operator=(Timer&&) = default;

    Timer(const Timer&) = default;
    Timer& operator=(const Timer&) = default;

    /* Start running the timer.
     *
     * onTickCallback function will be executed each tickPeriod. */
    void start();

    /* Stop the timer. */
    void stop();

private:
    std::function<void(void)> onTickCallback;
    std::chrono::milliseconds tickPeriod;
    std::atomic<bool> running{false};
    std::thread tr;
};

#endif
