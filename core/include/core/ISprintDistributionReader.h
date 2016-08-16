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
#ifndef IPOMODORODAILYDISTRIBUTIONREADER_H_XMW7JEI6
#define IPOMODORODAILYDISTRIBUTIONREADER_H_XMW7JEI6

#include "core/Distribution.h"
#include "core/TimeSpan.h"
#include <functional>

class ISprintDistributionReader {
public:
    using Handler = std::function<void(const Distribution<int>&)>;
    virtual ~ISprintDistributionReader() = default;
    // TODO rename to requestDistribution
    virtual void requestDailyDistribution(const TimeSpan& timeSpan,
                                          Handler handler)
        = 0;
};


#endif /* end of include guard: IPOMODORODAILYDISTRIBUTIONREADER_H_XMW7JEI6 */
