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
#include "core/PeriodicBackgroundRunner.h"

namespace sprint_timer {

PeriodicBackgroundRunner::PeriodicBackgroundRunner(
    OnTickFunction onTick,
    OnTimeRunOutFunction onTimeRunOut,
    std::chrono::milliseconds runnerDuration,
    PeriodicBackgroundRunner::TickPeriod tickPeriod)
{
    tr = std::thread(
        [this, onTick, onTimeRunOut, runnerDuration, tickPeriod]() {
            runTimer(runnerDuration, tickPeriod, onTick, onTimeRunOut);
        });
}

PeriodicBackgroundRunner::~PeriodicBackgroundRunner() { stop(); }

void PeriodicBackgroundRunner::stop()
{
    {
        std::unique_lock<std::mutex> lock{mtx};
        interruptRequested = true;
    }
    interruptCv.notify_all();
    if (tr.joinable())
        tr.join();
}

void PeriodicBackgroundRunner::setCyclic(bool cyclic) { cycle = cyclic; }

void PeriodicBackgroundRunner::runTimer(std::chrono::milliseconds remainingTime,
                                        TickPeriod tickPeriod,
                                        OnTickFunction onTick,
                                        OnTimeRunOutFunction onTimeRunOut)
{
    using namespace std::chrono;

    auto timeLeft = remainingTime;
    const auto startTime = steady_clock::now();
    int iterationNumber{0};

    while (true) {
        {
            const auto timeOut = startTime + ++iterationNumber * tickPeriod;
            std::unique_lock<std::mutex> lock{mtx};
            if (interruptCv.wait_until(
                    lock, timeOut, [this]() { return interruptRequested; })) {
                break;
            }
        }

        timeLeft -= tickPeriod;
        onTick(timeLeft);
        if (timeLeft < tickPeriod) {
            onTimeRunOut();
            if (cycle)
                timeLeft = remainingTime;
            else
                break;
        }
    }
}

} // namespace sprint_timer
