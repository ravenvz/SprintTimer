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
#include <core/CommandInvoker.h>
#include <core/use_cases/AddNewTask.h>

using sprint_timer::entities::Tag;
using sprint_timer::entities::Task;
using sprint_timer::use_cases::AddNewTask;
using namespace dw;

class RegisterTaskFixture : public ::testing::Test {
public:
    Task someTask{"Task name",
                  4,
                  2,
                  "550e8400-e29b-41d4-a716-446655440000",
                  {Tag{"Tag1"}, Tag{"Tag2"}},
                  false,
                  DateTime{Date{Year{2015}, Month{11}, Day{10}}}};
    TaskStorageMock task_storage_mock;
    sprint_timer::CommandInvoker commandInvoker;
};

TEST_F(RegisterTaskFixture, execute_and_undo)
{
    EXPECT_CALL(task_storage_mock, save(someTask)).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<AddNewTask>(task_storage_mock, someTask));

    EXPECT_CALL(task_storage_mock, remove(someTask.uuid())).Times(1);

    commandInvoker.undo();
}
