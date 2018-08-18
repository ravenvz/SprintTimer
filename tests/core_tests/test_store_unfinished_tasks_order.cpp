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

// TODO remove when Gtest drops std::tr1
// Workaround for C++17 as std::tr1 no longer available and Gtest uses it
#define GTEST_LANG_CXX11 1

#include "mocks/TaskStorageWriterMock.h"
#include "gtest/gtest.h"
#include <core/CommandInvoker.h>
#include <core/use_cases/StoreUnfinishedTasksOrder.h>

using namespace sprint_timer;
using namespace sprint_timer::use_cases;

class StoreUnfinishedTasksOrderFixture : public ::testing::Test {
public:
    TaskStorageWriterMock task_writer_mock;
    CommandInvoker commandInvoker;
};

TEST_F(StoreUnfinishedTasksOrderFixture, test_name)
{
    const std::vector<std::string> new_priorities{"2", "1", "3"};
    EXPECT_CALL(task_writer_mock, updatePriorities(new_priorities)).Times(1);

    commandInvoker.executeCommand(std::make_unique<StoreUnfinishedTasksOrder>(
        task_writer_mock,
        std::vector<std::string>{"1", "2", "3"},
        std::vector<std::string>{"2", "1", "3"}));
}

TEST_F(StoreUnfinishedTasksOrderFixture, undo_changing_tasks_order)
{
    const std::vector<std::string> old_priorities{"1", "2", "3"};
    const std::vector<std::string> new_priorities{"2", "1", "3"};
    EXPECT_CALL(task_writer_mock, updatePriorities(new_priorities)).Times(1);

    commandInvoker.executeCommand(std::make_unique<StoreUnfinishedTasksOrder>(
        task_writer_mock,
        std::vector<std::string>{"1", "2", "3"},
        std::vector<std::string>{"2", "1", "3"}));

    EXPECT_CALL(task_writer_mock, updatePriorities(old_priorities)).Times(1);
    commandInvoker.undo();
}
