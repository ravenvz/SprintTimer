#include "Timer.h"


Timer::Timer(std::function<void(void)> tickCallback,
             std::chrono::milliseconds interval)
    : onTickCallback{tickCallback}
    , interval{interval}
{
}

Timer::~Timer()
{
    stop();
    if (tr.joinable())
        tr.join();
    std::cout << "Timer destroyed" << std::endl;
}

void Timer::start()
{
    running = true;
    tr = std::thread([&]() {
        while (running) {
            std::this_thread::sleep_for(interval);
            onTickCallback();
        }
    });
}

void Timer::stop() { running = false; }
