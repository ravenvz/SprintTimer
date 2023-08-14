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
#include "qt_gui/presentation/EditTaskDialogPresenter.h"
#include "gmock/gmock.h"

using ::testing::_;
using ::testing::NiceMock;

class EditTaskDialogViewMock
    : public sprint_timer::ui::contracts::EditTaskDialogContract::View {
public:
    MOCK_METHOD(void,
                fillTaskDetails,
                (const sprint_timer::api::TaskDTO&),
                (override));

    MOCK_METHOD(void, fillTags, (std::span<const std::string>), (override));
};

class EditTaskDialogPresenterFixture : public ::testing::Test {
public:
    NiceMock<mocks::CommandHandlerMock<sprint_timer::api::EditTaskCommand>>
        editTaskHandler;
    NiceMock<mocks::QueryHandlerMock<sprint_timer::api::AllTagsQuery>>
        allTagsHandler;
    NiceMock<mocks::QueryHandlerMock<sprint_timer::api::ReadTaskTreeQuery>>
        readTaskTreeHandler;
    NiceMock<EditTaskDialogViewMock> view;
    sprint_timer::ui::EditTaskContext editTaskContext;
    sprint_timer::ui::EditTaskDialogPresenter sut{
        editTaskHandler, allTagsHandler, editTaskContext};
};

TEST_F(EditTaskDialogPresenterFixture, updates_view_when_view_is_attached)
{
    using namespace sprint_timer;
    const std::vector<std::string> tags{"Tag1", "Tag2", "Tag3", "Tag4"};
    const api::TaskDTO task{"123",
                            {"Tag1", "Tag2"},
                            "Some task name",
                            7,
                            {},
                            false,
                            dw::current_date_time_local(),
                            std::nullopt,
                            api::TaskTimeframeDTO{},
                            api::TaskTypeDTO{api::TaskTypeDTO::Regular}};
    api::TaskTreeDTO taskTree;
    taskTree.insert(
        std::ranges::find(
            taskTree, "123", [](const auto& node) { return node.uuid; }),
        task);
    ON_CALL(readTaskTreeHandler, handle(_))
        .WillByDefault(::testing::Return(taskTree));
    ON_CALL(allTagsHandler, handle(_)).WillByDefault(::testing::Return(tags));
    editTaskContext = ui::EditTaskContext{api::TaskDTO{task}};

    EXPECT_CALL(allTagsHandler, handle(_));
    EXPECT_CALL(view, fillTaskDetails(task));
    EXPECT_CALL(view, fillTags(::testing::ElementsAreArray(tags)));

    sut.attachView(view);
}

TEST_F(EditTaskDialogPresenterFixture, fires_command_on_edit_confirmation)
{
    using namespace sprint_timer;
    const sprint_timer::api::TaskDTO task{
        "123",
        {"Tag1", "Tag2"},
        "Some task name",
        7,
        {},
        false,
        dw::current_date_time_local(),
        std::nullopt,
        sprint_timer::api::TaskTimeframeDTO{},
        sprint_timer::api::TaskTypeDTO{
            sprint_timer::api::TaskTypeDTO::Regular}};
    editTaskContext = ui::EditTaskContext{api::TaskDTO{task}};
    sprint_timer::api::TaskTreeDTO taskTree;
    taskTree.insert(
        std::ranges::find(
            taskTree, "123", [](const auto& node) { return node.uuid; }),
        task);
    ON_CALL(readTaskTreeHandler, handle(_))
        .WillByDefault(::testing::Return(taskTree));

    EXPECT_CALL(editTaskHandler,
                handle(sprint_timer::api::EditTaskCommand{task}));

    sut.onEditTaskAccepted(sprint_timer::api::TaskDTO{task});
}
