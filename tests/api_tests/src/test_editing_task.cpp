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
                                        "Mon,Tue 2023-*-* 11:05:00"};
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

TEST_F(
    EditingTaskFixture,
    editing_due_date_updates_subtasks_due_dates_if_they_were_not_set_unequal_to_parents)
{
    using namespace std::chrono_literals;
    const auto tree = fixtures::givenTaskTreeWithDueDatesCreated(
        createTaskHandler, registerSprintsHandler);
    const std::string uuid{"1"};
    const DateTime referenceTime{DateTime{Date{Year{2023}, Month{8}, Day{3}}} +
                                 9h};
    const TaskTimeframeDTO updatedTimeFrame{dw::current_date_time_local(),
                                            referenceTime,
                                            std::nullopt,
                                            std::nullopt};
    const TaskDTO editedTask{uuid,
                             {},
                             "project1",
                             30,
                             {},
                             false,
                             current_date_time_local(),
                             std::nullopt,
                             updatedTimeFrame,
                             TaskTypeDTO::Project};
    auto extractTimeFrame = [](const auto& node) { return node.timeFrame; };
    Tree<std::string, TaskTimeframeDTO> expected;
    expected.addChild("0", TaskTimeframeDTO{}, std::nullopt);
    expected.addChild("1", updatedTimeFrame, "0");
    expected.addChild(
        "2",
        TaskTimeframeDTO{DateTime{Date{Year{2023}, Month{6}, Day{19}}} + 4h,
                         DateTime{Date{Year{2023}, Month{6}, Day{19}}} +
                             Days{10} + 4h,
                         std::nullopt,
                         std::nullopt},
        "1");
    expected.addChild("3", updatedTimeFrame, "1");
    expected.addChild("4", updatedTimeFrame, "3");
    expected.addChild("5", TaskTimeframeDTO{}, std::nullopt);

    editTaskHandler.handle(EditTaskCommand{editedTask});

    const auto editedTree = readTaskTreeHandler.handle(ReadTaskTreeQuery{})
                                .mapped(extractTimeFrame);
    EXPECT_EQ(expected, editedTree);
}

TEST_F(EditingTaskFixture, undoing_task_edition_restores_subtask_due_dates)
{
    using namespace std::chrono_literals;
    const auto tree = fixtures::givenTaskTreeWithDueDatesCreated(
        createTaskHandler, registerSprintsHandler);
    const std::string uuid{"1"};
    const DateTime referenceTime{DateTime{Date{Year{2023}, Month{8}, Day{3}}} +
                                 9h};
    const TaskTimeframeDTO updatedTimeFrame{dw::current_date_time_local(),
                                            referenceTime,
                                            std::nullopt,
                                            std::nullopt};
    const TaskDTO editedTask{uuid,
                             {"SomeNewTag"},
                             "project1",
                             30,
                             {},
                             false,
                             current_date_time_local(),
                             std::nullopt,
                             updatedTimeFrame,
                             TaskTypeDTO::Project};
    auto extractTimeFrame = [](const auto& node) { return node.timeFrame; };
    Tree<std::string, TaskTimeframeDTO> expected;
    expected.addChild("0", TaskTimeframeDTO{}, std::nullopt);
    expected.addChild("1", updatedTimeFrame, "0");
    expected.addChild(
        "2",
        TaskTimeframeDTO{DateTime{Date{Year{2023}, Month{6}, Day{19}}} + 4h,
                         DateTime{Date{Year{2023}, Month{6}, Day{19}}} +
                             Days{10} + 4h,
                         std::nullopt,
                         std::nullopt},
        "1");
    expected.addChild("3", updatedTimeFrame, "1");
    expected.addChild("4", updatedTimeFrame, "3");
    expected.addChild("5", TaskTimeframeDTO{}, std::nullopt);
    editTaskHandler.handle(EditTaskCommand{editedTask});

    undoHandler.handle(UndoLastCommand{});

    const auto actual = readTaskTreeHandler.handle(ReadTaskTreeQuery{});
    EXPECT_EQ(tree, actual);
    EXPECT_TRUE(allTagsHandler.handle(api::AllTagsQuery{}).empty());
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
