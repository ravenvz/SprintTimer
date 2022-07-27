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
#include "api/dtos/TaskDTO.h"
#include "api/handlers/ActiveTasksHandler.h"
#include "api/handlers/AllTagsHandler.h"
#include "api/handlers/CreateTaskHandler.h"
#include "api_tests/QtStorageInitializer.h"
#include "api_tests/matchers/MatchesTaskIgnoringUuid.h"
#include "gmock/gmock.h"

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
    const sprint_timer::api::DateTimeProvider& dt{
        initializer.dateTimeProvider()};
};

TEST_F(CreatingTasksFixture, creates_task)
{
    const std::string name{"Task name"};
    const std::vector<std::string> tags{"Tag1", "Tag2"};
    const int32_t estimatedCost{4};
    TaskDTO expected{"any_uuid",
                     tags,
                     name,
                     estimatedCost,
                     {},
                     false,
                     dt.dateTimeLocalNow()};

    createTaskHandler.handle(CreateTaskCommand{name, tags, estimatedCost});
    const auto activeTasks = activeTasksHandler.handle(ActiveTasksQuery{});

    EXPECT_EQ(1, activeTasks.size());
    EXPECT_THAT(activeTasks.front(),
                Truly(matchers::MatchesTaskIgnoringUuid{expected}));
    EXPECT_THAT(allTagsHandler.handle(AllTagsQuery{}),
                ::testing::ElementsAre("Tag1", "Tag2"));
}

TEST_F(CreatingTasksFixture, undoing_task_creation_cleans_up_associated_tags)
{
    const std::string name{"Task name"};
    const std::vector<std::string> tags{"Tag1", "Tag2"};
    const int32_t estimatedCost{4};
    TaskDTO expected{"any_uuid",
                     tags,
                     name,
                     estimatedCost,
                     {},
                     false,
                     dt.dateTimeLocalNow()};
    createTaskHandler.handle(CreateTaskCommand{name, tags, estimatedCost});

    undoHandler.handle(UndoLastCommand{});

    const auto activeTasks = activeTasksHandler.handle(ActiveTasksQuery{});
    EXPECT_TRUE(activeTasks.empty());
    EXPECT_TRUE(allTagsHandler.handle(AllTagsQuery{}).empty());
}
