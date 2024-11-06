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
#include "api/requests/AllTagsQuery.h"
#include "api/requests/CreateTaskCommand.h"
#include "common_utils/ConfigurableDateTimeProvider.h"
#include "mocks/CommandHandlerMock.h"
#include "mocks/QueryHandlerMock.h"
#include "qt_gui/presentation/AddTaskControlPresenter.h"

using namespace sprint_timer;

using sprint_timer::api::TaskTimeframeDTO;
using sprint_timer::api::TaskTypeDTO;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Truly;
using namespace dw;

class AddTaskControlViewMock : public ui::contracts::AddTaskControl::View {
public:
};

class AddTaskControlPresenterFixture : public ::testing::Test {
public:
    NiceMock<mocks::CommandHandlerMock<api::CreateTaskCommand>>
        createTaskHandler;
    NiceMock<mocks::QueryHandlerMock<api::AllTagsQuery>> allTagsHandler;
    AddTaskControlViewMock view;
    ui::AddTaskControlPresenter sut{createTaskHandler};
    DateTime pseudoCurrentTime{DateTime{Date{Year{2025}, Month{1}, Day{16}}}};
    DateTime taskStartTime{DateTime{Date{Year{2024}, Month{12}, Day{3}}}};
    TaskTimeframeDTO taskTimeframe{
        taskStartTime, std::nullopt, std::nullopt, std::nullopt};
};

TEST_F(AddTaskControlPresenterFixture, invokes_handler_to_add_task_given_dto)
{
    using namespace sprint_timer;
    const sprint_timer::api::TaskDTO details{"123",
                                             {"Tag 1", "Tag 2"},
                                             "SomeTask",
                                             4,
                                             {},
                                             false,
                                             pseudoCurrentTime,
                                             std::nullopt,
                                             taskTimeframe,
                                             TaskTypeDTO::Regular};

    EXPECT_CALL(createTaskHandler,
                handle(api::CreateTaskCommand{details.name,
                                              details.tags,
                                              details.expectedCost,
                                              TaskTypeDTO::Regular,
                                              std::nullopt,
                                              std::nullopt,
                                              std::nullopt,
                                              taskTimeframe}));

    sut.addTask(details);
}

TEST_F(AddTaskControlPresenterFixture,
       adds_task_from_decoded_description_with_all_parts_present)
{
    EXPECT_CALL(createTaskHandler,
                handle(api::CreateTaskCommand{"All parts present",
                                              {"Test"},
                                              5,
                                              TaskTypeDTO::Regular,
                                              std::nullopt,
                                              std::nullopt,
                                              std::nullopt,
                                              taskTimeframe}));

    sut.addTask("#Test All parts present *5", taskStartTime);
}

TEST_F(
    AddTaskControlPresenterFixture,
    adds_task_with_default_estimated_cost_when_it_missing_in_encoded_description)
{
    EXPECT_CALL(createTaskHandler,
                handle(api::CreateTaskCommand{"Task with tag",
                                              {"Test"},
                                              1,
                                              TaskTypeDTO::Regular,
                                              std::nullopt,
                                              std::nullopt,
                                              std::nullopt,
                                              taskTimeframe}));

    sut.addTask("#Test Task with tag", taskStartTime);
}

TEST_F(AddTaskControlPresenterFixture,
       adds_task_when_there_are_no_tags_in_encoded_description)
{
    EXPECT_CALL(createTaskHandler,
                handle(api::CreateTaskCommand{"Simple task",
                                              std::vector<std::string>{},
                                              2,
                                              TaskTypeDTO::Regular,
                                              std::nullopt,
                                              std::nullopt,
                                              std::nullopt,
                                              taskTimeframe}));

    sut.addTask("Simple task *2", taskStartTime);
}

TEST_F(AddTaskControlPresenterFixture,
       adds_task_when_there_is_no_name_in_encoded_description)
{
    EXPECT_CALL(createTaskHandler,
                handle(api::CreateTaskCommand{"",
                                              {"Tag", "Test"},
                                              44,
                                              TaskTypeDTO::Regular,
                                              std::nullopt,
                                              std::nullopt,
                                              std::nullopt,
                                              taskTimeframe}));

    sut.addTask("#Tag #Test *44", taskStartTime);
}

TEST_F(AddTaskControlPresenterFixture,
       adds_task_taking_only_last_encoded_cost_estimation)
{
    EXPECT_CALL(createTaskHandler,
                handle(api::CreateTaskCommand{"Multiple estimated",
                                              std::vector<std::string>{},
                                              9,
                                              TaskTypeDTO::Regular,
                                              std::nullopt,
                                              std::nullopt,
                                              std::nullopt,
                                              taskTimeframe}));

    sut.addTask("Multiple estimated *5 *9", taskStartTime);
}

TEST_F(AddTaskControlPresenterFixture,
       adds_task_only_treating_words_preceeded_by_single_hash_as_tags)
{
    EXPECT_CALL(createTaskHandler,
                handle(api::CreateTaskCommand{
                    "##My # ## beautiful,marvelous, great content",
                    {"tag1"},
                    1,
                    TaskTypeDTO::Regular,
                    std::nullopt,
                    std::nullopt,
                    std::nullopt,
                    taskTimeframe}));

    sut.addTask("##My #tag1  #   ##    beautiful,marvelous, great   content",
                taskStartTime);
}

