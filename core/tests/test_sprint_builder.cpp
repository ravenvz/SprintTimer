/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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

#include "core/entities/Task.h"
#include "core/SprintBuilder.h"
#include <TestHarness.h>

TEST_GROUP(TestSprintBuilder) {

};

TEST(TestSprintBuilder, test_throws_when_not_associated_with_task) {
    SprintBuilder builder;
    CHECK_THROWS(SprintBuilderError, builder.build());
}

TEST(TestSprintBuilder, test_builds_sprint_from_task)
{
    SprintBuilder builder;
    const Task task{"Some task",
                    4,
                    0,
                    "1234",
                    {Tag{"Tag1"}, Tag{"Tag2"}},
                    false,
                    DateTime::fromYMD(2016, 11, 26)};

    auto sprint = builder.forTask(task).build();

    CHECK_EQUAL(task.uuid(), sprint.taskUuid());
    CHECK_EQUAL(task.name(), sprint.name());
    CHECK(task.tags() == sprint.tags());
    CHECK(!sprint.uuid().empty());
}

TEST(TestSprintBuilder, test_sprint_builder)
{
    SprintBuilder builder;
    std::list<Tag> expectedTags{Tag{"Tag1"}, Tag{"Tag2"}};

    auto sprint = builder
        .withName("Petty sprint")
        .withTag("Tag1")
        .withTag("Tag2")
        .withTaskUuid("1234")
        .build();

    CHECK_EQUAL("Petty sprint", sprint.name());
    CHECK_EQUAL("1234", sprint.taskUuid());
    CHECK(!sprint.uuid().empty())
    CHECK(expectedTags == sprint.tags());
}

TEST(TestSprintBuilder, test_explicitly_overwrites_tags)
{
    SprintBuilder builder;
    auto expectedTags = std::list<Tag>{Tag{"NewTag1"}, Tag{"NewTag2"}};
    builder.withTag("Tag1").withTaskUuid("1234");

    builder.withExplicitTags({Tag{"NewTag1"}, Tag{"NewTag2"}});
    auto sprint = builder.build();

    CHECK(expectedTags == sprint.tags());
}


TEST(TestSprintBuilder, test_generates_new_uuid_when_serial_constructing)
{
    SprintBuilder builder;
    std::string associatedTaskUuid{"1234"};
    builder.withTaskUuid(associatedTaskUuid);

    auto sprint1 = builder.withUuid("1234").build();
    auto sprint2 = builder.build();
    auto sprint3 = builder.build();

    CHECK(!sprint1.uuid().empty());
    CHECK(sprint1.uuid() != sprint2.uuid());
    CHECK(sprint2.uuid() != sprint3.uuid());
}
