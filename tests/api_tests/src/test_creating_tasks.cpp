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
#include "api/ActionInvoker.h"
#include "api/ObservableActionInvoker.h"
#include "api/TaskStorage.h"
#include "api/dtos/TaskDTO.h"
#include "api/handlers/ActiveTasksHandler.h"
#include "api/handlers/AllTagsHandler.h"
#include "api/handlers/CreateTaskHandler.h"
#include "api_tests/QtStorageInitializer.h"
#include "api_tests/fixtures/TaskTreeFixture.h"
#include "api_tests/matchers/MatchesTaskIgnoringUuid.h"
#include "api_tests/utils/utils.h"
#include "gmock/gmock.h"
#include <fstream>

using namespace sprint_timer;
using namespace sprint_timer::api;
using namespace sprint_timer;
using namespace sprint_timer::compose;
using namespace dw;

using ::testing::Truly;

class CreatingTasksFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    asp::CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    asp::QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
    asp::QueryHandler<AllTagsQuery>& allTagsHandler{
        queryComposer.allTagsHandler()};
    asp::CommandHandler<UndoLastCommand>& undoHandler{
        commandComposer.undoHandler()};
    asp::QueryHandler<ReadTaskTreeQuery>& readTaskTreeHandler{
        queryComposer.readTaskTreeHandler()};
    asp::CommandHandler<RegisterSprintBulkCommand>& registerSprintsHandler{
        commandComposer.registerSprintBulkHandler()};
};

TEST_F(CreatingTasksFixture, creating_tasks)
{
    const auto tree = fixtures::givenSomeTaskTreeCreated(
        createTaskHandler, registerSprintsHandler, initializer);

    const auto actual = readTaskTreeHandler.handle(ReadTaskTreeQuery{});

    EXPECT_EQ(tree, actual);
}

TEST_F(CreatingTasksFixture, undoing_task_creation_cleans_up_associated_tags)
{
    const auto tree = fixtures::givenSomeTaskTreeCreated(
        createTaskHandler, registerSprintsHandler, initializer);
    createTaskHandler.handle(CreateTaskCommand{"Some name",
                                               {"ExoticTag", "Tag1"},
                                               4,
                                               TaskTypeDTO::Regular,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});

    undoHandler.handle(UndoLastCommand{});

    EXPECT_EQ(tree, readTaskTreeHandler.handle(ReadTaskTreeQuery{}));
    EXPECT_THAT(
        allTagsHandler.handle(AllTagsQuery{}),
        ::testing::UnorderedElementsAre("Tag1", "Tag2", "Tag3", "ProjectTag1"));
}

TEST_F(CreatingTasksFixture,
       creating_subtask_inherits_parents_start_date_and_due_date)
{
    using namespace std::chrono_literals;
    using namespace dw;
    const auto tree = fixtures::givenTaskTreeWithDueDatesCreated(
        createTaskHandler, registerSprintsHandler, initializer);
    const auto startTime = DateTime{Date{Year{2023}, Month{6}, Day{19}}} + 4h;
    const auto dueDateTime = startTime + Days{10};
    const TaskDTO expected{
        "6", // Generated uuid after fixture tree has been created
        {},
        "Subtask that inherits due date",
        5,
        {},
        false,
        dw::current_date_time_local(),
        std::nullopt,
        TaskTimeframeDTO{startTime, dueDateTime, std::nullopt, std::nullopt},
        TaskTypeDTO::Regular};
    const auto treeBefore = readTaskTreeHandler.handle(ReadTaskTreeQuery{});

    createTaskHandler.handle(CreateTaskCommand{"Subtask that inherits due date",
                                               {},
                                               5,
                                               TaskTypeDTO::Regular,
                                               "2",
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});

    const auto updatedTree = readTaskTreeHandler.handle(ReadTaskTreeQuery{});
    const auto actual_it = std::ranges::find(
        updatedTree, "6", [](const auto& node) { return node.uuid; });

    EXPECT_FALSE(actual_it == updatedTree.cend());
    EXPECT_EQ(expected, *actual_it);
}
