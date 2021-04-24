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
#include "core/ObservableActionInvoker.h"
#include "core/TaskStorage.h"
#include "core/use_cases/TaskDTO.h"
#include "core/use_cases/create_task/CreateTaskHandler.h"
#include "core/use_cases/request_tags/AllTagsHandler.h"
#include "core/use_cases/request_tasks/ActiveTasksHandler.h"
#include "use_cases_tests/QtStorageInitializer.h"
#include "use_cases_tests/matchers/MatchesTaskIgnoringUuid.h"
#include "gmock/gmock.h"

using namespace sprint_timer;
using namespace sprint_timer::use_cases;
using namespace sprint_timer::entities;
using namespace sprint_timer::compose;
using namespace dw;

using ::testing::Truly;

class CreatingTasksFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
    QueryHandler<AllTagsQuery>& allTagsHandler{queryComposer.allTagsHandler()};
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
                     0,
                     false,
                     current_date_time_local()};

    createTaskHandler.handle(CreateTaskCommand{name, tags, estimatedCost});

    const auto activeTasks = activeTasksHandler.handle(ActiveTasksQuery{});

    EXPECT_EQ(1, activeTasks.size());
    EXPECT_THAT(activeTasks.front(),
                Truly(matchers::MatchesTaskIgnoringUuid{expected}));
    EXPECT_THAT(allTagsHandler.handle(AllTagsQuery{}),
                ::testing::ElementsAre("Tag1", "Tag2"));
}

