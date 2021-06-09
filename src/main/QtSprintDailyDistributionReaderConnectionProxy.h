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
#ifndef QTSPRINTDISTRIBUTIONREADERCONNECTIONPROXY_H_EBGCGDJT
#define QTSPRINTDISTRIBUTIONREADERCONNECTIONPROXY_H_EBGCGDJT

#include "ThreadConnectionHelper.h"
#include "core/SprintDistributionReader.h"

namespace sprint_timer::compose {

class QtSprintDailyDistributionReaderConnectionProxy
    : public SprintDistributionReader {
public:
    QtSprintDailyDistributionReaderConnectionProxy(
        ThreadConnectionHelper& connectionHelper, size_t numDays);

    std::vector<int>
    sprintDistribution(const dw::DateRange& dateRange) override;

private:
    ThreadConnectionHelper& connectionHelper;
    size_t numDays;

    void createStorageInteractor();
};

} // namespace sprint_timer::compose

#endif /* end of include guard:                                                \
          QTSPRINTDISTRIBUTIONREADERCONNECTIONPROXY_H_EBGCGDJT */
