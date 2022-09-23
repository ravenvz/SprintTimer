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
#include "api/requests/RegisterSprintBulkCommand.h"
#include "api/requests/ToggleTaskCompletedCommand.h"
#include "mocks/CommandHandlerMock.h"
#include "mocks/QueryHandlerMock.h"
#include "qt_gui/presentation/TaskViewPresenter.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Truly;
using namespace sprint_timer;

class TaskViewViewMock : public ui::contracts::TaskViewContract::View {
public:
    MOCK_METHOD(void,
                selectTask,
                (const std::optional<std::string>&),
                (override));
};

class TaskSelectionColleagueMock
    : public sprint_timer::ui::TaskSelectionColleague {
public:
    MOCK_METHOD(void, onTaskSelectionChanged, (), (override));
};

class TaskViewPresenterFixture : public ::testing::Test {
public:
    ui::TaskSelectionMediator taskSelectionMediator;
    ui::TaskViewPresenter sut{taskSelectionMediator};
    TaskSelectionColleagueMock fakeColleague;
    NiceMock<TaskViewViewMock> view;
    api::TaskDTO someTask{"123",
                          {"Tag1", "Tag2"},
                          "Some name",
                          5,
                          {},
                          false,
                          dw::current_date_time_local(),
                          std::nullopt,
                          api::TaskTimeframeDTO{},
                          api::TaskTypeDTO::Regular};
};

TEST_F(TaskViewPresenterFixture,
       updates_view_with_selection_when_view_is_attached)
{
    taskSelectionMediator.changeSelection(&fakeColleague,
                                          api::TaskDTO{someTask});

    EXPECT_CALL(view, selectTask(std::optional<std::string>{someTask.uuid}));

    sut.attachView(view);
}

TEST_F(TaskViewPresenterFixture,
       notifies_task_selection_meditator_when_selected_task_changed)
{
    TaskSelectionColleagueMock taskSelectionColleague;

    sut.changeTaskSelection(api::TaskDTO{someTask});

    EXPECT_EQ(std::optional<api::TaskDTO>(someTask),
              taskSelectionMediator.currentSelection());
}

TEST_F(
    TaskViewPresenterFixture,
    updates_task_selection_in_view_when_mediator_notifies_about_selection_change)
{
    taskSelectionMediator.addColleague(&fakeColleague);
    sut.attachView(view);

    EXPECT_CALL(view, selectTask(std::optional<std::string>(someTask.uuid)));

    taskSelectionMediator.changeSelection(&fakeColleague,
                                          api::TaskDTO{someTask});
}
