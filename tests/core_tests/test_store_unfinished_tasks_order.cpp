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

#include "mocks/TaskStorageMock.h"
#include "gtest/gtest.h"
#include "core/ObservableActionInvoker.h"
#include "core/actions/ChangeTasksPriorities.h"

using sprint_timer::actions::ChangeTasksPriorities;

class StoreActiveTasksOrderFixture : public ::testing::Test {
public:
    mocks::TaskStorageMock task_storage_mock;
    sprint_timer::ObservableActionInvoker actionInvoker;
};

TEST_F(StoreActiveTasksOrderFixture, execute_and_undo)
{
    const std::vector<std::string> old_priorities{"1", "2", "3"};
    const std::vector<std::string> new_priorities{"2", "1", "3"};
    EXPECT_CALL(task_storage_mock, updatePriorities(new_priorities)).Times(1);

    actionInvoker.execute(std::make_unique<ChangeTasksPriorities>(
        task_storage_mock,
        std::vector<std::string>{"1", "2", "3"},
        std::vector<std::string>{"2", "1", "3"}));

    EXPECT_CALL(task_storage_mock, updatePriorities(old_priorities)).Times(1);
    actionInvoker.undo();
}
