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

#include "mocks/SprintStorageMock.h"
#include "gtest/gtest.h"
#include <core/use_cases/request_sprints/SprintsForTaskHandler.h>

using sprint_timer::use_cases::SprintsForTaskHandler;
using sprint_timer::use_cases::SprintsForTaskQuery;
using ::testing::_;

class RequestSprintsForTaskFixture : public ::testing::Test {
public:
    mocks::SprintStorageMock sprint_storage_mock;
    SprintsForTaskHandler handler{sprint_storage_mock};
};

TEST_F(RequestSprintsForTaskFixture, execute)
{
    EXPECT_CALL(sprint_storage_mock, findByTaskUuid("123")).Times(1);

    handler.handle(SprintsForTaskQuery{"123"});
}
