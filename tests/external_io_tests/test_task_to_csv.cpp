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
#include "gtest/gtest.h"
#include "core/TaskBuilder.h"
#include "external_io/TaskToCsvAlgorithm.h"

using namespace dw;
using namespace std::chrono_literals;
using namespace sprint_timer::entities;
using namespace sprint_timer;

class TaskToCsvFixture : public ::testing::Test {
public:
    Date someDate{Year{2020}, Month{6}, Day{19}};
    DateTime someTimeStamp{DateTime{someDate} + 2h};
    external_io::TaskToCsvAlgorithm taskSerializer;
};

TEST_F(TaskToCsvFixture, serializes_task)
{
    TaskBuilder builder;
    const auto task = builder.withUuid("123")
                          .withEstimatedCost(5)
                          .withActualCost(3)
                          .withName("Some task")
                          .withCompletionStatus(true)
                          .withExplicitTags({{Tag{"Tag 1"}, Tag{"Tag 2"}}})
                          .withLastModificationStamp(someTimeStamp + 2h + 33min)
                          .build();
    const std::string expected{
        "123;Some task;Tag 1,Tag 2;3;5;1;04:33 19.06.2020"};

    EXPECT_EQ(expected, taskSerializer.serialize(task));
}

TEST_F(TaskToCsvFixture, serializes_batch)
{
    TaskBuilder builder;
    std::vector<Task> tasks;
    tasks.push_back(builder.withUuid("123")
                        .withEstimatedCost(19)
                        .withActualCost(19)
                        .withName("First task")
                        .withCompletionStatus(true)
                        .withExplicitTags({Tag{"Tag 1"}})
                        .withLastModificationStamp(someTimeStamp + 8h + 12min)
                        .build());
    tasks.push_back(builder.withUuid("345")
                        .withEstimatedCost(7)
                        .withActualCost(12)
                        .withName("Second task")
                        .withCompletionStatus(false)
                        .withExplicitTags({})
                        .withLastModificationStamp(someTimeStamp + 12h + 58min)
                        .build());
    tasks.push_back(builder.withUuid("567")
                        .withEstimatedCost(20)
                        .withActualCost(9)
                        .withName("Third task")
                        .withCompletionStatus(true)
                        .withExplicitTags({Tag{"Tag 1"}, Tag{"Tag 2"}})
                        .withLastModificationStamp(someTimeStamp + 1h + 12min)
                        .build());
    const std::vector<std::string> expected{
        "123;First task;Tag 1;19;19;1;10:12 19.06.2020",
        "345;Second task;;12;7;0;14:58 19.06.2020",
        "567;Third task;Tag 1,Tag 2;9;20;1;03:12 19.06.2020"};

    EXPECT_EQ(expected, taskSerializer.serializeBatch(tasks));
}
