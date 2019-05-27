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
#ifndef WORKINGDAYSSTORAGEMOCK_H_XPILNRIE
#define WORKINGDAYSSTORAGEMOCK_H_XPILNRIE

#include <core/IWorkingDaysStorage.h>
#include <gmock/gmock.h>

class WorkingDaysStorageMock : public sprint_timer::IWorkingDaysStorage {
public:
    MOCK_METHOD1(requestData,
                 void(sprint_timer::IWorkingDaysReader::ResultHandler));

    MOCK_METHOD1(changeWorkingDays, void(const sprint_timer::WorkdayTracker&));

    MOCK_METHOD1(addExtraHolidays, void(const std::vector<dw::Date>&));

    MOCK_METHOD1(addExtraWorkdays, void(const std::vector<dw::Date>&));

    MOCK_METHOD1(removeExtraHolidays, void(const std::vector<dw::Date>&));

    MOCK_METHOD1(removeExtraWorkdays, void(const std::vector<dw::Date>&));
};

#endif /* end of include guard: WORKINGDAYSSTORAGEMOCK_H_XPILNRIE */
