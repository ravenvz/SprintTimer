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

#include "mocks/TaskStorageMock.h"
#include "gtest/gtest.h"
#include <core/ActionInvoker.h>
#include <core/ObservableActionInvoker.h>
#include <core/actions/ToggleTaskCompleted.h>

using sprint_timer::actions::ToggleTaskCompleted;
using sprint_timer::entities::Tag;
using sprint_timer::entities::Task;
using namespace dw;

class ToggleTaskCompletionFixture : public ::testing::Test {
public:
    Task someTask{"Task name",
                  4,
                  2,
                  "550e8400-e29b-41d4-a716-446655440000",
                  {Tag{"Tag1"}, Tag{"Tag2"}},
                  false,
                  dw::DateTime{Date{Year{2015}, Month{11}, Day{10}}}};

    sprint_timer::ObservableActionInvoker actionInvoker;
    mocks::TaskStorageMock task_storage_mock;
};

TEST_F(ToggleTaskCompletionFixture, execute)
{
    EXPECT_CALL(task_storage_mock,
                toggleCompleted(someTask.uuid(), dw::current_date_time_local()))
        .Times(1);

    actionInvoker.execute(
        std::make_unique<ToggleTaskCompleted>(task_storage_mock, someTask));
}

TEST_F(ToggleTaskCompletionFixture, undo_should_not_modify_timestamp)
{
    EXPECT_CALL(task_storage_mock,
                toggleCompleted(someTask.uuid(), dw::current_date_time_local()))
        .Times(1);

    actionInvoker.execute(
        std::make_unique<ToggleTaskCompleted>(task_storage_mock, someTask));

    EXPECT_CALL(task_storage_mock,
                toggleCompleted(someTask.uuid(), someTask.lastModified()))
        .Times(1);

    actionInvoker.undo();
}
