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

#include "mocks/SprintStorageReaderMock.h"
#include "mocks/SprintStorageWriterMock.h"
#include "mocks/TaskStorageWriterMock.h"
#include "gtest/gtest.h"
#include <core/CommandInvoker.h>
#include <core/use_cases/DeleteTask.h>

#include <thread>

using ::testing::_;
using ::testing::InvokeArgument;

using sprint_timer::entities::Tag;
using sprint_timer::entities::Task;
using sprint_timer::use_cases::DeleteTask;

using namespace dw;

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
                         DateTime{Date{Year{2015}, Month{11}, Day{10}}}};
    Task taskWithNoSprints{"Task name",
                           4,
                           0,
                           "550e8400-e29b-41d4-a716-446655440000",
                           {Tag{"Tag1"}, Tag{"Tag2"}},
                           false,
                           dw::DateTime{Date{Year{2015}, Month{11}, Day{10}}}};
};

TEST_F(DeleteTaskFixture, delete_task_with_no_sprints)
{
    EXPECT_CALL(task_writer_mock, remove(taskWithNoSprints.uuid())).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<DeleteTask>(task_writer_mock,
                                     sprint_reader_mock,
                                     sprint_writer_mock,
                                     taskWithNoSprints));
}

TEST_F(DeleteTaskFixture, undo_deletion_of_task_with_no_sprints)
{
    // TODO what about lastModified timestamp when undoing task deletion? Check
    EXPECT_CALL(task_writer_mock, remove(taskWithNoSprints.uuid())).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<DeleteTask>(task_writer_mock,
                                     sprint_reader_mock,
                                     sprint_writer_mock,
                                     taskWithNoSprints));

    EXPECT_CALL(task_writer_mock, save(taskWithNoSprints)).Times(1);

    commandInvoker.undo();
}

TEST_F(DeleteTaskFixture, delete_task_with_sprints)
{
    EXPECT_CALL(sprint_reader_mock, sprintsForTask(taskWithSprints.uuid(), _))
        .WillOnce(
            InvokeArgument<1>(std::vector<sprint_timer::entities::Sprint>{}));
    EXPECT_CALL(task_writer_mock, remove(taskWithSprints.uuid())).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<DeleteTask>(task_writer_mock,
                                     sprint_reader_mock,
                                     sprint_writer_mock,
                                     taskWithSprints));
}

TEST_F(DeleteTaskFixture, undo_deletion_of_task_with_sprints)
{
    EXPECT_CALL(sprint_reader_mock, sprintsForTask(taskWithSprints.uuid(), _))
        .WillOnce(
            InvokeArgument<1>(std::vector<sprint_timer::entities::Sprint>{}));
    EXPECT_CALL(task_writer_mock, remove(taskWithSprints.uuid())).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<DeleteTask>(task_writer_mock,
                                     sprint_reader_mock,
                                     sprint_writer_mock,
                                     taskWithSprints));

    // In case of Task with sprints, we must make sure that task is restored
    // with zeroed-out actualCost, because when restoring sprints, actualCost
    // will be also modified.
    Task taskWithZeroedActualCost = taskWithSprints;
    taskWithZeroedActualCost.setActualCost(0);
    EXPECT_CALL(task_writer_mock, save(taskWithZeroedActualCost)).Times(1);
    EXPECT_CALL(sprint_writer_mock,
                save(std::vector<sprint_timer::entities::Sprint>{}))
        .Times(1);

    commandInvoker.undo();
}
