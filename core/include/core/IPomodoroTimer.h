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
#ifndef IPOMODOROTIMER_H_D3DG0N7C
#define IPOMODOROTIMER_H_D3DG0N7C

#include "core/TimeSpan.h"

class IPomodoroTimer {
public:
    virtual ~IPomodoroTimer() = default;

    virtual void run() = 0;

    virtual void cancel() = 0;

    virtual TimeSpan finish() = 0;

    virtual int taskDuration() = 0;

    virtual bool isBreak() = 0;

    virtual void toggleInTheZoneMode() = 0;
};

#endif /* end of include guard: IPOMODOROTIMER_H_D3DG0N7C */
