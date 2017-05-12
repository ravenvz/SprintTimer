/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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

#include "core/TaskBuilder.h"
#include <TestHarness.h>

TEST_GROUP(TestTaskBuilder) {

};

TEST(TestTaskBuilder, test_name_) {
    TaskBuilder builder;
    DateTime expectedTimeStamp = DateTime::fromYMD(2015, 11, 28);
    std::list<Tag> expectedTags{Tag{"Tag1"}, Tag{"Tag2"}};

    auto task = builder
            .withName("Test task")
            .withEstimatedCost(4)
            .withActualCost(2)
            .withUuid("1234")
            .withTag("Tag1")
            .withTag("Tag2")
            .withCompletionStatus(true)
            .withLastModificationStamp(expectedTimeStamp)
            .build();

    CHECK_EQUAL("Test task", task.name());
    CHECK_EQUAL(4, task.estimatedCost());
    CHECK_EQUAL(2, task.actualCost());
    CHECK_EQUAL("1234", task.uuid());
    CHECK(task.isCompleted());
    CHECK(expectedTags == task.tags());
    CHECK(expectedTimeStamp == task.lastModified());
}

TEST(TestTaskBuilder, test_explicitly_overwrites_tags)
{
    TaskBuilder builder;
    builder.withUuid("1234").withTag("Tag1").withTag(Tag{"Tag2"});
    std::list<Tag> expectedTags{Tag{"NewTag1"}, Tag{"NewTag2"}};

    builder.withExplicitTags(expectedTags);
    auto task = builder.build();

    CHECK(expectedTags == task.tags());
}

TEST(TestTaskBuilder, test_generates_new_uuid_when_serial_constructing)
{
    TaskBuilder builder;

    auto task1 = builder.build();
    auto task2 = builder.build();
    auto task3 = builder.build();

    CHECK(!task1.uuid().empty());
    CHECK(task1.uuid() != task2.uuid());
    CHECK(task2.uuid() != task3.uuid());
}

TEST(TestTaskBuilder, test_timestamp_modification_bugfix_verification)
{
    TaskBuilder builder;
    DateTime dt = DateTime::fromYMD(2013, 10, 2);

    auto task1 = builder.withLastModificationStamp(dt).build();

    CHECK(dt == task1.lastModified());
}
