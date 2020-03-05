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
#ifndef SPRINTDAYTIMEDISTRIBUTION_H_AUY5FKGR
#define SPRINTDAYTIMEDISTRIBUTION_H_AUY5FKGR

#include "core/Distribution.h"
#include <date_wrapper/date_wrapper.h>

// TODO remove this

namespace sprint_timer::use_cases {

struct SprintDaytimeDistribution {
    Distribution<double> dayPartDistribution;
    std::vector<dw::DateTimeRange> timeRanges;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: SPRINTDAYTIMEDISTRIBUTION_H_AUY5FKGR */

