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

#include "mocks/SprintStorageReaderMock.h"
#include "gtest/gtest.h"
#include <core/QueryInvoker.h>
#include <core/use_cases/RequestSprintsForTask.h>

using sprint_timer::use_cases::RequestSprintsForTask;
using ::testing::_;

class RequestSprintsForTaskFixture : public ::testing::Test {
public:
    SprintStorageReaderMock sprint_reader_mock;
    sprint_timer::QueryInvoker queryInvoker;
};

TEST_F(RequestSprintsForTaskFixture, execute)
{
    EXPECT_CALL(sprint_reader_mock, sprintsForTask("123", _)).Times(1);

    queryInvoker.execute(std::make_unique<RequestSprintsForTask>(
        sprint_reader_mock, "123", [](const auto& result) {}));
}
