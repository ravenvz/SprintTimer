/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include "core/ObservableActionInvoker.h"
#include "core/actions/EditTask.h"
#include "mocks/TaskStorageMock.h"
#include "gtest/gtest.h"

using sprint_timer::actions::EditTask;
using sprint_timer::entities::Tag;
using sprint_timer::entities::Task;
using namespace dw;

class EditTaskFixture : public ::testing::Test {
public:
    Task someTask{"Task name",
                  4,
                  2,
                  "550e8400-e29b-41d4-a716-446655440000",
                  {Tag{"Tag1"}, Tag{"Tag2"}},
                  false,
                  DateTime{Date{Year{2015}, Month{11}, Day{10}}}};

    mocks::TaskStorageMock task_storage_mock;
    sprint_timer::ObservableActionInvoker actionInvoker;
};

TEST_F(EditTaskFixture, should_only_alter_allowed_parameters)
{
    const Task editedTask{"Edited",
                          someTask.estimatedCost() + 3,
                          someTask.actualCost() + 2,
                          someTask.uuid(),
                          {Tag{"Tag2"}, Tag{"NewTag"}},
                          !someTask.isCompleted(),
                          dw::current_date_time_local()};
    const Task restrictedTask{
        editedTask.name(),
        editedTask.estimatedCost(),
        someTask.actualCost(), // Should not be editable
        someTask.uuid(),       // Should not be editable
        editedTask.tags(),
        someTask.isCompleted(),
        dw::current_date_time_local()}; // Should not be editable

    EXPECT_CALL(task_storage_mock, edit(someTask, restrictedTask));

    actionInvoker.execute(
        std::make_unique<EditTask>(task_storage_mock, someTask, editedTask));
}

TEST_F(EditTaskFixture, undo)
{
    const Task editedTask{"Edited",
                          someTask.estimatedCost() + 2,
                          someTask.actualCost(),
                          someTask.uuid(),
                          {Tag{"NewTag"}},
                          someTask.isCompleted(),
                          dw::current_date_time_local()};
    EXPECT_CALL(task_storage_mock, edit(someTask, editedTask)).Times(1);

    actionInvoker.execute(
        std::make_unique<EditTask>(task_storage_mock, someTask, editedTask));

    EXPECT_CALL(task_storage_mock, edit(editedTask, someTask));

    actionInvoker.undo();
}
