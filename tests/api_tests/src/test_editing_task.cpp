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
using namespace std::chrono_literals;

namespace {

auto uuid_projection = [](const auto& node) { return node.uuid; };

auto name_projection = [](const auto& tree_node) { return tree_node.name(); };

} // namespace

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
    dw::DateTime someIrrelevantTimeStamp{
        DateTime{Date{Year{2025}, Month{1}, Day{16}}} + 15h + 43min};
};

TEST_F(EditingTaskFixture, throws_when_no_task_with_given_uuid_exists)
{
    const auto tree = fixtures::givenSomeTaskTreeCreated(
        createTaskHandler, registerSprintsHandler, initializer);
    const TaskDTO editedTask{"123",
                             {"Tag1"},
                             "Some edited task name",
                             22,
                             {},
                             false,
                             someIrrelevantTimeStamp,
                             std::nullopt,
                             TaskTimeframeDTO{someIrrelevantTimeStamp},
                             TaskTypeDTO::Regular};

    EXPECT_THROW(editTaskHandler.handle(EditTaskCommand{editedTask}),
                 HandlerException);
}

TEST_F(EditingTaskFixture,
       updates_timestamp_but_does_not_change_sprints_uuid_and_completion_status)
{
    const dw::DateTime editingTime{
        DateTime{Date{Year{2025}, Month{8}, Day{5}}} + 5h};
    const auto tree = fixtures::givenSomeTaskTreeCreated(
        createTaskHandler, registerSprintsHandler, initializer);
    const std::string uuid{"5"}; // from fixture
    const auto originalTask = *std::ranges::find(tree, uuid, uuid_projection);
    const TaskTimeframeDTO changedFrame{someIrrelevantTimeStamp - dw::Days{3},
                                        someIrrelevantTimeStamp + Days{20},
                                        someIrrelevantTimeStamp + Days{10},
                                        "Mon,Tue 2023-*-* 11:05:00"};
    const TaskDTO expected{uuid,
                           {"ChangedTag"},
                           "Changed task name",
                           77,
                           originalTask.sprints,
                           originalTask.finished,
                           editingTime,
                           NoteDTO{"Changed note"},
                           changedFrame,
                           TaskTypeDTO::Project};

    {
        TimePortalGuard timePortal{initializer.getDateTimeProvider(),
                                   editingTime};
        editTaskHandler.handle(
            EditTaskCommand{TaskDTO{uuid,
                                    {"ChangedTag"},
                                    "Changed task name",
                                    77,
                                    {},
                                    true,
                                    someIrrelevantTimeStamp - Days{100},
                                    NoteDTO{"Changed note"},
                                    changedFrame,
                                    TaskTypeDTO::Project}});
    }
    const auto updatedTree = readTaskTreeHandler.handle(ReadTaskTreeQuery{});
    const auto actual_it =
        std::ranges::find(updatedTree, uuid, uuid_projection);

    EXPECT_FALSE(actual_it == updatedTree.cend());
    EXPECT_EQ(expected, *actual_it);
}

TEST_F(
    EditingTaskFixture,
    editing_due_date_updates_subtasks_due_dates_unless_they_are_not_equal_to_parents)
{
    /* folder1 -- no changes
     *    project1 <-- editing task to change due date
     *       task1    Due date is set, should not change
     *       task2    Due date should be updated
     *          task3    Due date sould be updated
     * task4 -- no changes
     */
    const auto tree = fixtures::givenTaskTreeWithDueDatesCreated(
        createTaskHandler, registerSprintsHandler, initializer);
    const std::string uuid{"1"};
    const DateTime changedDueDate{DateTime{Date{Year{2023}, Month{8}, Day{3}}} +
                                  9h};
    const TaskTimeframeDTO updatedTimeFrame{
        changedDueDate - Days{2}, changedDueDate, std::nullopt, std::nullopt};
    auto extractTimeFrame = [](const auto& node) { return node.timeFrame; };
    // We're only interested in timeframes for this test.
    sprint_timer::TreeType<TaskTimeframeDTO> expected;
    auto node_1 = expected.insert(
        expected.end(),
        std::ranges::find(tree, "folder1", &TaskDTO::name)->timeFrame);
    auto node_2 = expected.insert(node_1, updatedTimeFrame);
    expected.insert(
        node_2, std::ranges::find(tree, "task1", &TaskDTO::name)->timeFrame);
    auto node_3 = expected.insert(node_2, updatedTimeFrame);
    expected.insert(node_3, updatedTimeFrame);
    expected.insert(
        expected.end(),
        std::ranges::find(tree, "task4", &TaskDTO::name)->timeFrame);

    editTaskHandler.handle(EditTaskCommand{TaskDTO{uuid,
                                                   {},
                                                   "project1",
                                                   30,
                                                   {},
                                                   false,
                                                   current_date_time_local(),
                                                   std::nullopt,
                                                   updatedTimeFrame,
                                                   TaskTypeDTO::Project}});

    const auto editedTree = readTaskTreeHandler.handle(ReadTaskTreeQuery{})
                                .transform(extractTimeFrame);
    EXPECT_EQ(expected, editedTree);
}

TEST_F(EditingTaskFixture, undoing_task_edition_restores_subtask_due_dates)
{
    using namespace std::chrono_literals;
    const auto tree = fixtures::givenTaskTreeWithDueDatesCreated(
        createTaskHandler, registerSprintsHandler, initializer);
    const std::string uuid{"1"};
    const DateTime changedDueDate{DateTime{Date{Year{2023}, Month{8}, Day{3}}} +
                                  9h};
    const TaskTimeframeDTO updatedTimeFrame{
        someIrrelevantTimeStamp, changedDueDate, std::nullopt, std::nullopt};
    const TaskDTO editedTask{uuid,
                             {"SomeNewTag"},
                             "renamed_project1",
                             30,
                             {},
                             false,
                             someIrrelevantTimeStamp,
                             std::nullopt,
                             updatedTimeFrame,
                             TaskTypeDTO::Project};
    auto extractTimeFrame = [](const auto& node) { return node.timeFrame; };

    editTaskHandler.handle(EditTaskCommand{editedTask});
    undoHandler.handle(UndoLastCommand{});

    const auto actual = readTaskTreeHandler.handle(ReadTaskTreeQuery{});
    EXPECT_EQ(tree, actual);
    EXPECT_TRUE(allTagsHandler.handle(api::AllTagsQuery{}).empty());
}

TEST_F(EditingTaskFixture, handles_orphaned_and_new_tags)
{
    const auto tree = fixtures::givenSomeTaskTreeCreated(
        createTaskHandler, registerSprintsHandler, initializer);
    const std::string uuid{"1"};
    const TaskDTO editedTask{
        uuid,
        {"ChangedTag"},
        "Changed name",
        77,
        {},
        true,
        someIrrelevantTimeStamp - Days{100},
        std::nullopt,
        TaskTimeframeDTO{someIrrelevantTimeStamp - Days{100}},
        TaskTypeDTO::Folder};

    editTaskHandler.handle(EditTaskCommand{editedTask});

    EXPECT_THAT(
        allTagsHandler.handle(api::AllTagsQuery{}),
        ::testing::UnorderedElementsAre("Tag1", "Tag2", "Tag3", "ChangedTag"));
}

TEST_F(EditingTaskFixture, orphaned_tags_are_removed_after_edition)
{
    createTaskHandler.handle(
        CreateTaskCommand{"Name",
                          {"Tag9"},
                          7,
                          TaskTypeDTO::Regular,
                          std::nullopt,
                          std::nullopt,
                          std::nullopt,
                          TaskTimeframeDTO{someIrrelevantTimeStamp}});
    const auto uuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const TaskDTO editedTask{
        uuid,
        {"Tag1"},
        "Some edited task name",
        22,
        {},
        false,
        someIrrelevantTimeStamp,
        std::nullopt,
        TaskTimeframeDTO{
            someIrrelevantTimeStamp, std::nullopt, std::nullopt, std::nullopt},
        TaskTypeDTO::Regular};
    const std::vector<std::string> expected{"Tag1"};

    editTaskHandler.handle(EditTaskCommand{editedTask});

    const auto tags = allTagsHandler.handle(AllTagsQuery{});
    EXPECT_EQ(expected, tags);
}

TEST_F(EditingTaskFixture, orphaned_tags_are_recreated_after_undoing_edition)
{
    createTaskHandler.handle(
        CreateTaskCommand{"Name",
                          {"Tag9"},
                          7,
                          TaskTypeDTO::Regular,
                          std::nullopt,
                          std::nullopt,
                          std::nullopt,
                          TaskTimeframeDTO{someIrrelevantTimeStamp}});
    const auto uuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const TaskDTO editedTask{
        uuid,
        {"Tag1"},
        "Some edited task name",
        22,
        {},
        false,
        someIrrelevantTimeStamp,
        std::nullopt,
        TaskTimeframeDTO{
            someIrrelevantTimeStamp, std::nullopt, std::nullopt, std::nullopt},
        TaskTypeDTO::Regular};
    const std::vector<std::string> expected{"Tag9"};
    editTaskHandler.handle(EditTaskCommand{editedTask});

    undoHandler.handle(UndoLastCommand{});

    const auto tags = allTagsHandler.handle(AllTagsQuery{});
    EXPECT_EQ(expected, tags);
}
