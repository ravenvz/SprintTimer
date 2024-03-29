/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#ifndef IWORKINGDAYREADER_H_G8IYQC0N
#define IWORKINGDAYREADER_H_G8IYQC0N

#include "core/WorkSchedule.h"
#include <functional>

namespace sprint_timer {

class WorkScheduleReader {
public:
    virtual ~WorkScheduleReader() = default;

    virtual WorkSchedule schedule() = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: IWORKINGDAYREADER_H_G8IYQC0N */

