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
#include "core/use_cases/TaskMapper.h"
#include "mocks/CommandHandlerMock.h"
#include "mocks/QueryHandlerMock.h"
#include "qt_gui/presentation/ActiveTasksPresenter.h"
#include "gmock/gmock.h"
#include <algorithm>

using sprint_timer::entities::Tag;
using sprint_timer::entities::Task;
using sprint_timer::ui::ActiveTasksPresenter;
using sprint_timer::ui::contracts::TaskContract::View;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

using namespace sprint_timer::use_cases;

namespace {

using namespace std::chrono_literals;

std::vector<TaskDTO> someTaskDtos{
    TaskDTO{"1",
            {"Tag 1", "Tag 2"},
            "Task 1",
            4,
            2,
            false,
            dw::DateTime{std::chrono::system_clock::time_point{1596401779s}}},
    TaskDTO{"2",
            {"Tag 5", "Tag 2"},
            "Task 2",
            15,
            4,
            false,
            dw::DateTime{std::chrono::system_clock::time_point{1572878664s}}},
    TaskDTO{"3",
            {"Tag 7", "Tag 1"},
            "Task 3",
            1,
            0,
            false,
            dw::DateTime{std::chrono::system_clock::time_point{1614965664s}}}};

std::vector<sprint_timer::entities::Task> someTasks{
    sprint_timer::entities::Task{
        "Task 1",
        4,
        2,
        "1",
        {sprint_timer::entities::Tag{"Tag 1"},
         sprint_timer::entities::Tag{"Tag 2"}},
        false,
        dw::DateTime{std::chrono::system_clock::time_point{1596401779s}}},
    sprint_timer::entities::Task{
        "Task 2",
        15,
        4,
        "2",
        {sprint_timer::entities::Tag{"Tag 5"},
         sprint_timer::entities::Tag{"Tag 2"}},
        false,
        dw::DateTime{std::chrono::system_clock::time_point{1572878664s}}},
    sprint_timer::entities::Task{
        "Task 3",
        1,
        0,
        "3",
        {sprint_timer::entities::Tag{"Tag 7"},
         sprint_timer::entities::Tag{"Tag 1"}},
        false,
        dw::DateTime{std::chrono::system_clock::time_point{1614965664s}}}};

} // namespace

namespace sprint_timer::use_cases {

bool operator==(const use_cases::ChangeActiveTasksPriorityCommand& lhs,
                const use_cases::ChangeActiveTasksPriorityCommand& rhs);

} // namespace sprint_timer::use_cases

class TaskContractViewMock
    : public sprint_timer::ui::contracts::TaskContract::View {
public:
    MOCK_METHOD(void, displayTasks, (const std::vector<TaskDTO>&), (override));
};

class ActiveTasksPresenterFixture : public ::testing::Test {
public:
    NiceMock<TaskContractViewMock> view;
    NiceMock<mocks::QueryHandlerMock<ActiveTasksQuery>> activeTasksHandler;
    NiceMock<mocks::CommandHandlerMock<EditTaskCommand>> editTaskHandler;
    NiceMock<mocks::CommandHandlerMock<DeleteTaskCommand>> deleteTaskHandler;
    NiceMock<mocks::CommandHandlerMock<ToggleTaskCompletedCommand>>
        toggleFinishedHandler;
    NiceMock<mocks::CommandHandlerMock<ChangeActiveTasksPriorityCommand>>
        changePriorityHandler;

    ActiveTasksPresenter makePresenter()
    {
        return ActiveTasksPresenter{activeTasksHandler,
                                    editTaskHandler,
                                    deleteTaskHandler,
                                    toggleFinishedHandler,
                                    changePriorityHandler};
    }
};

TEST_F(ActiveTasksPresenterFixture, populates_task_when_view_is_attached)
{
    mocks::given_handler_returns(activeTasksHandler, someTaskDtos);
    ActiveTasksPresenter sut{makePresenter()};

    EXPECT_CALL(view, displayTasks(someTaskDtos));

    sut.attachView(view);
}

TEST_F(ActiveTasksPresenterFixture, edits_task)
{
    const auto editedDTO =
        TaskDTO{"2", {"Tag 1", "Other tag"}, "Edited name", 55, 77, false};
    ActiveTasksPresenter sut{makePresenter()};
    sut.attachView(view);

    EXPECT_CALL(editTaskHandler, handle(EditTaskCommand{editedDTO}));

    sut.editTask(editedDTO);
}

TEST_F(ActiveTasksPresenterFixture, deletes_task)
{
    ActiveTasksPresenter sut{makePresenter()};
    sut.attachView(view);
    const std::string someUuid{"123"};

    EXPECT_CALL(deleteTaskHandler, handle(DeleteTaskCommand{someUuid}));

    sut.deleteTask(someUuid);
}

TEST_F(ActiveTasksPresenterFixture, toggles_task_completion)
{
    const auto someTask = someTaskDtos[1];
    ActiveTasksPresenter sut{makePresenter()};

    EXPECT_CALL(toggleFinishedHandler,
                handle(ToggleTaskCompletedCommand{someTask.uuid,
                                                  someTask.modificationStamp}));

    sut.toggleFinished(someTask.uuid, someTask.modificationStamp);
}

TEST_F(ActiveTasksPresenterFixture,
       changes_task_order_by_moving_bottom_task_to_top)
{
    mocks::given_handler_returns(activeTasksHandler, someTaskDtos);
    ChangeActiveTasksPriorityCommand expected{
        {someTasks[0].uuid(), someTasks[1].uuid(), someTasks[2].uuid()},
        {someTasks[2].uuid(), someTasks[0].uuid(), someTasks[1].uuid()}};
    ActiveTasksPresenter sut{makePresenter()};
    sut.attachView(view);

    EXPECT_CALL(changePriorityHandler, handle(std::move(expected)));

    sut.reorderTasks(2, 1, 0);
}

TEST_F(ActiveTasksPresenterFixture,
       changes_task_order_by_moving_top_task_to_bottom)
{
    mocks::given_handler_returns(activeTasksHandler, someTaskDtos);
    ChangeActiveTasksPriorityCommand expected{
        {someTasks[0].uuid(), someTasks[1].uuid(), someTasks[2].uuid()},
        {someTasks[1].uuid(), someTasks[2].uuid(), someTasks[0].uuid()}};
    ActiveTasksPresenter sut{makePresenter()};
    sut.attachView(view);

    EXPECT_CALL(changePriorityHandler, handle(std::move(expected)));

    sut.reorderTasks(0, 1, 2);
}

namespace sprint_timer::use_cases {

bool operator==(const ChangeActiveTasksPriorityCommand& lhs,
                const ChangeActiveTasksPriorityCommand& rhs)
{
    return lhs.oldOrder == rhs.oldOrder && lhs.newOrder == rhs.newOrder;
}

} // namespace sprint_timer::use_cases
