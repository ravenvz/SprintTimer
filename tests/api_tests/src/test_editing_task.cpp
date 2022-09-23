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
#include "api/HandlerException.h"
#include "api_tests/QtStorageInitializer.h"
#include "api_tests/fixtures/TaskTreeFixture.h"
#include "gtest/gtest.h"

using namespace sprint_timer;
using namespace sprint_timer::api;
using namespace sprint_timer;
using namespace sprint_timer::compose;
using namespace dw;

class EditingTaskFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    asp::CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    asp::QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
    asp::CommandHandler<EditTaskCommand>& editTaskHandler{
        commandComposer.editTaskHandler()};
    asp::CommandHandler<UndoLastCommand>& undoHandler{
        commandComposer.undoHandler()};
    asp::QueryHandler<AllTagsQuery>& allTagsHandler{
        queryComposer.allTagsHandler()};
    asp::QueryHandler<ReadTaskTreeQuery>& readTaskTreeHandler{
        queryComposer.readTaskTreeHandler()};
    asp::CommandHandler<RegisterSprintBulkCommand>& registerSprintsHandler{
        commandComposer.registerSprintBulkHandler()};
};

TEST_F(EditingTaskFixture, throws_when_no_task_with_given_uuid_exists)
{
    const auto tree = fixtures::givenSomeTaskTreeCreated(
        createTaskHandler, registerSprintsHandler);
    const TaskDTO editedTask{"123",
                             {"Tag1"},
                             "Some edited task name",
                             22,
                             {},
                             false,
                             dw::current_date_time_local(),
                             std::nullopt,
                             TaskTimeframeDTO{},
                             TaskTypeDTO::Regular};

    EXPECT_THROW(editTaskHandler.handle(EditTaskCommand{editedTask}),
                 HandlerException);
}

TEST_F(EditingTaskFixture,
       updates_timestamp_but_does_not_change_sprints_uuid_and_completion_status)
{
    const auto tree = fixtures::givenSomeTaskTreeCreated(
        createTaskHandler, registerSprintsHandler);
    const std::string uuid{"5"}; // from fixture
    const auto originalTask = tree.payload(uuid);
    const TaskTimeframeDTO changedFrame{current_date_time_local() - dw::Days{3},
                                        current_date_time_local() + Days{20},
                                        current_date_time_local() + Days{10},
                                        "recurrence string"};
    const TaskDTO editedTask{uuid,
                             {"ChangedTag"},
                             "Changed task name",
                             77,
                             {},
                             true,
                             current_date_time_local() - Days{100},
                             NoteDTO{"Changed note"},
                             changedFrame,
                             TaskTypeDTO::Project};
    const TaskDTO expected{uuid,
                           {"ChangedTag"},
                           "Changed task name",
                           77,
                           originalTask.value().get().sprints,
                           originalTask.value().get().finished,
                           current_date_time_local(),
                           NoteDTO{"Changed note"},
                           changedFrame,
                           TaskTypeDTO::Project};

    editTaskHandler.handle(EditTaskCommand{editedTask});

    EXPECT_EQ(expected,
              readTaskTreeHandler.handle(ReadTaskTreeQuery{}).payload(uuid));
}

TEST_F(EditingTaskFixture, handles_orphaned_and_new_tags)
{
    const auto tree = fixtures::givenSomeTaskTreeCreated(
        createTaskHandler, registerSprintsHandler);
    const std::string uuid{"1"};
    const TaskDTO editedTask{uuid,
                             {"ChangedTag"},
                             "Changed name",
                             77,
                             {},
                             true,
                             current_date_time_local() - Days{100},
                             std::nullopt,
                             TaskTimeframeDTO{},
                             TaskTypeDTO::Folder};

    editTaskHandler.handle(EditTaskCommand{editedTask});

    EXPECT_THAT(
        allTagsHandler.handle(api::AllTagsQuery{}),
        ::testing::UnorderedElementsAre("Tag1", "Tag2", "Tag3", "ChangedTag"));
}

TEST_F(EditingTaskFixture, undoing_task_edition)
{
    createTaskHandler.handle(CreateTaskCommand{"Name",
                                               {"Tag9"},
                                               7,
                                               TaskTypeDTO::Regular,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    const auto uuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const TaskDTO expected{uuid,
                           {"Tag9"},
                           "Name",
                           7,
                           {},
                           false,
                           current_date_time_local(),
                           std::nullopt,
                           TaskTimeframeDTO{current_date_time_local(),
                                            std::nullopt,
                                            std::nullopt,
                                            std::nullopt},
                           TaskTypeDTO::Regular};
    const TaskDTO editedTask{uuid,
                             {"Tag1"},
                             "Some edited task name",
                             22,
                             {},
                             false,
                             current_date_time_local(),
                             std::nullopt,
                             TaskTimeframeDTO{current_date_time_local(),
                                              std::nullopt,
                                              std::nullopt,
                                              std::nullopt},
                             TaskTypeDTO::Regular};
    editTaskHandler.handle(EditTaskCommand{editedTask});

    undoHandler.handle(UndoLastCommand{});

    EXPECT_EQ(expected, activeTasksHandler.handle(ActiveTasksQuery{}).front());
}

TEST_F(EditingTaskFixture, orphaned_tags_are_removed_after_edition)
{
    createTaskHandler.handle(CreateTaskCommand{"Name",
                                               {"Tag9"},
                                               7,
                                               TaskTypeDTO::Regular,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    const auto uuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const TaskDTO editedTask{uuid,
                             {"Tag1"},
                             "Some edited task name",
                             22,
                             {},
                             false,
                             current_date_time_local(),
                             std::nullopt,
                             TaskTimeframeDTO{current_date_time_local(),
                                              std::nullopt,
                                              std::nullopt,
                                              std::nullopt},
                             TaskTypeDTO::Regular};
    const std::vector<std::string> expected{"Tag1"};

    editTaskHandler.handle(EditTaskCommand{editedTask});

    const auto tags = allTagsHandler.handle(AllTagsQuery{});
    EXPECT_EQ(expected, tags);
}

TEST_F(EditingTaskFixture, orphaned_tags_are_recreated_after_undoing_edition)
{
    createTaskHandler.handle(CreateTaskCommand{"Name",
                                               {"Tag9"},
                                               7,
                                               TaskTypeDTO::Regular,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    const auto uuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const TaskDTO editedTask{uuid,
                             {"Tag1"},
                             "Some edited task name",
                             22,
                             {},
                             false,
                             current_date_time_local(),
                             std::nullopt,
                             TaskTimeframeDTO{current_date_time_local(),
                                              std::nullopt,
                                              std::nullopt,
                                              std::nullopt},
                             TaskTypeDTO::Regular};
    const std::vector<std::string> expected{"Tag9"};
    editTaskHandler.handle(EditTaskCommand{editedTask});

    undoHandler.handle(UndoLastCommand{});

    const auto tags = allTagsHandler.handle(AllTagsQuery{});
    EXPECT_EQ(expected, tags);
}
