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
#include "gmock/gmock.h"

#include "use_cases_tests/QtStorageInitializer.h"
#include "use_cases_tests/matchers/MatchesTaskIgnoringUuid.h"
#include "gmock/gmock.h"

using namespace sprint_timer;
using namespace sprint_timer::use_cases;
using namespace sprint_timer::entities;
using namespace sprint_timer::compose;
using namespace dw;

class RenamingTagFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    QueryHandler<AllTagsQuery>& allTagsHandler{queryComposer.allTagsHandler()};
    CommandHandler<RenameTagCommand>& renameTagHandler{
        commandComposer.renameTagHandler()};
};

TEST_F(RenamingTagFixture, renaming_tag)
{
    using ::testing::UnorderedElementsAre;
    createTaskHandler.handle(
        CreateTaskCommand{"Some task", {"Tag1", "Tag2", "Tag3"}, 12});
    createTaskHandler.handle(
        CreateTaskCommand{"Other task", {"Tag2", "Tag3", "Tag4"}, 21});

    renameTagHandler.handle(RenameTagCommand{"Tag3", "RenamedTag"});

    EXPECT_THAT(allTagsHandler.handle(AllTagsQuery{}),
                UnorderedElementsAre("Tag1", "Tag2", "RenamedTag", "Tag4"));
}

TEST_F(RenamingTagFixture, renaming_that_does_not_exist_does_nothing)
{
    using ::testing::UnorderedElementsAre;
    createTaskHandler.handle(
        CreateTaskCommand{"Some task", {"Tag1", "Tag2", "Tag3"}, 12});
    createTaskHandler.handle(
        CreateTaskCommand{"Other task", {"Tag2", "Tag3", "Tag4"}, 21});

    renameTagHandler.handle(RenameTagCommand{"NonExistingTag", "RenamedTag"});

    EXPECT_THAT(allTagsHandler.handle(AllTagsQuery{}),
                UnorderedElementsAre("Tag1", "Tag2", "Tag3", "Tag4"));
}
