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

#include "mocks/WorkingDaysReaderMock.h"
#include "gtest/gtest.h"
#include <core/QueryInvoker.h>
#include <core/use_cases/RequestWorkingDays.h>

using ::testing::_;

class RequestWorkingDaysFixture : public ::testing::Test {
public:
    WorkingDaysReaderMock working_days_reader_mock;
    sprint_timer::QueryInvoker queryInvoker;
};

TEST_F(RequestWorkingDaysFixture, execute)
{
    using sprint_timer::WorkdayTracker;
    using sprint_timer::use_cases::RequestWorkingDays;
    EXPECT_CALL(working_days_reader_mock, requestData(_)).Times(1);

    queryInvoker.execute(std::make_unique<RequestWorkingDays>(
        working_days_reader_mock, [](const WorkdayTracker& workdayTracker) {}));
}
