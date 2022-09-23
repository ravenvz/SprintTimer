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
using sprint_timer::api::TaskDTO;

class TaskToCsvFixture : public ::testing::Test {
public:
    Date someDate{Year{2020}, Month{6}, Day{19}};
    DateTime someTimeStamp{DateTime{someDate} + 2h};
    DateTimeRange someSprint{DateTime{someDate}, DateTime{someDate} + 25min};
    external_io::TaskToCsvAlgorithm taskSerializer;
};

TEST_F(TaskToCsvFixture, serializes_task)
{
    const TaskDTO task{
        "123",
        {"Tag1", "Tag2"},
        "Some task",
        5,
        {someSprint, add_offset(someSprint, 50min), add_offset(someSprint, 2h)},
        true,
        someTimeStamp + 2h + 33min,
        api::NoteDTO{"Some task note"},
        api::TaskTimeframeDTO{someTimeStamp + 1h,
                              someTimeStamp + Days{20},
                              someTimeStamp + Days{20} - 30min,
                              "3,2,2"},
        api::TaskTypeDTO::Regular};
    const std::string expected{
        "123;Some task;Tag1,Tag2;3;5;1;04:33 19.06.2020;Some task note;03:00 "
        "19.06.2020;02:00 09.07.2020;01:30 09.07.2020;3,2,2;2"};

    EXPECT_EQ(expected, taskSerializer.serialize(task));
}

TEST_F(TaskToCsvFixture, serializes_batch)
{
    const std::vector<TaskDTO> tasks{
        TaskDTO{"123",
                {"Tag1"},
                "First task",
                19,
                {someSprint, add_offset(someSprint, 50min)},
                true,
                someTimeStamp + 8h + 12min,
                api::NoteDTO{"Some task note"},
                api::TaskTimeframeDTO{someTimeStamp + 1h,
                                      someTimeStamp + Days{20},
                                      someTimeStamp + Days{20} - 30min,
                                      std::nullopt},
                api::TaskTypeDTO::Folder},
        TaskDTO{"345",
                std::vector<std::string>{},
                "Second task",
                7,
                {someSprint,
                 add_offset(someSprint, 50min),
                 add_offset(someSprint, 2h)},
                false,
                someTimeStamp + 12h + 58min,
                std::nullopt,
                api::TaskTimeframeDTO{someTimeStamp + 5h + 10min,
                                      someTimeStamp + Days{22},
                                      std::nullopt,
                                      std::nullopt},
                api::TaskTypeDTO::Project},
        TaskDTO{"567",
                {"Tag1", "Tag2"},
                "Third task",
                20,
                {someSprint,
                 add_offset(someSprint, 50min),
                 add_offset(someSprint, 2h),
                 add_offset(someSprint, 3h)},
                true,
                someTimeStamp + 1h + 12min,
                std::nullopt,
                api::TaskTimeframeDTO{
                    someTimeStamp, std::nullopt, std::nullopt, std::nullopt},
                api::TaskTypeDTO::Regular},

    };
    const std::vector<std::string> expected{
        "123;First task;Tag1;2;19;1;10:12 19.06.2020;Some task note;03:00 "
        "19.06.2020;02:00 09.07.2020;01:30 09.07.2020;;1",
        "345;Second task;;3;7;0;14:58 19.06.2020;;07:10 19.06.2020;02:00 "
        "11.07.2020;;;0",
        "567;Third task;Tag1,Tag2;4;20;1;03:12 19.06.2020;;02:00 "
        "19.06.2020;;;;2"};

    EXPECT_EQ(expected, taskSerializer.serializeBatch(tasks));
}
