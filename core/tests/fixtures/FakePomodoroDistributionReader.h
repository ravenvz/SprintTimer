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
#ifndef FAKEPOMODORODISTRIBUTIONREADER_H_UREC2M3T
#define FAKEPOMODORODISTRIBUTIONREADER_H_UREC2M3T

#include "core/ISprintDistributionReader.h"
#include "FakeStorage.h"


class FakePomodoroDistributionReader : public ISprintDistributionReader {
public:
    FakePomodoroDistributionReader(FakeStorage<Sprint>& storage)
        : storage{storage}
    {
    }

    void requestDailyDistribution(const TimeSpan& timeSpan,
                                  Handler handler) final
    {
        return storage.requestDailyDistribution(timeSpan, handler);
    }

    FakeStorage<Sprint>& storage;
};

#endif /* end of include guard: FAKEPOMODORODISTRIBUTIONREADER_H_UREC2M3T */
