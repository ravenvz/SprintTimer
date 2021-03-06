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

#ifndef ISTATEFULTIMER_H_D3DG0N7C
#define ISTATEFULTIMER_H_D3DG0N7C

#include "date_wrapper/date_wrapper.h"
#include <vector>

namespace sprint_timer {

class IStatefulTimer {
public:
    enum class StateId {
        IdleEntered,
        IdleLeft,
        SprintEntered,
        SprintLeft,
        SprintCancelled,
        SprintFinished,
        BreakEntered,
        BreakLeft,
        BreakCancelled,
        ZoneEntered,
        ZoneLeft
    };

    virtual ~IStatefulTimer() = default;

    virtual void start() = 0;

    virtual void cancel() = 0;

    virtual std::chrono::seconds currentDuration() const = 0;

    virtual void toggleInTheZoneMode() = 0;

    virtual std::vector<dw::DateTimeRange> completedSprints() const = 0;

    virtual void setNumFinishedSprints(int num) = 0;

    virtual void clearSprintsBuffer() = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: ISTATEFULTIMER_H_D3DG0N7C */
