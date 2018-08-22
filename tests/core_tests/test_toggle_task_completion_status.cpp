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
#include <core/use_cases/ToggleTaskCompletionStatus.h>

using sprint_timer::entities::Tag;
using sprint_timer::entities::Task;
using sprint_timer::use_cases::ToggleTaskCompletionStatus;

class ToggleTaskCompletionFixture : public ::testing::Test {
public:
    Task someTask{"Task name",
                  4,
                  2,
                  "550e8400-e29b-41d4-a716-446655440000",
                  {Tag{"Tag1"}, Tag{"Tag2"}},
                  false,
                  dw::DateTime::fromYMD(2015, 11, 10)};

    sprint_timer::CommandInvoker commandInvoker;
    TaskStorageWriterMock task_writer_mock;
};

TEST_F(ToggleTaskCompletionFixture, execute)
{
    EXPECT_CALL(task_writer_mock,
                toggleTaskCompletionStatus(
                    someTask.uuid(), dw::DateTime::currentDateTimeLocal()))
        .Times(1);

    commandInvoker.executeCommand(std::make_unique<ToggleTaskCompletionStatus>(
        task_writer_mock, someTask));
}

TEST_F(ToggleTaskCompletionFixture, undo_should_not_modify_timestamp)
{
    EXPECT_CALL(task_writer_mock,
                toggleTaskCompletionStatus(
                    someTask.uuid(), dw::DateTime::currentDateTimeLocal()))
        .Times(1);

    commandInvoker.executeCommand(std::make_unique<ToggleTaskCompletionStatus>(
        task_writer_mock, someTask));

    EXPECT_CALL(
        task_writer_mock,
        toggleTaskCompletionStatus(someTask.uuid(), someTask.lastModified()))
        .Times(1);

    commandInvoker.undo();
}
