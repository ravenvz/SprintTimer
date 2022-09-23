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
#include "mocks/CommandHandlerMock.h"
#include "mocks/QueryHandlerMock.h"
#include "mocks/TaskSelectionColleagueMock.h"
#include "qt_gui/presentation/AddTaskDialogPresenter.h"
#include "qt_gui/presentation/EditTaskDialogPresenter.h"
#include "gmock/gmock.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

namespace {

/*
 *    folder1
 *        project1
 *            task1
 *            task3
 *            project2
 *    task2
 *
 *
 */
auto createTaskTree() -> sprint_timer::api::TaskTreeDTO;

} // namespace

class AddTaskDialogViewMock
    : public sprint_timer::ui::contracts::AddTaskDialogContract::View {
public:
    MOCK_METHOD(void,
                fillParentData,
                (const std::optional<std::string>&,
                 const std::optional<int64_t>&),
                (override));
    MOCK_METHOD(void, fillTags, (std::span<const std::string>), (override));
};

#include "gtest/gtest.h"

class AddTaskDialogPresenterFixture : public ::testing::Test {
public:
    NiceMock<mocks::CommandHandlerMock<sprint_timer::api::CreateTaskCommand>>
        createTaskHandler;
    NiceMock<mocks::QueryHandlerMock<sprint_timer::api::AllTagsQuery>>
        allTagsHandler;
    NiceMock<mocks::QueryHandlerMock<sprint_timer::api::ReadTaskTreeQuery>>
        readTaskTreeHandler;
    NiceMock<mocks::TaskSelectionColleagueMock> taskSelectionColleague;
    sprint_timer::ui::AddTaskContext addTaskContext;

    AddTaskDialogViewMock view;
    sprint_timer::ui::AddTaskDialogPresenter sut{
        createTaskHandler, allTagsHandler, readTaskTreeHandler, addTaskContext};
};

TEST_F(AddTaskDialogPresenterFixture,
       finds_parent_and_position_after_sibling_when_adding_sibling_task)
{
    const std::vector<std::string> tags{"ProjectTag1", "Tag1", "Tag2", "Tag3"};
    ON_CALL(allTagsHandler, handle(_)).WillByDefault(::testing::Return(tags));
    ON_CALL(readTaskTreeHandler, handle(_))
        .WillByDefault(testing::Return(createTaskTree()));
    addTaskContext = sprint_timer::ui::AddTaskContext{
        "5", sprint_timer::ui::TaskAddMode::Sibling};

    EXPECT_CALL(view, fillTags(::testing::ElementsAreArray(tags)));
    EXPECT_CALL(view,
                fillParentData(std::optional<std::string>{"1"},
                               std::optional<int64_t>{2}));

    sut.attachView(view);
}

TEST_F(AddTaskDialogPresenterFixture,
       fills_data_to_insert_subtask_on_last_postion)
{
    const std::vector<std::string> tags{"ProjectTag1", "Tag1", "Tag2", "Tag3"};
    ON_CALL(allTagsHandler, handle(_)).WillByDefault(::testing::Return(tags));
    ON_CALL(readTaskTreeHandler, handle(_))
        .WillByDefault(testing::Return(createTaskTree()));
    addTaskContext = sprint_timer::ui::AddTaskContext{
        "1", sprint_timer::ui::TaskAddMode::Subtask};

    EXPECT_CALL(view, fillTags(::testing::ElementsAreArray(tags)));
    EXPECT_CALL(view,
                fillParentData(std::optional<std::string>{"1"},
                               std::optional<int64_t>{3}));

    sut.attachView(view);
}

TEST_F(AddTaskDialogPresenterFixture, fires_command_on_confirmation)
{
    using namespace sprint_timer::api;
    // TaskTreeDTO taskTree;
    EXPECT_CALL(
        createTaskHandler,
        handle(sprint_timer::api::CreateTaskCommand{"Some task name",
                                                    {"Tag1", "Tag2"},
                                                    7,
                                                    TaskTypeDTO::Regular,
                                                    "123",
                                                    2,
                                                    NoteDTO{"Tas notes"},
                                                    TaskTimeframeDTO{}}));

    sut.onTaskCreationAccepted("Some task name",
                               {"Tag1", "Tag2"},
                               7,
                               TaskTypeDTO::Regular,
                               "123",
                               2,
                               NoteDTO{"Tas notes"},
                               TaskTimeframeDTO{});
}

namespace {

/*
 *    folder1
 *        project1
 *            task1
 *            task3
 *            project2
 *    task2
 *
 *
 */
auto createTaskTree() -> sprint_timer::api::TaskTreeDTO
{
    using namespace sprint_timer::api;
    using Tags = std::vector<std::string>;
    TaskTreeDTO tree;
    tree.addChild("0",
                  TaskDTO{"0",
                          Tags{},
                          "folder1",
                          0,
                          std::vector<dw::DateTimeRange>{},
                          false,
                          dw::current_date_time_local(),
                          std::nullopt,
                          TaskTimeframeDTO{},
                          TaskTypeDTO::Folder},
                  std::nullopt);
    tree.addChild("1",
                  TaskDTO{"1",
                          Tags{"ProjectTag1"},
                          "project1",
                          5,
                          std::vector<dw::DateTimeRange>{},
                          false,
                          dw::current_date_time_local(),
                          std::nullopt,
                          TaskTimeframeDTO{},
                          TaskTypeDTO::Project},
                  "0");
    tree.addChild("2",
                  TaskDTO{"2",
                          Tags{"Tag1", "Tag2"},
                          "task1",
                          7,
                          std::vector<dw::DateTimeRange>{},
                          false,
                          dw::current_date_time_local(),
                          std::nullopt,
                          TaskTimeframeDTO{},
                          TaskTypeDTO::Regular},
                  "1");
    tree.addChild("3",
                  TaskDTO{"3",
                          Tags{},
                          "project2",
                          20,
                          std::vector<dw::DateTimeRange>{},
                          false,
                          dw::current_date_time_local(),
                          std::nullopt,
                          TaskTimeframeDTO{},
                          TaskTypeDTO::Project},
                  "1");
    tree.addChild("4",
                  TaskDTO{"4",
                          Tags{"Tag1"},
                          "task2",
                          10,
                          std::vector<dw::DateTimeRange>{},
                          false,
                          dw::current_date_time_local(),
                          std::nullopt,
                          TaskTimeframeDTO{},
                          TaskTypeDTO::Regular},
                  std::nullopt);
    tree.addChild("5",
                  TaskDTO{"5",
                          Tags{"Tag1", "Tag3"},
                          "task3",
                          4,
                          std::vector<dw::DateTimeRange>{},
                          false,
                          dw::current_date_time_local(),
                          std::nullopt,
                          TaskTimeframeDTO{},
                          TaskTypeDTO::Regular},
                  "1",
                  1);

    return tree;
}

} // namespace

