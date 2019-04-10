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

#include "mocks/TaskStorageWriterMock.h"
#include "gtest/gtest.h"
#include <core/CommandInvoker.h>
#include <core/use_cases/RenameTag.h>

using sprint_timer::use_cases::RenameTag;

class RenameTagFixture : public ::testing::Test {
public:
    sprint_timer::CommandInvoker commandInvoker;
    TaskStorageWriterMock task_writer_mock;
};

TEST_F(RenameTagFixture, execute_and_undo)
{
    EXPECT_CALL(task_writer_mock, editTag("oldName", "newName")).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<RenameTag>(task_writer_mock, "oldName", "newName"));

    EXPECT_CALL(task_writer_mock, editTag("newName", "oldName")).Times(1);

    commandInvoker.undo();
}
