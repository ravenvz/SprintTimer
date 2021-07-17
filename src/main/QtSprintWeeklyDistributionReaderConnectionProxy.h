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
#ifndef QTSPRINTWEEKLYDISTRIBUTIONREADERCONNECTIONPROXY_H_JC2HZ8AV
#define QTSPRINTWEEKLYDISTRIBUTIONREADERCONNECTIONPROXY_H_JC2HZ8AV

#include "ThreadConnectionHelper.h"
#include "core/IConfig.h"
#include "core/SprintDistributionReader.h"

namespace sprint_timer::compose {

class QtSprintWeeklyDistributionReaderConnectionProxy
    : public SprintDistributionReader {
public:
    QtSprintWeeklyDistributionReaderConnectionProxy(
        ThreadConnectionHelper& connectionHelper,
        size_t numWeeks,
        IConfig& applicationSettings);

    std::vector<int>
    sprintDistribution(const dw::DateRange& dateRange) override;

private:
    ThreadConnectionHelper& connectionHelper;
    size_t numWeeks;
    IConfig& applicationSettings;

    void checkStorageInteractorInitialized();
};

} // namespace sprint_timer::compose

#endif /* end of include guard:                                                \
          QTSPRINTWEEKLYDISTRIBUTIONREADERCONNECTIONPROXY_H_JC2HZ8AV */
