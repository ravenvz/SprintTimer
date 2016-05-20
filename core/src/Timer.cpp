#include "core/Timer.h"


Timer::Timer(std::function<void(void)> tickCallback,
             std::chrono::milliseconds tickPeriod)
    : onTickCallback{tickCallback}
    , tickPeriod{tickPeriod}
{
}

Timer::~Timer()
{
    stop();
    if (tr.joinable())
        tr.join();
}

void Timer::start()
{
    running = true;
    tr = std::thread([&]() {
        while (running) {
            std::this_thread::sleep_for(tickPeriod);
            onTickCallback();
        }
    });
}

void Timer::stop() { running = false; }
