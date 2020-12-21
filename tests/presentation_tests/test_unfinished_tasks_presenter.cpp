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
#include <qt_gui/presentation/UnfinishedTasksPresenter.h>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Truly;

namespace {

auto sameIgnoringUuid = [](const auto& lhs, const auto& rhs) {
    return lhs.name() == rhs.name() && lhs.timeSpan() == rhs.timeSpan() &&
           lhs.taskUuid() == rhs.taskUuid() && lhs.tags() == rhs.tags();
};

struct MatchesRegisterSprintBulkCommand {
    MatchesRegisterSprintBulkCommand(
        const std::vector<sprint_timer::entities::Sprint>& sprints_)
        : sprints{sprints_}
    {
    }

    bool operator()(
        const sprint_timer::use_cases::RegisterSprintBulkCommand& command) const
    {
        return std::equal(cbegin(sprints),
                          cend(sprints),
                          cbegin(command.sprints),
                          cend(command.sprints),
                          sameIgnoringUuid);
    }

private:
    const std::vector<sprint_timer::entities::Sprint> sprints;
};

std::vector<sprint_timer::entities::Sprint>
makeConsecutiveSprints(const sprint_timer::entities::Task& task,
                       size_t numSprints,
                       dw::DateTimeRange startingRange = {
                           dw::current_date_time(), dw::current_date_time()})
{
    using namespace std::chrono_literals;
    using namespace sprint_timer::entities;
    std::vector<Sprint> result;
    size_t sprintUuid{1};
    result.reserve(numSprints);
    for (size_t i = 0; i < numSprints; ++i) {
        result.emplace_back(task.name(),
                            startingRange,
                            task.tags(),
                            std::to_string(sprintUuid++),
                            task.uuid());
        startingRange = dw::add_offset(startingRange, 25min);
    }
    return result;
}

} // namespace

using namespace sprint_timer;

class UnfinishedTasksViewMock
    : public ui::contracts::UnfinishedTasksContract::View {
public:
    MOCK_METHOD(void,
                displayTasks,
                (const std::vector<entities::Task>&),
                (override));

    MOCK_METHOD(void, selectTask, (size_t), (override));
};

namespace sprint_timer::use_cases {

bool operator==(const use_cases::DeleteTaskCommand& lhs,
                const use_cases::DeleteTaskCommand& rhs)
{
    return lhs.task == rhs.task;
}

bool operator==(const EditTaskCommand& lhs, const EditTaskCommand& rhs)
{
    return lhs.originalTask == rhs.originalTask &&
           lhs.editedTask == rhs.editedTask;
}

} // namespace sprint_timer::use_cases

class UnfinishedTasksPresenterFixture : public ::testing::Test {
public:
    mocks::QueryHandlerMock<use_cases::UnfinishedTasksQuery,
                            std::vector<entities::Task>>
        unfinishedTasksHandler;
    mocks::CommandHandlerMock<use_cases::DeleteTaskCommand> deleteTaskHandler;
    mocks::CommandHandlerMock<use_cases::EditTaskCommand> editTaskHandler;
    mocks::CommandHandlerMock<use_cases::RegisterSprintBulkCommand>
        registerSprintBulkHandler;
    mocks::CommandHandlerMock<use_cases::ToggleTaskCompletedCommand>
        toggleTaskCompletionHandler;
    ui::UnfinishedTasksPresenter presenter{unfinishedTasksHandler,
                                           deleteTaskHandler,
                                           editTaskHandler,
                                           registerSprintBulkHandler,
                                           toggleTaskCompletionHandler};
    NiceMock<UnfinishedTasksViewMock> view;
    entities::Task someTask1{
        "Name", 4, 2, {entities::Tag{"Tag 1"}, entities::Tag{"Tag 2"}}, false};
    entities::Task someTask{"Task name",
                            5,
                            2,
                            {entities::Tag{"Tag1"}, entities::Tag{"Tag2"}},
                            false,
                            dw::current_date_time()};
    entities::Task otherTask{
        "Other task name",
        2,
        1,
        {sprint_timer::entities::Tag{"Tag3"}, entities::Tag{"Tag1"}},
        false,
        dw::current_date_time()};
    std::vector<entities::Task> unfinishedTasks{otherTask, someTask};
};

TEST_F(UnfinishedTasksPresenterFixture,
       updates_view_with_tasks_when_view_is_attached)
{
    const std::vector<entities::Task> tasks{someTask};
    ON_CALL(unfinishedTasksHandler, handle(_)).WillByDefault(Return(tasks));
    ui::UnfinishedTasksPresenter presenter_{unfinishedTasksHandler,
                                            deleteTaskHandler,
                                            editTaskHandler,
                                            registerSprintBulkHandler,
                                            toggleTaskCompletionHandler};
    EXPECT_CALL(view, displayTasks(tasks));

    presenter_.attachView(view);
}

TEST_F(UnfinishedTasksPresenterFixture, fires_delete_task_command)
{
    presenter.attachView(view);
    EXPECT_CALL(deleteTaskHandler,
                handle(use_cases::DeleteTaskCommand{someTask}));

    presenter.onTaskDelete(someTask);
}

TEST_F(UnfinishedTasksPresenterFixture, fires_edit_task_command)
{
    presenter.attachView(view);
    entities::Task originalTask = someTask;
    entities::Task editedTask{"Edited name",
                              someTask.estimatedCost(),
                              someTask.actualCost(),
                              someTask.tags(),
                              someTask.isCompleted()};
    auto originalCopy = originalTask;
    auto editedCopy = editedTask;
    EXPECT_CALL(editTaskHandler,
                handle(use_cases::EditTaskCommand{std::move(originalCopy),
                                                  std::move(editedCopy)}));

    presenter.onTaskEdit(std::move(originalTask), std::move(editedTask));
}

TEST_F(UnfinishedTasksPresenterFixture,
       notifies_mediator_when_tag_editor_requested)
{
    FAIL();
}

TEST_F(UnfinishedTasksPresenterFixture,
       notifies_mediator_when_sprints_for_task_are_requested)
{
    FAIL();
}

TEST_F(UnfinishedTasksPresenterFixture,
       notifies_mediator_when_task_selection_is_changed)
{
    FAIL();
    // TODO(vizier): note that this test should be removed if decision to reuse
    // presenter for both views is employed
}

TEST_F(UnfinishedTasksPresenterFixture,
       adds_task_when_encoded_task_string_received)
{
    FAIL();
}

TEST_F(UnfinishedTasksPresenterFixture, notifies_mediator_when_add_task_invoked)
{
    FAIL();
}

TEST_F(UnfinishedTasksPresenterFixture, toggles_task_completion)
{
    using namespace std::chrono_literals;
    using namespace sprint_timer::entities;
    ON_CALL(unfinishedTasksHandler, handle(_))
        .WillByDefault(Return(unfinishedTasks));
    ui::UnfinishedTasksPresenter sut{unfinishedTasksHandler,
                                     deleteTaskHandler,
                                     editTaskHandler,
                                     registerSprintBulkHandler,
                                     toggleTaskCompletionHandler};
    sut.attachView(view);
    auto matchesToggleTaskCompletionCommand =
        [this](const use_cases::ToggleTaskCompletedCommand& command) {
            return someTask == command.task;
        };

    EXPECT_CALL(toggleTaskCompletionHandler,
                handle(Truly(matchesToggleTaskCompletionCommand)));

    sut.onToggleTaskComplete(1);
}

TEST_F(UnfinishedTasksPresenterFixture,
       when_task_selection_changes_notifies_other_views)
{
    UnfinishedTasksViewMock initiatingView;
    UnfinishedTasksViewMock viewA;
    UnfinishedTasksViewMock viewB;
    presenter.attachView(initiatingView);
    presenter.attachView(viewA);
    presenter.attachView(viewB);
    const size_t selectionIndex{3};
    EXPECT_CALL(initiatingView, selectTask(selectionIndex)).Times(0);
    EXPECT_CALL(viewA, selectTask(selectionIndex));
    EXPECT_CALL(viewB, selectTask(selectionIndex));

    presenter.onTaskSelectionChanged(initiatingView, selectionIndex);
}

TEST_F(UnfinishedTasksPresenterFixture, fires_register_sprint_bulk_command)
{
    using namespace std::chrono_literals;
    using namespace sprint_timer::entities;
    ON_CALL(unfinishedTasksHandler, handle(_))
        .WillByDefault(Return(unfinishedTasks));
    ui::UnfinishedTasksPresenter presenter_{unfinishedTasksHandler,
                                            deleteTaskHandler,
                                            editTaskHandler,
                                            registerSprintBulkHandler,
                                            toggleTaskCompletionHandler};
    presenter_.attachView(view);
    dw::DateTimeRange someTimeRange{dw::current_date_time(),
                                    dw::current_date_time()};
    auto sprints = makeConsecutiveSprints(someTask, 2, someTimeRange);

    EXPECT_CALL(registerSprintBulkHandler,
                handle(Truly(MatchesRegisterSprintBulkCommand{sprints})));

    const size_t taskIndex{1};
    presenter_.onRegisterSprints(
        taskIndex, {someTimeRange, dw::add_offset(someTimeRange, 25min)});
}
