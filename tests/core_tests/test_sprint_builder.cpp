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

#include "core/entities/Task.h"
#include "gtest/gtest.h"
#include <core/SprintBuilder.h>

using namespace std::chrono_literals;
using namespace sprint_timer;
using namespace sprint_timer::entities;
using namespace dw;

namespace {

const DateTimeRange defaultTimespan{current_date_time(),
                                    current_date_time() + 25min};

} // namespace

TEST(TestSprintBuilder, test_throws_when_not_associated_with_task)
{
    SprintBuilder builder;

    ASSERT_THROW(builder.build(), SprintBuilderError);
}

TEST(TestSprintBuilder, test_throws_when_no_timespan_provided)
{
    SprintBuilder builder;
    builder.withTaskUuid("123");

    ASSERT_THROW(builder.build(), SprintBuilderError);
}

TEST(TestSprintBuilder, test_builds_sprint_for_task)
{
    SprintBuilder builder;
    const Task task{"Some task",
                    4,
                    0,
                    "1234",
                    {Tag{"Tag1"}, Tag{"Tag2"}},
                    false,
                    DateTime{Date{Year{2016}, Month{11}, Day{26}}}};

    auto sprint = builder.forTask(task).withTimeSpan(defaultTimespan).build();

    EXPECT_EQ(task.uuid(), sprint.taskUuid());
    EXPECT_EQ(task.name(), sprint.name());
    EXPECT_TRUE(task.tags() == sprint.tags());
    EXPECT_TRUE(!sprint.uuid().empty());
}

TEST(TestSprintBuilder, test_sprint_builder)
{
    SprintBuilder builder;
    std::list<Tag> expectedTags{Tag{"Tag1"}, Tag{"Tag2"}};

    auto sprint = builder.withName("Petty sprint")
                      .withTag("Tag1")
                      .withTag("Tag2")
                      .withTaskUuid("1234")
                      .withTimeSpan(defaultTimespan)
                      .build();

    EXPECT_EQ("Petty sprint", sprint.name());
    EXPECT_EQ("1234", sprint.taskUuid());
    EXPECT_FALSE(sprint.uuid().empty());
    EXPECT_TRUE(expectedTags == sprint.tags());
}

TEST(TestSprintBuilder, test_explicitly_overwrites_tags)
{
    SprintBuilder builder;
    auto expectedTags = std::list<Tag>{Tag{"NewTag1"}, Tag{"NewTag2"}};
    builder.withTag("Tag1").withTaskUuid("1234").withTimeSpan(defaultTimespan);

    builder.withExplicitTags({Tag{"NewTag1"}, Tag{"NewTag2"}});
    auto sprint = builder.build();

    EXPECT_TRUE(expectedTags == sprint.tags());
}


TEST(TestSprintBuilder,
     test_generates_new_uuid_when_constructing_multiple_instances)
{
    using namespace std::chrono_literals;
    SprintBuilder builder;
    std::string associatedTaskUuid{"1234"};
    builder.withTaskUuid(associatedTaskUuid);

    auto sprint1
        = builder.withUuid("1234").withTimeSpan(defaultTimespan).build();
    auto sprint2 = builder.build();
    auto sprint3 = builder.build();

    EXPECT_FALSE(sprint1.uuid().empty());
    EXPECT_TRUE(sprint1.uuid() != sprint2.uuid());
    EXPECT_TRUE(sprint2.uuid() != sprint3.uuid());
}
