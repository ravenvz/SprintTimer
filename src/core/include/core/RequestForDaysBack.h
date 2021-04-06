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
#ifndef REQUESTFORDAYSBACK_H_N7Y5DX8W
#define REQUESTFORDAYSBACK_H_N7Y5DX8W

#include "core/BackRequestStrategy.h"

namespace sprint_timer {

class RequestForDaysBack : public BackRequestStrategy {
public:
    RequestForDaysBack(int numDays);

    dw::DateRange dateRange() const override;

private:
    int numDays;
};

} // namespace sprint_timer

#endif /* end of include guard: REQUESTFORDAYSBACK_H_N7Y5DX8W */
