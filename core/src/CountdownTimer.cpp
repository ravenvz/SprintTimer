/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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
#include "core/CountdownTimer.h"

CountdownTimer::CountdownTimer(OnTickCallback tickCallback,
                               OnTimeRunOutCallback onTimeRunOutCallback,
                               std::chrono::milliseconds duration,
                               CountdownTimer::TickPeriod tickPeriod)
    : onTickCallback{tickCallback}
    , onTimeRunOutCallback{onTimeRunOutCallback}
    , duration{duration}
    , tickPeriod{tickPeriod}
{
    start();
}

CountdownTimer::~CountdownTimer()
{
    stop();
    if (tr.joinable())
        tr.join();
}

void CountdownTimer::start()
{
    running = true;
    tr = std::thread([this]() {
        auto remainingTime = duration;
        while (running && (remainingTime.count() > 0)) {
            auto nextTickPoint = std::chrono::steady_clock::now() + tickPeriod;
            onTickCallback(remainingTime);
            remainingTime -= tickPeriod;
            std::this_thread::sleep_until(nextTickPoint);
        }
        onTimeRunOutCallback();
    });
}

void CountdownTimer::stop() { running = false; }
