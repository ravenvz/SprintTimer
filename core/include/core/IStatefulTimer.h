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

#ifndef ISTATEFULTIMER_H_D3DG0N7C
#define ISTATEFULTIMER_H_D3DG0N7C

#include "core/TimeSpan.h"
#include <vector>

class IStatefulTimer {
public:
    enum class State { Idle, Task, Break, LongBreak, ZoneEntered, ZoneLeft, Finished };

    virtual ~IStatefulTimer() = default;

    virtual void start() = 0;

    virtual void cancel() = 0;

    virtual int currentDuration() const = 0;

    virtual State state() const = 0;

    virtual void toggleInTheZoneMode() = 0;

    virtual std::vector<TimeSpan> completedTaskIntervals() const = 0;

    virtual void clearIntervalsBuffer() = 0;
};

#endif /* end of include guard: ISTATEFULTIMER_H_D3DG0N7C */
