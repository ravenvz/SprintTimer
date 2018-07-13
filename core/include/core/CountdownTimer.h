/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#ifndef TIMER_H_Z8WLMNH0
#define TIMER_H_Z8WLMNH0

#include <atomic>
#include <chrono>
#include <functional>
#include <thread>

namespace sprint_timer {

/* Countdown timer that runs in background thread
 * and executes callback function each tick. */
class CountdownTimer {
public:
    using TickPeriod = std::chrono::milliseconds;
    using OnTickCallback = std::function<void(TickPeriod)>;
    using OnTimeRunOutCallback = std::function<void(void)>;

    CountdownTimer(OnTickCallback tickCallback,
                   OnTimeRunOutCallback onTimeRunOutCallback,
                   std::chrono::milliseconds duration,
                   TickPeriod tickPeriod);

    ~CountdownTimer();

    CountdownTimer(CountdownTimer&&) = delete;
    CountdownTimer& operator=(CountdownTimer&&) = delete;

    CountdownTimer(const CountdownTimer&) = delete;
    CountdownTimer& operator=(const CountdownTimer&) = delete;

    void stop();

private:
    OnTickCallback onTickCallback;
    OnTimeRunOutCallback onTimeRunOutCallback;
    const std::chrono::milliseconds duration;
    const TickPeriod tickPeriod;
    std::atomic<bool> running{false};
    std::thread tr;

    void start();
};

} // namespace sprint_timer

#endif
