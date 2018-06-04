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
}

void CountdownTimer::start()
{
    running = true;
    tr = std::thread([this]() {
        auto remainingTime = duration;
        // TODO think of more elegant way than counter workaround; it can potentially cause problems with some tickPeriod values
        auto tickCheckPeriod = tickPeriod / 10;
        size_t counter{0};
        while (running) {
            if (counter % 10 == 0) {
                counter = 0;
                onTickCallback(remainingTime);
                if (remainingTime.count() <= 0) {
                    onTimeRunOutCallback();
                    break;
                }
            }
            auto nextTickPoint = std::chrono::steady_clock::now() + tickCheckPeriod;
            remainingTime -= tickCheckPeriod;
            ++counter;
            std::this_thread::sleep_until(nextTickPoint);
        }
    });
}

void CountdownTimer::stop()
{
    running = false;
    if (tr.joinable())
        tr.join();
}
