/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
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
