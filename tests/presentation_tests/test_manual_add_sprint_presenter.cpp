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
#include <algorithm>
#include <core/use_cases/register_sprint/RegisterSprintBulkCommand.h>
#include <qt_gui/presentation/ManualSprintAddPresenter.h>
#include <tuple>

using namespace sprint_timer;
using namespace ::testing;

class ManualAddSprintViewMock
    : public ui::contracts::ManualSprintAddContract::View {
public:
    MOCK_METHOD(void,
                displayAddSprintDialog,
                (const std::vector<entities::Task>&,
                 dw::Weekday,
                 std::chrono::minutes),
                (override));

    MOCK_METHOD(void, setInteractive, (bool), (override));
};

class ManualSprintAddPresenterFixture : public ::testing::Test {
public:
    const std::chrono::minutes sprintDuration{25};
    const dw::Weekday firstDayOfWeek{dw::Weekday::Monday};
    NiceMock<mocks::QueryHandlerMock<use_cases::UnfinishedTasksQuery,
                                     std::vector<entities::Task>>>
        unfinishedTasksHandler;
    NiceMock<mocks::CommandHandlerMock<use_cases::RegisterSprintBulkCommand>>
        registerSprintsHandler;
    ui::ManualSprintAddPresenter presenter{registerSprintsHandler,
                                           unfinishedTasksHandler,
                                           firstDayOfWeek,
                                           sprintDuration};
    NiceMock<ManualAddSprintViewMock> view;
};

Matcher<entities::Sprint> IgnoringUuid(const entities::Sprint& sprint)
{
    return AllOf(
        Property("name", &entities::Sprint::name, sprint.name()),
        Property("timeSpan", &entities::Sprint::timeSpan, sprint.timeSpan()),
        Property("taskUuid", &entities::Sprint::taskUuid, sprint.taskUuid()),
        Property("tags", &entities::Sprint::tags, sprint.tags()));
}

TEST_F(ManualSprintAddPresenterFixture, adds_sprints)
{
    using namespace std::chrono_literals;
    using namespace sprint_timer::entities;
    const std::string taskUuid{"123"};
    const dw::DateTime someDateTime{
        dw::Date{dw::Year{2020}, dw::Month{7}, dw::Day{2}}};
    const std::vector<Task> activeTasks{
        Task{"Some name",
             4,
             2,
             taskUuid,
             {Tag{"Tag1"}, Tag{"Tag2"}},
             false,
             someDateTime},
        Task{"Other task", 5, 5, {Tag{"Tag3"}}, true, someDateTime}};
    ON_CALL(unfinishedTasksHandler, handle(_))
        .WillByDefault(Return(activeTasks));

    EXPECT_CALL(
        registerSprintsHandler,
        handle(Field(
            "sprints",
            &use_cases::RegisterSprintBulkCommand::sprints,
            ElementsAre(IgnoringUuid(
                            Sprint{"Some name",
                                   dw::DateTimeRange(someDateTime + 3h,
                                                     someDateTime + 3h + 25min),
                                   {Tag{"Tag1"}, Tag{"Tag2"}},
                                   taskUuid}),
                        IgnoringUuid(
                            Sprint{"Some name",
                                   dw::DateTimeRange(someDateTime + 3h + 25min,
                                                     someDateTime + 3h + 50min),
                                   {Tag{"Tag1"}, Tag{"Tag2"}},
                                   taskUuid})))));

    presenter.onSprintAddConfirmed(0, someDateTime + 3h, 2);
}

TEST_F(ManualSprintAddPresenterFixture,
       makes_view_display_add_sprint_dialog_when_requested)
{
    const std::vector<entities::Task> tasks;
    ON_CALL(unfinishedTasksHandler, handle(_)).WillByDefault(Return(tasks));

    EXPECT_CALL(view,
                displayAddSprintDialog(tasks, firstDayOfWeek, sprintDuration));
    presenter.attachView(view);

    presenter.onSprintAddRequested();
}

TEST_F(ManualSprintAddPresenterFixture,
       makes_view_uninteractive_when_there_no_active_tasks)
{
    const std::vector<entities::Task> tasks;
    ON_CALL(unfinishedTasksHandler, handle(_)).WillByDefault(Return(tasks));

    EXPECT_CALL(view, setInteractive(false));

    presenter.attachView(view);
}

TEST_F(ManualSprintAddPresenterFixture,
       makes_view_interactive_when_there_are_active_tasks)
{
    using sprint_timer::entities::Task;
    const std::vector<Task> tasks{Task{"Task name",
                                       4,
                                       3,
                                       "123",
                                       std::list<entities::Tag>{},
                                       false,
                                       dw::current_date_time_local()}};
    ON_CALL(unfinishedTasksHandler, handle(_)).WillByDefault(Return(tasks));

    EXPECT_CALL(view, setInteractive(true));

    presenter.attachView(view);
}
