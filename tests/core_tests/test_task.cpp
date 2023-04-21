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
                                 Recurrence{"Mon,Wed"}}};
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
                                         Recurrence{"Fri *-03/2-*"}}};
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
                                      Recurrence{"Fri *-03/2-*"}}};

    EXPECT_EQ(expected, sut.edit(desiredTask, dw::current_date_time_local()));
}

TEST(TestTask, inherits_due_date_unconditionally)
{
    constexpr auto modificationStamp =
        DateTime{Date{Year{2016}, Month{9}, Day{21}}} + 12h + 59min + 19s;
    const auto time = modificationStamp + Days{10};
    const auto previouslySetDueDate = modificationStamp + Days{12};
    const auto currentTimeNow = dw::current_date_time_local();
    const Task original{"Some task",
                        7,
                        {},
                        "123",
                        {},
                        false,
                        modificationStamp,
                        TaskType::Regular,
                        std::nullopt,
                        TaskTimeframe{modificationStamp,
                                      previouslySetDueDate,
                                      std::nullopt,
                                      std::nullopt}};
    const Task other{
        "Other Task",
        8,
        {},
        "345",
        {},
        false,
        time,
        TaskType::Project,
        std::nullopt,
        TaskTimeframe{modificationStamp, time, std::nullopt, std::nullopt}};
    const Task expected{
        original.name(),
        original.estimatedCost(),
        std::vector<Sprint>(cbegin(original.sprints()),
                            cend(original.sprints())),
        original.uuid(),
        std::vector<Tag>(cbegin(original.tags()), cend(original.tags())),
        original.isCompleted(),
        currentTimeNow,
        original.kind(),
        original.notes(),
        TaskTimeframe{modificationStamp, time, std::nullopt, std::nullopt}};

    const auto actual = original.inheritDate(other, currentTimeNow);

    EXPECT_EQ(expected, actual);
}

TEST(TestTask, inherits_due_date_from_other_task_if_due_date_was_not_set)
{
    constexpr auto modificationStamp =
        DateTime{Date{Year{2016}, Month{9}, Day{21}}} + 12h + 59min + 19s;
    const auto time = modificationStamp + Days{10};
    const auto currentTimeNow = dw::current_date_time_local();
    const Task original{
        "Some task",
        7,
        {},
        "123",
        {},
        false,
        modificationStamp,
        TaskType::Regular,
        std::nullopt,
        TaskTimeframe{
            modificationStamp, std::nullopt, std::nullopt, std::nullopt}};
    const Task other{
        "Other Task",
        8,
        {},
        "345",
        {},
        false,
        modificationStamp + Days{20},
        TaskType::Project,
        std::nullopt,
        TaskTimeframe{modificationStamp, time, std::nullopt, std::nullopt}};
    const Task expected{
        original.name(),
        original.estimatedCost(),
        std::vector<Sprint>(cbegin(original.sprints()),
                            cend(original.sprints())),
        original.uuid(),
        std::vector<Tag>(cbegin(original.tags()), cend(original.tags())),
        original.isCompleted(),
        currentTimeNow,
        original.kind(),
        original.notes(),
        TaskTimeframe{modificationStamp, time, std::nullopt, std::nullopt}};

    const auto actual = original.inheritDateIfNotSet(other, currentTimeNow);

    EXPECT_EQ(expected, actual);
}

TEST(TestTask,
     does_not_inherit_due_date_from_other_task_if_due_date_already_set)
{
    constexpr auto modificationStamp =
        DateTime{Date{Year{2016}, Month{9}, Day{21}}} + 12h + 59min + 19s;
    const auto time = modificationStamp + Days{10};
    const auto previouslySetDueDate = modificationStamp + Days{12};
    const auto currentTimeNow = dw::current_date_time_local();
    const Task original{"Some task",
                        7,
                        {},
                        "123",
                        {},
                        false,
                        modificationStamp,
                        TaskType::Regular,
                        std::nullopt,
                        TaskTimeframe{modificationStamp,
                                      previouslySetDueDate,
                                      std::nullopt,
                                      std::nullopt}};
    const Task other{
        "Other Task",
        8,
        {},
        "345",
        {},
        false,
        modificationStamp + Days{20},
        TaskType::Project,
        std::nullopt,
        TaskTimeframe{modificationStamp, time, std::nullopt, std::nullopt}};
    const Task expected{
        original.name(),
        original.estimatedCost(),
        std::vector<Sprint>(cbegin(original.sprints()),
                            cend(original.sprints())),
        original.uuid(),
        std::vector<Tag>(cbegin(original.tags()), cend(original.tags())),
        original.isCompleted(),
        modificationStamp,
        original.kind(),
        original.notes(),
        TaskTimeframe{modificationStamp,
                      previouslySetDueDate,
                      std::nullopt,
                      std::nullopt}};

    const auto actual = original.inheritDateIfNotSet(other, currentTimeNow);

    EXPECT_EQ(expected, actual);
}

TEST(TestTask, returns_none_recurrence_when_recurrence_is_not_set)
{
    const auto someIrrelevantTimeStamp = dw::current_date_time();
    const Task nonRecurringTask{
        "Some task",
        7,
        {},
        "123",
        {},
        false,
        someIrrelevantTimeStamp,
        TaskType::Regular,
        std::nullopt,
        TaskTimeframe{
            someIrrelevantTimeStamp, std::nullopt, std::nullopt, std::nullopt}};

    EXPECT_EQ(std::nullopt,
              nonRecurringTask.nextRecurrence("123", someIrrelevantTimeStamp));
}

TEST(TestTask, generates_next_recurrence_of_recurring_task)
{
    constexpr auto modificationStamp =
        DateTime{Date{Year{2016}, Month{9}, Day{21}}} + 12h + 59min + 19s;
    const Task task{"Some task",
                    7,
                    {},
                    "123",
                    {},
                    false,
                    modificationStamp,
                    TaskType::Regular,
                    std::nullopt,
                    TaskTimeframe{modificationStamp + Days{10},
                                  std::nullopt,
                                  std::nullopt,
                                  Recurrence{"*-*-10 21:20:00"}}};
    const std::string nextUuid{"345678"};
    const auto nextRecurrenceDateTime =
        DateTime{Date{Year{2016}, Month{10}, Day{10}}} + 21h + 20min;
    const auto currentTime =
        DateTime{Date{Year{2016}, Month{10}, Day{1}}} + 10h;
    const Task expected{"Some task",
                        7,
                        {},
                        nextUuid,
                        {},
                        false,
                        currentTime,
                        TaskType::Regular,
                        std::nullopt,
                        TaskTimeframe{currentTime,
                                      nextRecurrenceDateTime,
                                      std::nullopt,
                                      Recurrence{"*-*-10 21:20:00"}}};

    const auto actual = task.nextRecurrence(nextUuid, currentTime);

    EXPECT_TRUE(actual);
    EXPECT_EQ(expected, actual.value());
}

TEST(TestTask, recurrence_of_non_recurring_task_is_none)
{
    const auto modificationStamp = current_date_time();
    const Task task{"Some task",
                    7,
                    {},
                    "123",
                    {},
                    false,
                    modificationStamp,
                    TaskType::Regular,
                    std::nullopt,
                    TaskTimeframe{modificationStamp + Days{10},
                                  std::nullopt,
                                  std::nullopt,
                                  std::nullopt}};

    EXPECT_FALSE(task.nextRecurrence("1234", current_date_time_local()));
}

TEST(TestTask,
     recurrence_of_non_recurring_task_is_none_when_no_next_recurrence_possible)
{
    const auto currentTime =
        DateTime{Date{Year{2016}, Month{10}, Day{1}}} + 10h;
    const Task task{"Some task",
                    7,
                    {},
                    "777",
                    {},
                    false,
                    currentTime,
                    TaskType::Regular,
                    std::nullopt,
                    TaskTimeframe{currentTime,
                                  currentTime + Days{5},
                                  std::nullopt,
                                  Recurrence{"2016-01..09-10 21:20:00"}}};

    EXPECT_FALSE(task.nextRecurrence("12345", currentTime));
}

TEST(TestTask, throws_when_recurrence_is_set_without_due_date)
{
    constexpr auto modificationStamp =
        DateTime{Date{Year{2016}, Month{9}, Day{21}}} + 12h + 59min + 19s;

    ASSERT_THROW(Task("Some task",
                      7,
                      {},
                      "123",
                      {},
                      false,
                      modificationStamp,
                      TaskType::Regular,
                      std::nullopt,
                      TaskTimeframe{modificationStamp + Days{10},
                                    std::nullopt,
                                    std::nullopt,
                                    Recurrence{"*-*-10 21:20:00"}}),
                 std::runtime_error);
}

