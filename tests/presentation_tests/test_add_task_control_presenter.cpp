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
#include "mocks/CommandHandlerMock.h"
#include "mocks/QueryHandlerMock.h"
#include <core/use_cases/create_task/CreateTaskCommand.h>
#include <core/use_cases/request_tags/AllTagsQuery.h>
#include <qt_gui/presentation/AddTaskControlPresenter.h>

using namespace sprint_timer;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Truly;

class AddTaskControlViewMock : public ui::contracts::AddTaskControl::View {
public:
};

class AddTaskControlPresenterFixture : public ::testing::Test {
public:
    NiceMock<mocks::CommandHandlerMock<use_cases::CreateTaskCommand>>
        createTaskHandler;
    NiceMock<mocks::QueryHandlerMock<use_cases::AllTagsQuery,
                                     std::vector<std::string>>>
        allTagsHandler;
    AddTaskControlViewMock view;
    ui::AddTaskControlPresenter sut{createTaskHandler};
};

namespace {

const dw::DateTime someModificationStamp{dw::current_date_time()};

std::pair<sprint_timer::entities::Task, sprint_timer::ui::TaskDTO>
someTaskWithDescription();

/* This matcher ignores uuid and modification timestamp */
struct MatchesCreateTaskCommandWithoutUuid {
    MatchesCreateTaskCommandWithoutUuid(
        const sprint_timer::use_cases::CreateTaskCommand& expectedCommand_);

    bool
    operator()(const sprint_timer::use_cases::CreateTaskCommand& command) const;

private:
    const sprint_timer::use_cases::CreateTaskCommand& expectedCommand;
};

} // namespace

TEST_F(AddTaskControlPresenterFixture, invokes_handler_to_add_task_given_dto)
{
    using namespace sprint_timer::entities;
    const auto [task, details] = someTaskWithDescription();
    const use_cases::CreateTaskCommand expectedCommand{task};

    EXPECT_CALL(
        createTaskHandler,
        handle(Truly(MatchesCreateTaskCommandWithoutUuid{expectedCommand})));

    sut.addTask(details);
}

TEST_F(AddTaskControlPresenterFixture,
       invokes_handler_to_add_task_given_task_description)
{
    using namespace sprint_timer::entities;
    const use_cases::CreateTaskCommand command{
        Task{"All parts present",
             5,
             0,
             {Tag{"Test"}},
             false,
             dw::current_date_time_local()}};

    EXPECT_CALL(createTaskHandler,
                handle(Truly(MatchesCreateTaskCommandWithoutUuid{command})));

    sut.addTask("#Test All parts present *5");
}

namespace {

std::pair<sprint_timer::entities::Task, sprint_timer::ui::TaskDTO>
someTaskWithDescription()
{
    using namespace sprint_timer::entities;
    const ui::TaskDTO details{"123",
                              {"Tag 1", "Tag 2"},
                              "SomeTask",
                              4,
                              0,
                              false,
                              someModificationStamp};
    Task task{"SomeTask",
              4,
              0,
              "123",
              {Tag{"Tag 1"}, Tag{"Tag 2"}},
              false,
              someModificationStamp};
    return {task, details};
}

MatchesCreateTaskCommandWithoutUuid::MatchesCreateTaskCommandWithoutUuid(
    const sprint_timer::use_cases::CreateTaskCommand& expectedCommand_)
    : expectedCommand{expectedCommand_}
{
}

bool MatchesCreateTaskCommandWithoutUuid::operator()(
    const sprint_timer::use_cases::CreateTaskCommand& command) const
{
    const auto& expected = expectedCommand.task;
    const auto& actual = command.task;
    return expected.name() == actual.name() &&
           expected.estimatedCost() == actual.estimatedCost() &&
           expected.actualCost() == actual.actualCost() &&
           expected.isCompleted() == actual.isCompleted() &&
           expected.tags() == actual.tags() &&
           expected.lastModified() == actual.lastModified();
}

} // namespace
