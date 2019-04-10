/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#ifndef PERIODICBACKGROUNDRUNNER_H_PWNGBFXL
#define PERIODICBACKGROUNDRUNNER_H_PWNGBFXL

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <thread>

namespace sprint_timer {

/* Periodically executes user-supplied function in the background thread.
 *
 * Executes function periodically each tickPeriod.
 * Executes (another) function when time runs out. */
class PeriodicBackgroundRunner {
public:
    using TickPeriod = std::chrono::milliseconds;
    using OnTickFunction = std::function<void(TickPeriod)>;
    using OnTimeRunOutFunction = std::function<void(void)>;

    PeriodicBackgroundRunner(OnTickFunction onTick,
                             OnTimeRunOutFunction onTimeRunOut,
                             std::chrono::milliseconds runnerDuration,
                             TickPeriod tickPeriod);

    ~PeriodicBackgroundRunner();

    PeriodicBackgroundRunner(PeriodicBackgroundRunner&&) = delete;
    PeriodicBackgroundRunner& operator=(PeriodicBackgroundRunner&&) = delete;

    PeriodicBackgroundRunner(const PeriodicBackgroundRunner&) = delete;
    PeriodicBackgroundRunner& operator=(const PeriodicBackgroundRunner&)
        = delete;

    void stop();

    void setCyclic(bool cyclic);

private:
    bool interruptRequested{false};
    std::atomic<bool> cycle{false};
    std::condition_variable interruptCv;
    std::mutex mtx;
    std::thread tr;

    void runTimer(std::chrono::milliseconds remainingTime,
                  TickPeriod tickPeriod,
                  OnTickFunction onTick,
                  OnTimeRunOutFunction onTimeRunOut);
};

} // namespace sprint_timer

#endif /* end of include guard: PERIODICBACKGROUNDRUNNER_H_PWNGBFXL */
