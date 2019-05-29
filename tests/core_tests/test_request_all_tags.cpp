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
#include <core/QueryInvoker.h>
#include <core/use_cases/RequestAllTags.h>

using sprint_timer::QueryInvoker;
using sprint_timer::use_cases::RequestAllTags;
using ::testing::_;

class RequestAllTagsFixture : public ::testing::Test {
public:
    QueryInvoker queryInvoker;
    TaskStorageMock task_storage_mock;
};

TEST_F(RequestAllTagsFixture, execute)
{
    EXPECT_CALL(task_storage_mock, requestAllTags(_)).Times(1);

    queryInvoker.execute(std::make_unique<RequestAllTags>(
        task_storage_mock, [](const auto& result) {}));
}
