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
#include "mocks/WorkingDaysStorageMock.h"
#include "gtest/gtest.h"
#include <core/CommandInvoker.h>
#include <core/use_cases/RemoveExtraHolidays.h>

class RemoveExtraHolidayFixture : public ::testing::Test {
public:
    WorkingDaysStorageMock writer_mock;
    sprint_timer::CommandInvoker command_invoker;
};

TEST_F(RemoveExtraHolidayFixture, execute_and_undo)
{
    using namespace dw;
    using sprint_timer::use_cases::RemoveExtraHolidays;
    const std::vector<Date> days{Date{Year{2018}, Month{3}, Day{8}},
                                 Date{Year{2018}, Month{3}, Day{9}},
                                 Date{Year{2018}, Month{3}, Day{10}}};
    EXPECT_CALL(writer_mock, removeExtraHolidays(days)).Times(1);

    command_invoker.executeCommand(
        std::make_unique<RemoveExtraHolidays>(writer_mock, days));

    EXPECT_CALL(writer_mock, addExtraHolidays(days)).Times(1);

    command_invoker.undo();
}

