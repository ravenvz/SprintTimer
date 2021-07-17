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
#include "mocks/WorkScheduleStorageMock.h"
#include "gtest/gtest.h"
#include "core/use_cases/request_schedule/WorkScheduleHandler.h"

using sprint_timer::use_cases::WorkScheduleHandler;
using sprint_timer::use_cases::WorkScheduleQuery;
using ::testing::_;

class RequestWorkScheduleFixture : public ::testing::Test {
public:
    mocks::WorkScheduleStorageMock working_days_storage_mock;
    WorkScheduleHandler handler{working_days_storage_mock};
};

TEST_F(RequestWorkScheduleFixture, execute)
{
    EXPECT_CALL(working_days_storage_mock, schedule()).Times(1);

    handler.handle(WorkScheduleQuery{});
}
