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
#ifndef WORKINGDAYSWRITERMOCK_H_Y2JLALNQ
#define WORKINGDAYSWRITERMOCK_H_Y2JLALNQ

#include <core/IWorkingDaysWriter.h>
#include <gmock/gmock.h>

class WorkingDaysWriterMock : public sprint_timer::IWorkingDaysWriter {
public:
    MOCK_METHOD1(changeWorkingDays, void(const sprint_timer::WorkdayTracker&));

    MOCK_METHOD1(addExtraHolidays, void(const std::vector<dw::Date>&));

    MOCK_METHOD1(addExtraWorkdays, void(const std::vector<dw::Date>&));

    MOCK_METHOD1(removeExtraHolidays, void(const std::vector<dw::Date>&));
    MOCK_METHOD1(removeExtraWorkdays, void(const std::vector<dw::Date>&));
};

#endif /* end of include guard: WORKINGDAYSWRITERMOCK_H_Y2JLALNQ */
