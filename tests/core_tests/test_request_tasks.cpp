/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

#include "mocks/TaskStorageReaderMock.h"
#include "gtest/gtest.h"
#include <core/QueryInvoker.h>
#include <core/use_cases/RequestTasks.h>

using namespace dw;
using sprint_timer::use_cases::RequestTasks;
using ::testing::_;

class RequestTasksFixture : public ::testing::Test {
public:
    TaskStorageReaderMock task_reader_mock;
    sprint_timer::QueryInvoker queryInvoker;
    const DateTimeRange someTimeSpan{add_offset(
        DateTimeRange{current_date_time(), current_date_time()}, Days{-1})};
};

TEST_F(RequestTasksFixture, execute)
{
    EXPECT_CALL(task_reader_mock, requestTasks(someTimeSpan, _)).Times(1);

    queryInvoker.execute(std::make_unique<RequestTasks>(
        task_reader_mock, someTimeSpan, [](const auto& result) {}));
}
