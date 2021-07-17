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
#ifndef QTSPRINTMONTHLYDISTRIBUTIONREADERCONNECTIONPROXY_H_UBE4QP29
#define QTSPRINTMONTHLYDISTRIBUTIONREADERCONNECTIONPROXY_H_UBE4QP29

#include "ThreadConnectionHelper.h"
#include "core/SprintDistributionReader.h"

namespace sprint_timer::compose {

class QtSprintMonthlyDistributionReaderConnectionProxy
    : public SprintDistributionReader {
public:
    QtSprintMonthlyDistributionReaderConnectionProxy(
        ThreadConnectionHelper& connectionHelper, size_t numMonths);

    std::vector<int>
    sprintDistribution(const dw::DateRange& dateRange) override;

private:
    ThreadConnectionHelper& connectionHelper;
    size_t numMonths;

    void checkStorageInteractorInitialized();
};

} // namespace sprint_timer::compose

#endif /* end of include guard:                                                \
          QTSPRINTMONTHLYDISTRIBUTIONREADERCONNECTIONPROXY_H_UBE4QP29 */
