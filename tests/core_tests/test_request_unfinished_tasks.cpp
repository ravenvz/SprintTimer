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
#include <core/use_cases/request_tasks/UnfinishedTasksHandler.h>
#include <core/use_cases/request_tasks/UnfinishedTasksQuery.h>

using ::testing::_;

class RequestUnfinishedTasksFixture : public ::testing::Test {
public:
    mocks::TaskStorageMock task_storage_mock;
    sprint_timer::use_cases::UnfinishedTasksHandler handler{task_storage_mock};
};

TEST_F(RequestUnfinishedTasksFixture, execute)
{
    using namespace sprint_timer::use_cases;
    using namespace sprint_timer::entities;
    EXPECT_CALL(task_storage_mock, unfinishedTasks()).Times(1);

    handler.handle(UnfinishedTasksQuery{});
}
