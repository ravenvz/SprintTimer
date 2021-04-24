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
#include "core/use_cases/create_task/CreateTaskCommand.h"
#include "core/use_cases/request_tags/AllTagsQuery.h"
#include "mocks/CommandHandlerMock.h"
#include "mocks/QueryHandlerMock.h"
#include "qt_gui/presentation/AddTaskControlPresenter.h"

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
    NiceMock<mocks::QueryHandlerMock<use_cases::AllTagsQuery>> allTagsHandler;
    AddTaskControlViewMock view;
    ui::AddTaskControlPresenter sut{createTaskHandler};
};

namespace {

const dw::DateTime someModificationStamp{dw::current_date_time()};

std::pair<sprint_timer::entities::Task, sprint_timer::use_cases::TaskDTO>
someTaskWithDescription();

} // namespace

TEST_F(AddTaskControlPresenterFixture, invokes_handler_to_add_task_given_dto)
{
    using namespace sprint_timer::entities;
    const auto [task, details] = someTaskWithDescription();

    EXPECT_CALL(createTaskHandler,
                handle(use_cases::CreateTaskCommand{
                    details.name, details.tags, details.expectedCost}));

    sut.addTask(details);
}

TEST_F(AddTaskControlPresenterFixture,
       adds_task_from_decoded_description_with_all_parts_present)
{
    EXPECT_CALL(
        createTaskHandler,
        handle(use_cases::CreateTaskCommand{"All parts present", {"Test"}, 5}));

    sut.addTask("#Test All parts present *5");
}

TEST_F(AddTaskControlPresenterFixture,
       adds_task_with_default_estimated_cost_when_it_missing_in_encoded_description)
{
    EXPECT_CALL(
        createTaskHandler,
        handle(use_cases::CreateTaskCommand{"Task with tag", {"Test"}, 1}));

    sut.addTask("#Test Task with tag");
}

TEST_F(AddTaskControlPresenterFixture,
       adds_task_when_there_are_no_tags_in_encoded_description)
{
    EXPECT_CALL(
        createTaskHandler,
        handle(use_cases::CreateTaskCommand{"Simple task", std::vector<std::string>{}, 2}));

    sut.addTask("Simple task *2");
}

TEST_F(AddTaskControlPresenterFixture,
       adds_task_when_there_is_no_name_in_encoded_description)
{
    EXPECT_CALL(
        createTaskHandler,
        handle(use_cases::CreateTaskCommand{"", {"Tag", "Test"}, 44}));

    sut.addTask("#Tag #Test *44");
}

TEST_F(AddTaskControlPresenterFixture,
       adds_task_taking_only_last_encoded_cost_estimation)
{
    EXPECT_CALL(
        createTaskHandler,
        handle(use_cases::CreateTaskCommand{"Multiple estimated", std::vector<std::string>{}, 9}));

    sut.addTask("Multiple estimated *5 *9");
}

TEST_F(AddTaskControlPresenterFixture,
       adds_task_only_treating_words_preceeded_by_single_hash_as_tags)
{
    EXPECT_CALL(
        createTaskHandler,
        handle(use_cases::CreateTaskCommand{"##My # ## beautiful,marvelous, great content", {"tag1"}, 1}));

    sut.addTask("##My #tag1  #   ##    beautiful,marvelous, great   content");
}

namespace {

std::pair<sprint_timer::entities::Task, sprint_timer::use_cases::TaskDTO>
someTaskWithDescription()
{
    using namespace sprint_timer::entities;
    const sprint_timer::use_cases::TaskDTO details{"123",
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

} // namespace
