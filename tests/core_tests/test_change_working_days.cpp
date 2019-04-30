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
#include <core/use_cases/ChangeWorkingDays.h>

using namespace sprint_timer;

class ChangeWorkingDaysFixture : public ::testing::Test {
public:
    WorkingDaysStorageMock storage;
    CommandInvoker command_invoker;
};

TEST_F(ChangeWorkingDaysFixture, execute_and_undo)
{
    using sprint_timer::use_cases::ChangeWorkingDays;
    using namespace dw;
    const WorkdayTracker oldTracker;
    WorkdayTracker newTracker;
    newTracker.addExtraHoliday(Date{Year{2019}, Month{1}, Day{1}});
    EXPECT_CALL(storage, changeWorkingDays(newTracker)).Times(1);

    command_invoker.executeCommand(
        std::make_unique<ChangeWorkingDays>(storage, oldTracker, newTracker));

    EXPECT_CALL(storage, changeWorkingDays(oldTracker));

    command_invoker.undo();
}
