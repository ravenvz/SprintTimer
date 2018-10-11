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
#include <iostream>

namespace {

using sprint_timer::PeriodicBackgroundRunner;


template <int FrequencyScale>
void runTimer(const std::atomic<bool>& running,
              std::chrono::milliseconds remainingTime,
              PeriodicBackgroundRunner::TickPeriod tickPeriod,
              PeriodicBackgroundRunner::OnTickFunction onTick,
              PeriodicBackgroundRunner::OnTimeRunOutFunction onTimeRunOut)
{
    using namespace std::chrono;
    using TimePollPeriod = duration<int, std::ratio<1, FrequencyScale>>;

    const auto startTime = steady_clock::now();
    auto lastCallbackTime = startTime;
    int iterationNumber{0};

    while (running) {
        const auto timeNow = steady_clock::now();
        if (timeNow - lastCallbackTime >= tickPeriod) {
            lastCallbackTime = timeNow;
            remainingTime -= tickPeriod;
            onTick(remainingTime);
            if (remainingTime < TimePollPeriod{1}) {
                onTimeRunOut();
                break;
            }
        }
        std::this_thread::sleep_until(startTime
                                      + TimePollPeriod{++iterationNumber});
    }
}

} // namespace

namespace sprint_timer {

PeriodicBackgroundRunner::PeriodicBackgroundRunner(
    OnTickFunction onTick,
    OnTimeRunOutFunction onTimeRunOut,
    std::chrono::milliseconds runnerDuration,
    PeriodicBackgroundRunner::TickPeriod tickPeriod)
{
    tr = std::thread(
        [this, onTick, onTimeRunOut, runnerDuration, tickPeriod]() {
            constexpr int frequencyScale{5};
            runTimer<frequencyScale>(
                running, runnerDuration, tickPeriod, onTick, onTimeRunOut);
        });
}

PeriodicBackgroundRunner::~PeriodicBackgroundRunner() { stop(); }

void PeriodicBackgroundRunner::stop()
{
    running = false;
    if (tr.joinable())
        tr.join();
}

} // namespace sprint_timer
