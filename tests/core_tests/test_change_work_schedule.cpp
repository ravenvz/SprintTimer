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
#include "mocks/WorkScheduleStorageMock.h"
#include "gtest/gtest.h"
#include <core/ObservableActionInvoker.h>
#include <core/actions/ChangeWorkSchedule.h>
#include <core/use_cases/change_schedule/ChangeWorkScheduleHandler.h>

using namespace sprint_timer;

class ChangeWorkingDaysFixture : public ::testing::Test {
public:
    mocks::WorkScheduleStorageMock storage;
    sprint_timer::ObservableActionInvoker actionInvoker;
};

TEST_F(ChangeWorkingDaysFixture, execute_and_undo)
{
    using sprint_timer::use_cases::ChangeWorkScheduleCommand;
    using sprint_timer::use_cases::ChangeWorkScheduleHandler;
    using namespace dw;
    const WorkSchedule oldWorkSchedule;
    WorkSchedule newWorkSchedule;
    newWorkSchedule.addExceptionalDay(Date{Year{2019}, Month{1}, Day{1}}, 0);
    EXPECT_CALL(storage, updateSchedule(newWorkSchedule)).Times(1);

    actionInvoker.execute(std::make_unique<actions::ChangeWorkSchedule>(
        storage, oldWorkSchedule, newWorkSchedule));

    EXPECT_CALL(storage, updateSchedule(oldWorkSchedule));

    actionInvoker.undo();
}
