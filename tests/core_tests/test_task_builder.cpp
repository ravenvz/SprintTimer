/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

// TODO remove when Gtest drops std::tr1
// Workaround for C++17 as std::tr1 no longer available and Gtest uses it
#define GTEST_LANG_CXX11 1


#include "core/TaskBuilder.h"
#include "gtest/gtest.h"

using dw::DateTime;
using namespace sprint_timer;
using namespace sprint_timer::entities;

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

    EXPECT_EQ("Test task", task.name());
    EXPECT_EQ(4, task.estimatedCost());
    EXPECT_EQ(2, task.actualCost());
    EXPECT_EQ("1234", task.uuid());
    EXPECT_TRUE(task.isCompleted());
    EXPECT_TRUE(expectedTags == task.tags());
    EXPECT_TRUE(expectedTimeStamp == task.lastModified());
}

TEST(TestTaskBuilder, test_explicitly_overwrites_tags)
{
    TaskBuilder builder;
    builder.withUuid("1234").withTag("Tag1").withTag(Tag{"Tag2"});
    std::list<Tag> expectedTags{Tag{"NewTag1"}, Tag{"NewTag2"}};

    builder.withExplicitTags(expectedTags);
    auto task = builder.build();

    EXPECT_TRUE(expectedTags == task.tags());
}

TEST(TestTaskBuilder, test_generates_new_uuid_when_serial_constructing)
{
    TaskBuilder builder;

    auto task1 = builder.build();
    auto task2 = builder.build();
    auto task3 = builder.build();

    EXPECT_FALSE(task1.uuid().empty());
    EXPECT_TRUE(task1.uuid() != task2.uuid());
    EXPECT_TRUE(task2.uuid() != task3.uuid());
}

TEST(TestTaskBuilder, test_timestamp_modification_bugfix_verification)
{
    TaskBuilder builder;
    DateTime dt = DateTime::fromYMD(2013, 10, 2);

    auto task1 = builder.withLastModificationStamp(dt).build();

    EXPECT_TRUE(dt == task1.lastModified());
}
