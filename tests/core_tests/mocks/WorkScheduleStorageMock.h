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
#ifndef WORKINGDAYSSTORAGEMOCK_H_XPILNRIE
#define WORKINGDAYSSTORAGEMOCK_H_XPILNRIE

#include <core/WorkScheduleStorage.h>
#include <gmock/gmock.h>

namespace mocks {

class WorkScheduleStorageMock : public sprint_timer::WorkScheduleStorage {
public:
    MOCK_METHOD0(schedule, sprint_timer::WorkSchedule());

    MOCK_METHOD1(updateSchedule, void(const sprint_timer::WorkSchedule&));
};

} // namespace mocks

#endif /* end of include guard: WORKINGDAYSSTORAGEMOCK_H_XPILNRIE */
