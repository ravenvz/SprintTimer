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
