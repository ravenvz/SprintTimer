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
#include "core/SprintTimerException.h"
#include "core/Task.h"
#include "gtest/gtest.h"

using namespace sprint_timer;
using namespace dw;
using namespace std::chrono_literals;

TEST(TestTask, adding_sprint)
{
    const std::string taskName{"Some name"};
    const std::string taskUuid{"123"};
    constexpr auto modificationStamp =
        DateTime{Date{Year{2016}, Month{9}, Day{21}}} + 12h + 59min + 19s;
    Task someTask{taskName,
                  4,
                  {},
                  taskUuid,
                  {},
                  false,
                  modificationStamp,
                  TaskType::Regular,
                  std::nullopt,
                  TaskTimeframe{}};

    someTask.addSprint(Sprint{current_date_time_local(), 25min});
    someTask.addSprint(Sprint{current_date_time_local() + 25min, 25min});

    EXPECT_EQ(2, someTask.actualCost());
    EXPECT_EQ(current_date_time_local(), someTask.lastModified());
}

TEST(TestTask, rejects_sprint_if_it_intersects_with_others)
{
    constexpr auto modificationStamp =
        DateTime{Date{Year{2016}, Month{9}, Day{21}}} + 12h + 59min + 19s;
    Task someTask{"Some name",
                  4,
                  {},
                  "123",
                  {},
                  false,
                  modificationStamp,
                  TaskType::Regular,
                  std::nullopt,
                  TaskTimeframe{}};
    const DateTimeRange timeRange{dw::current_date_time(),
                                  dw::current_date_time() + 25min};
    const DateTimeRange conflictingTimeRange{dw::add_offset(timeRange, 15min)};
    someTask.addSprint(Sprint{timeRange});

    ASSERT_THROW(someTask.addSprint(Sprint{conflictingTimeRange}),
                 sprint_timer::SprintTimerException);
}

TEST(TestTask, accepts_sprints_when_there_are_no_conflicts)
{
    constexpr auto modificationStamp =
        DateTime{Date{Year{2016}, Month{9}, Day{21}}} + 12h + 59min + 19s;
    Task someTask{"Some name",
                  4,
                  {},
                  "123",
                  {},
                  false,
                  modificationStamp,
                  TaskType::Regular,
                  std::nullopt,
                  TaskTimeframe{}};
    const DateTimeRange timeRange{dw::current_date_time(),
                                  dw::current_date_time() + 25min};
    const std::vector<Sprint> sprints{
        Sprint{timeRange},
        Sprint{add_offset(timeRange, 25min)},
        Sprint{add_offset(timeRange, 24h)},
        Sprint{add_offset(timeRange, 24h + 25min)}};

    for (const auto& sprint : sprints) {
        someTask.addSprint(sprint);
    }

    EXPECT_TRUE(std::ranges::equal(sprints, someTask.sprints()));
    EXPECT_EQ(4, someTask.actualCost());
    EXPECT_EQ(current_date_time_local(), someTask.lastModified());
}

TEST(TestTask,
     editing_task_updates_timestamp_but_does_not_alters_sprints_and_uuid)
{

    constexpr auto modificationStamp =
        DateTime{Date{Year{2016}, Month{9}, Day{21}}} + 12h + 59min + 19s;
    const DateTimeRange timeRange{dw::current_date_time(),
                                  dw::current_date_time() + 25min};
    const std::vector<Sprint> sprints{
        Sprint{timeRange},
        Sprint{add_offset(timeRange, 25min)},
        Sprint{add_offset(timeRange, 24h)},
        Sprint{add_offset(timeRange, 24h + 25min)}};
    const std::string uuid{"123"};
    const Task sut{"Some name",
                   4,
                   sprints,
                   uuid,
                   {Tag{"Tag1"}, Tag{"Tag2"}},
                   false,
                   modificationStamp,
                   TaskType::Regular,
                   Note{"Some task note"},
                   TaskTimeframe{modificationStamp - Days{3},
                                 modificationStamp + Days{10},
                                 modificationStamp + Days{3} - 15min,
                                 Recurrence{"rec_string"}}};
    const Task desiredTask{"Altered name",
                           7,
                           {},
                           "7777777",
                           {Tag{"Altered_tag"}},
                           true,
                           modificationStamp,
                           TaskType::Folder,
                           Note{"Altered note"},
                           TaskTimeframe{modificationStamp - Days{5},
                                         modificationStamp + Days{20},
                                         modificationStamp + Days{20} - 1h,
                                         Recurrence{"altered_rec_string"}}};
    const Task expected{"Altered name",
                        7,
                        sprints,
                        uuid,
                        {Tag{"Altered_tag"}},
                        false,
                        dw::current_date_time_local(),
                        TaskType::Folder,
                        Note{"Altered note"},
                        TaskTimeframe{modificationStamp - Days{5},
                                      modificationStamp + Days{20},
                                      modificationStamp + Days{20} - 1h,
                                      Recurrence{"altered_rec_string"}}};

    EXPECT_EQ(expected, sut.edit(desiredTask));
}
