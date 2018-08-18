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

#include "mocks/SprintStorageReaderMock.h"
#include "mocks/SprintStorageWriterMock.h"
#include "mocks/TaskStorageWriterMock.h"
#include "gtest/gtest.h"
#include <core/CommandInvoker.h>
#include <core/use_cases/DeleteTask.h>

using ::testing::_;

using sprint_timer::entities::Tag;
using sprint_timer::entities::Task;
using sprint_timer::use_cases::DeleteTask;

class DeleteTaskFixture : public ::testing::Test {
public:
    TaskStorageWriterMock task_writer_mock;
    SprintStorageWriterMock sprint_writer_mock;
    SprintStorageReaderMock sprint_reader_mock;
    sprint_timer::CommandInvoker commandInvoker;

    Task taskWithSprints{"Task name",
                         4,
                         2,
                         "550e8400-e29b-41d4-a716-446655440000",
                         {Tag{"Tag1"}, Tag{"Tag2"}},
                         false,
                         dw::DateTime::fromYMD(2015, 11, 10)};
    Task taskWithNoSprints{"Task name",
                           4,
                           0,
                           "550e8400-e29b-41d4-a716-446655440000",
                           {Tag{"Tag1"}, Tag{"Tag2"}},
                           false,
                           dw::DateTime::fromYMD(2015, 11, 10)};
};

TEST_F(DeleteTaskFixture, delete_task_with_no_sprints)
{
    EXPECT_CALL(task_writer_mock, remove(taskWithNoSprints)).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<DeleteTask>(task_writer_mock,
                                     sprint_reader_mock,
                                     sprint_writer_mock,
                                     taskWithNoSprints));
}

TEST_F(DeleteTaskFixture, undo_deletion_of_task_with_no_sprints)
{
    // TODO what about lastModified timestamp when undoing task deletion? Check
    EXPECT_CALL(task_writer_mock, remove(taskWithNoSprints)).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<DeleteTask>(task_writer_mock,
                                     sprint_reader_mock,
                                     sprint_writer_mock,
                                     taskWithNoSprints));

    EXPECT_CALL(task_writer_mock, save(taskWithNoSprints)).Times(1);

    commandInvoker.undo();
}

// TODO figure out how to test it when
// PM-135-deletion-of-task-triggers-infinite-loop is fixed
TEST_F(DeleteTaskFixture, DISABLED_delete_task_with_sprints)
{
    EXPECT_CALL(sprint_reader_mock, sprintsForTask(taskWithSprints.uuid(), _))
        .Times(1);
    EXPECT_CALL(task_writer_mock, remove(taskWithSprints)).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<DeleteTask>(task_writer_mock,
                                     sprint_reader_mock,
                                     sprint_writer_mock,
                                     taskWithSprints));
}

TEST_F(DeleteTaskFixture, DISABLED_undo_deletion_of_task_with_sprints) {}
