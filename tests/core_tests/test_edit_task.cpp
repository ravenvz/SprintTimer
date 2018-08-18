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
#include <core/TaskBuilder.h>
#include <core/use_cases/EditTask.h>

using sprint_timer::TaskBuilder;
using sprint_timer::entities::Tag;
using sprint_timer::entities::Task;
using sprint_timer::use_cases::EditTask;

class EditTaskFixture : public ::testing::Test {
public:
    Task someTask{"Task name",
                  4,
                  2,
                  "550e8400-e29b-41d4-a716-446655440000",
                  {Tag{"Tag1"}, Tag{"Tag2"}},
                  false,
                  dw::DateTime::fromYMD(2015, 11, 10)};

    TaskStorageWriterMock task_writer_mock;
    sprint_timer::CommandInvoker commandInvoker;
};

TEST_F(EditTaskFixture, should_only_alter_allowed_parameters)
{
    Task editedTask = TaskBuilder{}
                          .withName("Edited")
                          .withEstimatedCost(someTask.estimatedCost() + 3)
                          .withActualCost(someTask.actualCost() + 2)
                          .withCompletionStatus(!someTask.isCompleted())
                          .withExplicitTags({Tag{"Tag2"}, Tag{"New Tag"}})
                          .build();
    Task restrictedTask
        = TaskBuilder{}
              .withUuid(someTask.uuid()) // Should not be editable
              .withActualCost(someTask.actualCost()) // Should not be editable
              .withCompletionStatus(
                  someTask.isCompleted()) // Should not be editable
              .withName(editedTask.name())
              .withEstimatedCost(editedTask.estimatedCost())
              .withExplicitTags(editedTask.tags())
              .build();
    EXPECT_CALL(task_writer_mock, edit(someTask, restrictedTask)).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<EditTask>(task_writer_mock, someTask, editedTask));
}

TEST_F(EditTaskFixture, undo)
{
    Task editedTask = TaskBuilder{}
                          .withUuid(someTask.uuid())
                          .withExplicitTags({Tag{"New tag"}})
                          .withName("Edited")
                          .withActualCost(someTask.actualCost())
                          .withEstimatedCost(someTask.estimatedCost() + 2)
                          .build();
    EXPECT_CALL(task_writer_mock, edit(someTask, editedTask)).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<EditTask>(task_writer_mock, someTask, editedTask));

    EXPECT_CALL(task_writer_mock, edit(editedTask, someTask));

    commandInvoker.undo();
}
