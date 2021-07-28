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
#include "core/entities/Task.h"
#include "gtest/gtest.h"

using namespace sprint_timer::entities;
using dw::DateTime;

TEST(TestTask, adding_sprint)
{
    using namespace std::chrono_literals;
    using namespace dw;
    const std::string taskName{"Some name"};
    const std::string taskUuid{"123"};
    constexpr auto modificationStamp =
        DateTime{Date{Year{2016}, Month{9}, Day{21}}} + 12h + 59min + 19s;
    Task someTask{taskUuid, taskName, 4, modificationStamp};

    someTask.addSprint(Sprint{taskName,
                              DateTimeRange{current_date_time_local(),
                                            current_date_time_local() + 25min},
                              std::list<Tag>{},
                              "777",
                              taskUuid});
    someTask.addSprint(Sprint{taskName,
                              DateTimeRange{current_date_time_local() + 25min,
                                            current_date_time_local() + 50min},
                              std::list<Tag>{},
                              "888",
                              taskUuid});

    EXPECT_EQ(2, someTask.actualCost());
    EXPECT_EQ(current_date_time_local(), someTask.lastModified());
}

TEST(TestTask, rejects_sprint_if_it_intersects_with_others)
{
    using namespace dw;
    using namespace std::chrono_literals;
    constexpr auto modificationStamp =
        DateTime{Date{Year{2016}, Month{9}, Day{21}}} + 12h + 59min + 19s;
    Task someTask{"123", "Some name", 4, modificationStamp};
    const DateTimeRange timeRange{dw::current_date_time(),
                                  dw::current_date_time() + 25min};
    const DateTimeRange conflictingTimeRange{dw::add_offset(timeRange, 15min)};
    someTask.addSprint(
        Sprint{"Some name", timeRange, std::list<Tag>{}, "111", "123"});

    ASSERT_THROW(
        someTask.addSprint(Sprint{
            "Some name", conflictingTimeRange, std::list<Tag>{}, "222", "123"}),
        sprint_timer::SprintTimerException);
}

TEST(TestTask, accepts_sprints_when_there_are_no_conflicts)
{
    using namespace dw;
    using namespace std::chrono_literals;
    constexpr auto modificationStamp =
        DateTime{Date{Year{2016}, Month{9}, Day{21}}} + 12h + 59min + 19s;
    Task someTask{"123", "Some name", 4, modificationStamp};
    const DateTimeRange timeRange{dw::current_date_time(),
                                  dw::current_date_time() + 25min};
    const std::vector<Sprint> sprints{
        Sprint{"Some name", timeRange, std::list<Tag>{}, "1", "123"},
        Sprint{"Some name",
               add_offset(timeRange, 25min),
               std::list<Tag>{},
               "2",
               "123"},
        Sprint{"Some name",
               add_offset(timeRange, 24h),
               std::list<Tag>{},
               "3",
               "123"},
        Sprint{"Some name",
               add_offset(timeRange, 24h + 25min),
               std::list<Tag>{},
               "4",
               "123"}};

    for (const auto& sprint : sprints) {
        someTask.addSprint(sprint);
    }

    EXPECT_EQ(sprints, someTask.sprints());
    EXPECT_EQ(4, someTask.actualCost());
    EXPECT_EQ(current_date_time_local(), someTask.lastModified());
}

