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

#include "core/ActionInvoker.h"
#include "core/ObservableActionInvoker.h"
#include "core/actions/ToggleTaskCompleted.h"
#include "mocks/TaskStorageMock.h"
#include "gtest/gtest.h"

using sprint_timer::actions::ToggleTaskCompleted;
using sprint_timer::entities::Tag;
using sprint_timer::entities::Task;
using namespace dw;

class ToggleTaskCompletionFixture : public ::testing::Test {
public:
    sprint_timer::ObservableActionInvoker actionInvoker;
    mocks::TaskStorageMock task_storage_mock;
    const DateTime oldTimeStamp{DateTime{Date{Year{2021}, Month{4}, Day{28}}} +
                                std::chrono::hours{3}};
};

TEST_F(ToggleTaskCompletionFixture, execute)
{
    const std::string uuid{"123"};
    EXPECT_CALL(task_storage_mock,
                toggleCompleted(uuid, dw::current_date_time_local()));

    actionInvoker.execute(std::make_unique<ToggleTaskCompleted>(
        task_storage_mock, uuid, oldTimeStamp));
}

TEST_F(ToggleTaskCompletionFixture, undo_should_not_modify_timestamp)
{
    const std::string uuid{"123"};
    EXPECT_CALL(task_storage_mock,
                toggleCompleted(uuid, dw::current_date_time_local()));

    actionInvoker.execute(std::make_unique<ToggleTaskCompleted>(
        task_storage_mock, uuid, oldTimeStamp));

    EXPECT_CALL(task_storage_mock, toggleCompleted(uuid, oldTimeStamp));

    actionInvoker.undo();
}
