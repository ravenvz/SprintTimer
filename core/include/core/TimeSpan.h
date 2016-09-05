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
#ifndef TIMEINTERVAL_H
#define TIMEINTERVAL_H

#include "DateTime.h"
#include <chrono>


/* Represent finite interval in time with start and finish points. */
struct TimeSpan {

    using SystemClock = std::chrono::system_clock::time_point;

    DateTime startTime;
    DateTime finishTime;


    /* Construct from chrono time_point. */
    TimeSpan(SystemClock start, SystemClock finish);

    /* Construct from DateTime start and finish points. */
    TimeSpan(const DateTime& start, const DateTime& finish);

    /* Construct from std::time_t start and finish points. */
    TimeSpan(std::time_t start,
             std::time_t finish,
             int offsetFromUtcInSeconds = 0);

    /* Return time span size in days as unsigned integer.
     *
     * It doesn't matter if start point is further in time compared to
     * finish point. */
    unsigned sizeInDays() const;

    /* Return time representation as string in "HH:mm - HH:mm" format. */
    std::string toTimeString() const;

    /* Return "dd.mm.yy - dd.mm.yy" */
    std::string toDateString() const;
};

/* Return absolute number of days between startTime of this TimeSpan and
 * startTime of other TimeSpan. */
unsigned startDateAbsDiff(const TimeSpan& one, const TimeSpan& other);


#endif /* end of include guard: TIMEINTERVAL_H */
