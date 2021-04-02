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
#include <core/use_cases/register_sprint/RegisterSprintBulkCommand.h>
#include <core/use_cases/toggle_task_completed/ToggleTaskCompletedCommand.h>
#include <qt_gui/presentation/TaskViewPresenter.h>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Truly;

namespace {

// std::vector<sprint_timer::entities::Sprint>
// makeConsecutiveSprints(const sprint_timer::entities::Task& task,
//                        size_t numSprints,
//                        dw::DateTimeRange startingRange = {
//                            dw::current_date_time(), dw::current_date_time()})
// {
//     using namespace std::chrono_literals;
//     using namespace sprint_timer::entities;
//     std::vector<Sprint> result;
//     size_t sprintUuid{1};
//     result.reserve(numSprints);
//     for (size_t i = 0; i < numSprints; ++i) {
//         result.emplace_back(task.name(),
//                             startingRange,
//                             task.tags(),
//                             std::to_string(sprintUuid++),
//                             task.uuid());
//         startingRange = dw::add_offset(startingRange, 25min);
//     }
//     return result;
// }

} // namespace

using namespace sprint_timer;

class TaskViewViewMock : public ui::contracts::TaskViewContract::View {
public:
    MOCK_METHOD(void, selectTask, (std::optional<size_t>), (override));
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
};

TEST_F(TaskViewPresenterFixture,
       updates_view_with_selection_when_view_is_attached)
{
    taskSelectionMediator.changeSelection(&fakeColleague, 2, "123");

    EXPECT_CALL(view, selectTask(std::optional<size_t>{2}));

    sut.attachView(view);
}

TEST_F(TaskViewPresenterFixture,
       notifies_task_selection_meditator_when_selected_task_changed)
{
    const size_t selectionIndex{3};
    std::string uuid{"123"};
    TaskSelectionColleagueMock taskSelectionColleague;

    sut.changeTaskSelection(selectionIndex, std::move(uuid));

    EXPECT_EQ(std::optional<size_t>(3), taskSelectionMediator.taskIndex());
    EXPECT_EQ(std::optional<std::string>("123"),
              taskSelectionMediator.taskUuid());
}

TEST_F(
    TaskViewPresenterFixture,
    updates_task_selection_in_view_when_mediator_notifies_about_selection_change)
{
    const size_t taskIndex{22};
    std::string taskUuid{"123"};
    taskSelectionMediator.addColleague(&fakeColleague);
    sut.attachView(view);

    EXPECT_CALL(view, selectTask(std::optional<size_t>(taskIndex)));

    taskSelectionMediator.changeSelection(
        &fakeColleague, taskIndex, std::move(taskUuid));
}
