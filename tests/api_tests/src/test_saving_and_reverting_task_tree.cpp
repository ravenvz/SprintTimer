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
#include "api/ObservableActionInvoker.h"
#include "api/TaskStorage.h"
#include "api/dtos/TaskTreeMapper.h"
#include "api/handlers/ActiveTasksHandler.h"
#include "api/handlers/AllTagsHandler.h"
#include "api/handlers/CreateTaskHandler.h"
#include "api_tests/QtStorageInitializer.h"
#include "api_tests/fixtures/TaskTreeFixture.h"
#include "api_tests/matchers/MatchesTaskIgnoringUuid.h"
#include "core/TaskTree.h"
#include "gtest/gtest.h"

using namespace sprint_timer;
using namespace sprint_timer::api;
using namespace sprint_timer;
using namespace sprint_timer::compose;
using namespace dw;
using namespace std::chrono_literals;

namespace sprint_timer {

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const TaskType& taskType)
{
    using enum TaskType;
    os << "TaskType::";
    switch (taskType) {
    case Regular:
        os << "Regular";
        break;
    case Project:
        os << "Project";
        break;
    case Folder:
        os << "Folder";
        break;
    }
    return os;
}

} // namespace sprint_timer

class ChangingTaskTreeFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    asp::CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    asp::CommandHandler<SaveTaskTreeCommand>& saveTaskTreeHandler{
        commandComposer.saveTaskTreeHandler()};
    asp::QueryHandler<ReadTaskTreeQuery>& readTaskTreeHandler{
        queryComposer.readTaskTreeHandler()};
    asp::CommandHandler<RegisterSprintBulkCommand>& registerSprintsHandler{
        commandComposer.registerSprintBulkHandler()};
    asp::CommandHandler<UndoLastCommand>& undoActionHandler{
        commandComposer.undoHandler()};
};

TEST_F(ChangingTaskTreeFixture, saving_tree_and_reverting)
{
    using namespace sprint_timer::api;
    const auto initialTree = fixtures::givenSomeTaskTreeCreated(
        createTaskHandler, registerSprintsHandler);
    // std::cout << initialTree.to_string() << std::endl;
    auto reverted_tree = readTaskTreeHandler.handle(ReadTaskTreeQuery{});
    std::cout << reverted_tree.to_string() << std::endl;
    auto mutatedTree = initialTree;
    auto uuid_projection = [](const auto& node) { return node.uuid; };
    mutatedTree.move_nodes(std::ranges::find(mutatedTree, "1", uuid_projection),
                           ds::SourcePosition{1},
                           ds::Count{1},
                           std::ranges::find(mutatedTree, "0", uuid_projection),
                           ds::DestinationPosition{0});
    // std::cout << mutatedTree.to_string() << std::endl;

    saveTaskTreeHandler.handle(SaveTaskTreeCommand{mutatedTree});
    EXPECT_EQ(mutatedTree, readTaskTreeHandler.handle(ReadTaskTreeQuery{}));

    // std::cout << initialTree.to_string() << std::endl;
    undoActionHandler.handle(UndoLastCommand{});
    // auto reverted_tree = readTaskTreeHandler.handle(ReadTaskTreeQuery{});
    // std::cout << reverted_tree.to_string() << std::endl;
    EXPECT_EQ(initialTree, readTaskTreeHandler.handle(ReadTaskTreeQuery{}));
}
