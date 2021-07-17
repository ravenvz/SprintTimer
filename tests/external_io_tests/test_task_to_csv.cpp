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
#include "external_io/TaskToCsvAlgorithm.h"
#include "gtest/gtest.h"

using namespace dw;
using namespace std::chrono_literals;
using namespace sprint_timer;
using sprint_timer::use_cases::TaskDTO;

class TaskToCsvFixture : public ::testing::Test {
public:
    Date someDate{Year{2020}, Month{6}, Day{19}};
    DateTime someTimeStamp{DateTime{someDate} + 2h};
    external_io::TaskToCsvAlgorithm taskSerializer;
};

TEST_F(TaskToCsvFixture, serializes_task)
{
    const TaskDTO task{"123",
                       {"Tag1", "Tag2"},
                       "Some task",
                       5,
                       3,
                       true,
                       someTimeStamp + 2h + 33min};
    const std::string expected{
        "123;Some task;Tag1,Tag2;3;5;1;04:33 19.06.2020"};

    EXPECT_EQ(expected, taskSerializer.serialize(task));
}

TEST_F(TaskToCsvFixture, serializes_batch)
{
    const std::vector<TaskDTO> tasks{
        TaskDTO{"123",
                {"Tag1"},
                "First task",
                19,
                19,
                true,
                someTimeStamp + 8h + 12min},
        TaskDTO{"345",
                std::vector<std::string>{},
                "Second task",
                7,
                12,
                false,
                someTimeStamp + 12h + 58min},
        TaskDTO{"567",
                {"Tag1", "Tag2"},
                "Third task",
                20,
                9,
                true,
                someTimeStamp + 1h + 12min},

    };
    const std::vector<std::string> expected{
        "123;First task;Tag1;19;19;1;10:12 19.06.2020",
        "345;Second task;;12;7;0;14:58 19.06.2020",
        "567;Third task;Tag1,Tag2;9;20;1;03:12 19.06.2020"};

    EXPECT_EQ(expected, taskSerializer.serializeBatch(tasks));
}
