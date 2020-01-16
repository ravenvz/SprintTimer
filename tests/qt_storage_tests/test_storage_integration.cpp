/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "QtStorageInitializer.h"
#include "gtest/gtest.h"
#include <core/SprintBuilder.h>
#include <core/TaskBuilder.h>
#include <core/WorkSchedule.h>

using namespace sprint_timer::storage::qt_storage;
using sprint_timer::SprintBuilder;
using sprint_timer::SprintStorageReader;
using sprint_timer::TaskBuilder;
using sprint_timer::TaskStorageReader;
using sprint_timer::entities::Sprint;
using sprint_timer::entities::Task;

sprint_timer::WeekSchedule
buildWeekSchedule(const std::array<int, 7>& raw_schedule)
{
    sprint_timer::WeekSchedule schedule;
    for (size_t i = 0; i < raw_schedule.size(); ++i)
        schedule.setTargetGoal(static_cast<dw::Weekday>(i), raw_schedule[i]);
    return schedule;
}

class QtStorageTestFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
};

TEST_F(QtStorageTestFixture, saves_task)
{
    const Task someTask = TaskBuilder{}.withName("Some name").build();
    auto taskStorage = initializer.factory.taskStorage();

    taskStorage->save(someTask);
    const auto tasks = taskStorage->unfinishedTasks();
    ASSERT_EQ(1, tasks.size());
    ASSERT_EQ(someTask, tasks.front());
}

TEST_F(QtStorageTestFixture, remove_task_with_no_sprints)
{
    const Task someTask = TaskBuilder{}.withUuid("123").build();
    auto taskStorage = initializer.factory.taskStorage();

    taskStorage->save(someTask);
    taskStorage->remove(someTask.uuid());

    const auto tasks = taskStorage->unfinishedTasks();
    ASSERT_TRUE(tasks.empty());
}

TEST_F(QtStorageTestFixture, toggles_task_completion)
{
    auto taskStorage = initializer.factory.taskStorage();
    const Task someTask =
        TaskBuilder{}
            .withUuid("123")
            .withCompletionStatus(true)
            .withLastModificationStamp(dw::current_date_time())
            .build();

    taskStorage->save(someTask);
    taskStorage->toggleCompleted(someTask.uuid(), someTask.lastModified());

    const auto tasks = taskStorage->unfinishedTasks();
    ASSERT_EQ(1, tasks.size());
    ASSERT_FALSE(someTask.isCompleted() == tasks.front().isCompleted());
}

TEST_F(QtStorageTestFixture, retrieves_unfinished_tasks)
{
    auto taskStorage = initializer.factory.taskStorage();
    std::vector<Task> unfinishedTasks;
    std::vector<Task> finishedTasks;
    TaskBuilder builder;
    unfinishedTasks.push_back(builder.withCompletionStatus(false).build());
    unfinishedTasks.push_back(builder.withCompletionStatus(false).build());
    finishedTasks.push_back(
        builder.withCompletionStatus(true)
            .withLastModificationStamp(dw::current_date_time() - dw::Days{2})
            .build());
    finishedTasks.push_back(
        builder.withCompletionStatus(true)
            .withLastModificationStamp(dw::current_date_time() - dw::Days{2})
            .build());

    for (const auto& task : unfinishedTasks)
        taskStorage->save(task);
    for (const auto& task : finishedTasks)
        taskStorage->save(task);

    const auto tasks = taskStorage->unfinishedTasks();

    ASSERT_EQ(unfinishedTasks, tasks);
}

TEST_F(QtStorageTestFixture, recently_completed_tasks_considered_unfinished)
{
    auto taskStorage = initializer.factory.taskStorage();
    std::vector<Task> unfinishedTasks;
    std::vector<Task> finishedTasks;
    TaskBuilder builder;
    unfinishedTasks.push_back(builder.withCompletionStatus(false).build());
    unfinishedTasks.push_back(builder.withCompletionStatus(false).build());
    finishedTasks.push_back(
        builder.withCompletionStatus(true)
            .withLastModificationStamp(dw::current_date_time() - dw::Days{1})
            .build());
    finishedTasks.push_back(
        builder.withCompletionStatus(true)
            .withLastModificationStamp(dw::current_date_time() - dw::Days{1})
            .build());
    std::vector<Task> recentTasks = unfinishedTasks;
    std::copy(cbegin(finishedTasks),
              cend(finishedTasks),
              std::back_inserter(recentTasks));

    for (const auto& task : unfinishedTasks)
        taskStorage->save(task);
    for (const auto& task : finishedTasks)
        taskStorage->save(task);

    const auto tasks = taskStorage->unfinishedTasks();

    ASSERT_EQ(recentTasks, tasks);
}

TEST_F(QtStorageTestFixture, edits_task)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const Task someTask =
        TaskBuilder{}
            .withName("Edit me")
            .withTag("Tag")
            .withEstimatedCost(4)
            .withActualCost(2)
            .withLastModificationStamp(dw::current_date_time() - dw::Days{3})
            .withCompletionStatus(true)
            .build();
    const Task expectedEditedTask =
        TaskBuilder{}
            .withUuid(someTask.uuid())
            .withName("I'm edited")
            .withTag("EditedTag")
            .withEstimatedCost(42)
            .withActualCost(someTask.actualCost())
            .withLastModificationStamp(dw::current_date_time())
            .withCompletionStatus(someTask.isCompleted())
            .build();

    taskStorage->save(someTask);
    taskStorage->edit(someTask, expectedEditedTask);

    const auto tasks = taskStorage->unfinishedTasks();

    ASSERT_EQ(1, tasks.size());
    ASSERT_EQ(expectedEditedTask, tasks.front());
}

TEST_F(QtStorageTestFixture, edits_tag)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const std::string tagToEdit{"TagToEdit"};
    const std::string editedTag{"EditedTag"};
    const Task someTask = TaskBuilder{}.withTag(tagToEdit).build();

    taskStorage->save(someTask);
    taskStorage->editTag(tagToEdit, editedTag);

    const auto tags = taskStorage->allTags();

    ASSERT_EQ(1, tags.size());
    ASSERT_EQ(editedTag, tags.front());
}

TEST_F(QtStorageTestFixture, updates_task_priorities)
{
    const auto taskStorage = initializer.factory.taskStorage();
    TaskBuilder builder;
    std::vector<Task> someTasks;
    const std::vector<std::string> priorities{"3", "2", "1"};

    for (int i = 0; i < 3; ++i) {
        someTasks.push_back(builder.withUuid(std::to_string(i + 1)).build());
        taskStorage->save(someTasks.back());
    }
    taskStorage->updatePriorities(priorities);

    const auto tasks = taskStorage->unfinishedTasks();

    ASSERT_TRUE(std::equal(
        cbegin(priorities),
        cend(priorities),
        cbegin(tasks),
        cend(tasks),
        [](const auto& lhs, const auto& rhs) { return lhs == rhs.uuid(); }));
}

TEST_F(QtStorageTestFixture, removing_task_with_sprints_deletes_sprints)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    const Task someTask = TaskBuilder{}.build();
    SprintBuilder sprintBuilder;
    const dw::DateTimeRange timeSpan{dw::current_date_time(),
                                     dw::current_date_time()};

    taskStorage->save(someTask);
    sprintStorage->save(sprintBuilder.withTaskUuid(someTask.uuid())
                            .withTimeSpan(timeSpan)
                            .build());
    taskStorage->remove(someTask.uuid());

    const auto sprints = sprintStorage->findByTaskUuid(someTask.uuid());

    ASSERT_TRUE(sprints.empty());
}

TEST_F(QtStorageTestFixture, requests_all_tasks_in_given_date_range)
{
    using namespace dw;
    const auto taskStorage = initializer.factory.taskStorage();
    const DateRange targetDateRange{Date{Year{2018}, Month{10}, Day{10}},
                                    Date{Year{2018}, Month{10}, Day{15}}};
    const DateTime leftmostDateTime{DateTime{targetDateRange.start()}};
    const DateTime rightmostDateTime{DateTime{targetDateRange.finish()}};
    TaskBuilder builder;
    const Task taskOutOfRangeLeft =
        builder.withLastModificationStamp(leftmostDateTime - Days{1}).build();
    const Task taskLeftRangeBorder =
        builder.withLastModificationStamp(leftmostDateTime).build();
    const Task insideRangeTask1 =
        builder.withLastModificationStamp(leftmostDateTime + Days{1}).build();
    const Task insideRangeTask2 =
        builder.withLastModificationStamp(rightmostDateTime - Days{1}).build();
    const Task taskRightRangeBorder =
        builder.withLastModificationStamp(rightmostDateTime).build();
    const Task taskOutOfRangeRight =
        builder.withLastModificationStamp(rightmostDateTime + Days{1}).build();
    const std::vector<Task> expected{taskLeftRangeBorder,
                                     insideRangeTask1,
                                     insideRangeTask2,
                                     taskRightRangeBorder};

    taskStorage->save(taskOutOfRangeLeft);
    taskStorage->save(taskLeftRangeBorder);
    taskStorage->save(insideRangeTask1);
    taskStorage->save(insideRangeTask2);
    taskStorage->save(taskRightRangeBorder);
    taskStorage->save(taskOutOfRangeRight);

    const auto tasks = taskStorage->allTasks(targetDateRange);

    ASSERT_EQ(expected.size(), tasks.size());
    ASSERT_EQ(expected, tasks);
}

TEST_F(QtStorageTestFixture,
       inserting_sprint_increments_associated_task_actual_count)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    const Task someTask = TaskBuilder{}.build();
    const Task expectedTask =
        TaskBuilder{}
            .withUuid(someTask.uuid())
            .withActualCost(2)
            .withLastModificationStamp(someTask.lastModified())
            .build();
    const dw::DateTimeRange timeSpan{dw::current_date_time(),
                                     dw::current_date_time()};
    SprintBuilder builder;

    taskStorage->save(someTask);
    sprintStorage->save(
        builder.forTask(someTask).withTimeSpan(timeSpan).build());
    sprintStorage->save(
        builder.forTask(someTask).withTimeSpan(timeSpan).build());

    const auto tasks = taskStorage->unfinishedTasks();

    ASSERT_EQ(1, tasks.size());
    ASSERT_EQ(expectedTask, tasks.front());
}

TEST_F(QtStorageTestFixture,
       removing_sprint_decrements_associated_task_actual_count)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    const Task someTask = TaskBuilder{}.build();
    const Task expectedTask =
        TaskBuilder{}
            .withUuid(someTask.uuid())
            .withActualCost(1)
            .withLastModificationStamp(someTask.lastModified())
            .build();
    const dw::DateTimeRange timeSpan{dw::current_date_time(),
                                     dw::current_date_time()};
    SprintBuilder builder;
    const Sprint sprint =
        builder.forTask(someTask).withTimeSpan(timeSpan).build();

    taskStorage->save(someTask);
    sprintStorage->save(sprint);
    sprintStorage->save(
        builder.forTask(someTask).withTimeSpan(timeSpan).build());
    sprintStorage->remove(sprint);

    const auto tasks = taskStorage->unfinishedTasks();

    ASSERT_EQ(1, tasks.size());
    ASSERT_EQ(expectedTask, tasks.front());
}

TEST_F(QtStorageTestFixture, retrieves_tags)
{
    const auto taskStorage = initializer.factory.taskStorage();
    using sprint_timer::entities::Tag;
    TaskBuilder builder;

    taskStorage->save(
        builder.withExplicitTags({Tag{"Tag1"}, Tag{"Tag2"}}).build());
    taskStorage->save(
        builder.withExplicitTags({Tag{"Tag3"}, Tag{"Tag4"}}).build());
    const std::vector<std::string> expected{"Tag1", "Tag2", "Tag3", "Tag4"};

    auto tags = taskStorage->allTags();

    std::sort(begin(tags), end(tags));
    ASSERT_EQ(expected, tags);
}

TEST_F(QtStorageTestFixture,
       removing_task_does_not_remove_tag_if_any_other_task_has_it)
{
    const auto taskStorage = initializer.factory.taskStorage();
    TaskBuilder builder;
    const std::string someTag{"Tag"};
    const Task someTask = builder.withTag(someTag).build();
    const Task someOtherTask = builder.withTag(someTag).build();

    taskStorage->save(someTask);
    taskStorage->save(someOtherTask);
    taskStorage->remove(someTask.uuid());

    const auto tags = taskStorage->allTags();
    ASSERT_EQ(1, tags.size());
    ASSERT_EQ(someTag, tags.front());
}

TEST_F(QtStorageTestFixture, removes_orphaned_tags_when_deleting_task)
{
    const auto taskStorage = initializer.factory.taskStorage();
    using sprint_timer::entities::Tag;
    const Task someTask =
        TaskBuilder{}.withExplicitTags({Tag{"Tag1"}, Tag{"Tag2"}}).build();

    taskStorage->save(someTask);
    taskStorage->remove(someTask.uuid());

    const auto tags = taskStorage->allTags();

    ASSERT_TRUE(tags.empty());
}

TEST_F(QtStorageTestFixture, retrieves_sprints_for_given_task)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    const Task someTask{TaskBuilder{}.build()};
    const Task irrelevantTask{TaskBuilder{}.build()};
    SprintBuilder sprintBuilder;
    const dw::DateTimeRange timeSpan{dw::current_date_time(),
                                     dw::current_date_time()};
    const std::vector<Sprint> sprints{
        sprintBuilder.forTask(someTask).withTimeSpan(timeSpan).build(),
        sprintBuilder.forTask(irrelevantTask).withTimeSpan(timeSpan).build(),
        sprintBuilder.forTask(someTask).withTimeSpan(timeSpan).build(),
        sprintBuilder.forTask(irrelevantTask).withTimeSpan(timeSpan).build(),
    };
    std::vector<Sprint> expectedSprints;
    std::copy_if(cbegin(sprints),
                 cend(sprints),
                 std::back_inserter(expectedSprints),
                 [&someTask](const auto& sprint) {
                     return sprint.taskUuid() == someTask.uuid();
                 });

    taskStorage->save(someTask);
    taskStorage->save(irrelevantTask);
    sprintStorage->save(sprints);

    const auto taskSprints = sprintStorage->findByTaskUuid(someTask.uuid());

    ASSERT_EQ(2, taskSprints.size());
    ASSERT_EQ(expectedSprints, taskSprints);
}

TEST_F(QtStorageTestFixture, retrieves_sprints_in_given_date_range)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    using namespace dw;
    SprintBuilder sprintBuilder;
    const DateTime timestamp{current_date_time()};
    const Task someTask = TaskBuilder{}.build();
    const DateRange range{(timestamp - Days{5}).date(),
                          (timestamp + Days{5}).date()};
    const Sprint outOfRangeLeft{
        sprintBuilder.withTaskUuid(someTask.uuid())
            .withTimeSpan(add_offset({timestamp, timestamp}, -Days{20}))
            .build()};
    const Sprint onLeftBorder{
        sprintBuilder.withTaskUuid(someTask.uuid())
            .withTimeSpan({DateTime{range.start()}, DateTime{range.start()}})
            .build()};
    const Sprint inRange{sprintBuilder.withTaskUuid(someTask.uuid())
                             .withTimeSpan(DateTimeRange{timestamp, timestamp})
                             .build()};
    const Sprint onRightBorder{
        sprintBuilder.withTaskUuid(someTask.uuid())
            .withTimeSpan({DateTime{range.finish()}, DateTime{range.finish()}})
            .build()};
    const Sprint outOfRangeRight{
        sprintBuilder.withTaskUuid(someTask.uuid())
            .withTimeSpan(add_offset(
                {DateTime{range.finish()}, DateTime{range.finish()}}, Days{1}))
            .build()};
    const std::vector<Sprint> expected{onLeftBorder, inRange, onRightBorder};

    taskStorage->save(someTask);
    sprintStorage->save(outOfRangeLeft);
    sprintStorage->save(onLeftBorder);
    sprintStorage->save(inRange);
    sprintStorage->save(onRightBorder);
    sprintStorage->save(outOfRangeRight);

    const auto sprints = sprintStorage->findByDateRange(range);

    ASSERT_EQ(3, sprints.size());
    ASSERT_EQ(expected, sprints);
}

TEST_F(QtStorageTestFixture,
       retrieves_operation_range_using_current_date_as_upper_bound)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    const auto operationalRangeReader =
        initializer.factory.operationalRangeReader();
    using namespace dw;
    const Task someTask{TaskBuilder{}.build()};
    const dw::DateTime timestamp{
        dw::DateTime{Date{Year{2018}, Month{12}, Day{1}}}};
    const Sprint right{SprintBuilder{}
                           .withTaskUuid(someTask.uuid())
                           .withTimeSpan(add_offset(
                               DateTimeRange{timestamp, timestamp}, -Years{4}))
                           .build()};
    const DateRange expected{Date{Year{2014}, Month{12}, Day{1}},
                             current_date_local()};

    taskStorage->save(someTask);
    sprintStorage->save(right);

    const auto operationalRange = operationalRangeReader->operationalRange();

    ASSERT_EQ(expected, operationalRange);
}

TEST_F(QtStorageTestFixture,
       retrieves_operational_range_when_having_some_future_sprints)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    const auto operationalRangeReader =
        initializer.factory.operationalRangeReader();
    using namespace dw;
    const Task someTask{TaskBuilder{}.build()};
    const dw::DateTime timestamp{
        dw::DateTime{Date{Year{2018}, Month{12}, Day{1}}}};
    const Sprint left{SprintBuilder{}
                          .withTaskUuid(someTask.uuid())
                          .withTimeSpan(add_offset(
                              DateTimeRange{timestamp, timestamp}, -Years{4}))
                          .build()};
    const Sprint right{
        SprintBuilder{}
            .withTaskUuid(someTask.uuid())
            .withTimeSpan(add_offset(DateTimeRange{current_date_time_local(),
                                                   current_date_time_local()},
                                     Years{4}))
            .build()};
    const DateRange expected{Date{Year{2014}, Month{12}, Day{1}},
                             current_date_local() + Years{4}};

    taskStorage->save(someTask);
    sprintStorage->save(left);
    sprintStorage->save(right);

    const auto operationalRange = operationalRangeReader->operationalRange();

    ASSERT_EQ(expected, operationalRange);
}

TEST_F(QtStorageTestFixture, saves_sprints_in_bulk)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    using namespace dw;
    const Task someTask{TaskBuilder{}.build()};
    const DateTimeRange timeSpan{DateTime{Date{Year{2018}, Month{12}, Day{1}}},
                                 DateTime{Date{Year{2018}, Month{12}, Day{1}}}};
    std::vector<Sprint> sprintBulk;
    SprintBuilder sprintBuilder{
        SprintBuilder{}.withTaskUuid(someTask.uuid()).withTimeSpan(timeSpan)};
    std::generate_n(std::back_inserter(sprintBulk), 5, [&sprintBuilder]() {
        return sprintBuilder.build();
    });

    taskStorage->save(someTask);
    sprintStorage->save(sprintBulk);

    const auto sprints = sprintStorage->findByTaskUuid(someTask.uuid());

    ASSERT_EQ(sprintBulk, sprints);
}

TEST_F(QtStorageTestFixture, removes_sprints_in_bulk)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    using namespace dw;
    const Task someTask{TaskBuilder{}.build()};
    const Task otherTask{TaskBuilder{}.build()};
    const DateRange dateRange{Date{Year{2018}, Month{12}, Day{1}},
                              Date{Year{2018}, Month{12}, Day{1}}};
    std::vector<Sprint> sprintBulk;
    SprintBuilder sprintBuilder{SprintBuilder{}.withTimeSpan(DateTimeRange{
        DateTime{dateRange.start()}, DateTime{dateRange.finish()}})};
    std::generate_n(
        std::back_inserter(sprintBulk), 2, [&sprintBuilder, &someTask]() {
            return sprintBuilder.withTaskUuid(someTask.uuid()).build();
        });
    std::generate_n(
        std::back_inserter(sprintBulk), 2, [&sprintBuilder, &otherTask]() {
            return sprintBuilder.withTaskUuid(otherTask.uuid()).build();
        });

    taskStorage->save(someTask);
    taskStorage->save(otherTask);
    sprintStorage->save(sprintBulk);
    sprintStorage->remove(sprintBulk);

    const auto sprints = sprintStorage->findByDateRange(dateRange);

    ASSERT_TRUE(sprints.empty());
}

TEST_F(QtStorageTestFixture, reads_sprint_daily_distribution)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    const auto dailyDistReader = initializer.factory.dailyDistReader();
    using namespace dw;
    const Task someTask{TaskBuilder{}.build()};
    SprintBuilder sprintBuilder{SprintBuilder{}.withTaskUuid(someTask.uuid())};
    const DateTime someDate{Date{Year{2018}, Month{12}, Day{2}}};
    const DateRange range{(someDate - Days{29}).date(), someDate.date()};
    std::vector<Sprint> sprints;
    std::vector<int> expected{4, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2};

    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate - Days{30}};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    std::generate_n(
        std::back_inserter(sprints), 4, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate - Days{29}};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    std::generate_n(
        std::back_inserter(sprints), 3, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate - Days{20}};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    std::generate_n(
        std::back_inserter(sprints), 7, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate - Days{10}};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &someDate]() {
            return sprintBuilder.withTimeSpan(DateTimeRange{someDate, someDate})
                .build();
        });
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 3, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate + Days{1}};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });

    taskStorage->save(someTask);
    sprintStorage->save(sprints);

    const auto distribution = dailyDistReader->sprintDistribution(range);

    ASSERT_EQ(expected, distribution);
}

TEST_F(QtStorageTestFixture, retrieves_monthly_distribution)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    const auto monthlyDistReader = initializer.factory.monthlyDistReader();
    using namespace dw;
    auto first_day_of_month = [](const Date& date) {
        return Date{date.year(), date.month(), Day{1}};
    };
    const Task someTask{TaskBuilder{}.build()};
    SprintBuilder sprintBuilder{SprintBuilder{}.withTaskUuid(someTask.uuid())};
    const Date someDate{Year{2018}, Month{12}, Day{2}};
    Date lowerDate{first_day_of_month(someDate - Months{11})};
    const std::vector<int> expected{3, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 5};
    std::vector<Sprint> sprints;
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{
                last_day_of_month(someDate - Months(12))};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // On border
    std::generate_n(std::back_inserter(sprints),
                    3,
                    [&sprintBuilder, &someDate, &first_day_of_month]() {
                        const dw::DateTime timestamp{
                            first_day_of_month(someDate - Months{11})};
                        return sprintBuilder
                            .withTimeSpan(DateTimeRange{timestamp, timestamp})
                            .build();
                    });
    // Somewhere in the middle
    std::generate_n(
        std::back_inserter(sprints), 4, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate - Months{5}};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // On border
    std::generate_n(
        std::back_inserter(sprints), 5, [&sprintBuilder, &someDate]() {
            return sprintBuilder
                .withTimeSpan(
                    DateTimeRange{DateTime{someDate}, DateTime{someDate}})
                .build();
        });
    // Out of range
    std::generate_n(std::back_inserter(sprints),
                    6,
                    [&sprintBuilder, &someDate, &first_day_of_month]() {
                        const dw::DateTime timestamp{
                            first_day_of_month(someDate + Months{1})};
                        return sprintBuilder
                            .withTimeSpan(DateTimeRange{timestamp, timestamp})
                            .build();
                    });

    taskStorage->save(someTask);
    sprintStorage->save(sprints);

    const auto distribution =
        monthlyDistReader->sprintDistribution(DateRange{lowerDate, someDate});

    ASSERT_EQ(expected, distribution);
}

TEST_F(QtStorageTestFixture,
       retrieves_sprint_weekly_distribution_with_monday_first_setting)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    const auto mondayFirstWeeklyDistributionReader =
        initializer.factory.weeklyDistReader(dw::Weekday::Monday);
    using namespace dw;
    const Task someTask{TaskBuilder{}.build()};
    SprintBuilder sprintBuilder{SprintBuilder{}.withTaskUuid(someTask.uuid())};
    const dw::DateTime upperDate{Date{Year{2016}, Month{2}, Day{12}}};
    const dw::DateTime lowerDate{upperDate - Weeks(11)};
    const std::vector<int> expected{3, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 6};
    std::vector<Sprint> sprints;
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &lowerDate]() {
            const dw::DateTime timestamp{lowerDate - Days{1}};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // On lower border
    std::generate_n(
        std::back_inserter(sprints), 3, [&sprintBuilder, &lowerDate]() {
            return sprintBuilder
                .withTimeSpan(DateTimeRange{lowerDate, lowerDate})
                .build();
        });
    // Week 53 in 2015
    std::generate_n(
        std::back_inserter(sprints), 4, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate +
                                         Days(-6 * 7 - 4)}; // Monday
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // Week 53 in 2016
    std::generate_n(
        std::back_inserter(sprints), 5, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate +
                                         Days(-6 * 7 + 2)}; // Sunday
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // On upper border
    std::generate_n(
        std::back_inserter(sprints), 6, [&sprintBuilder, &upperDate]() {
            return sprintBuilder
                .withTimeSpan(DateTimeRange{upperDate, upperDate})
                .build();
        });
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 7, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate + Days(1)};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });

    taskStorage->save(someTask);
    sprintStorage->save(sprints);

    const auto distribution =
        mondayFirstWeeklyDistributionReader->sprintDistribution(
            DateRange{lowerDate.date(), upperDate.date()});
    ASSERT_EQ(expected, distribution);
}

TEST_F(QtStorageTestFixture,
       retrieves_weekly_distribution_with_sunday_first_setting)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    const auto sundayFirstWeeklyDistributionReader =
        initializer.factory.weeklyDistReader(dw::Weekday::Sunday);
    using namespace dw;
    const Task someTask{TaskBuilder{}.build()};
    SprintBuilder sprintBuilder{SprintBuilder{}.withTaskUuid(someTask.uuid())};
    const dw::DateTime upperDate{Date{Year{2016}, Month{2}, Day{12}}};
    const dw::DateTime lowerDate{upperDate - Weeks(11)};
    const std::vector<int> expected{3, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 6};
    std::vector<Sprint> sprints;
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &lowerDate]() {
            const dw::DateTime timestamp{lowerDate - Days(1)};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // On lower border
    std::generate_n(
        std::back_inserter(sprints), 3, [&sprintBuilder, &lowerDate]() {
            return sprintBuilder
                .withTimeSpan(DateTimeRange{lowerDate, lowerDate})
                .build();
        });
    // Week in 2015
    std::generate_n(
        std::back_inserter(sprints), 4, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate +
                                         Days(-6 * 7 - 5)}; // Sunday
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // Week in 2016
    std::generate_n(
        std::back_inserter(sprints), 5, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate +
                                         Days(-6 * 7 + 1)}; // Saturday
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // On upper border
    std::generate_n(
        std::back_inserter(sprints), 6, [&sprintBuilder, &upperDate]() {
            return sprintBuilder
                .withTimeSpan(DateTimeRange{upperDate, upperDate})
                .build();
        });
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 7, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate + Days(1)};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });

    taskStorage->save(someTask);
    sprintStorage->save(sprints);

    const auto distribution =
        sundayFirstWeeklyDistributionReader->sprintDistribution(
            DateRange{lowerDate.date(), upperDate.date()});

    ASSERT_EQ(expected, distribution);
}

TEST_F(QtStorageTestFixture, stores_and_retrieves_work_schedule)
{
    auto scheduleStorage = initializer.factory.scheduleStorage();
    using namespace dw;
    using sprint_timer::WorkSchedule;
    WorkSchedule expected;
    expected.addWeekSchedule(Date{Year{2012}, Month{3}, Day{1}},
                             buildWeekSchedule({1, 1, 1, 1, 1, 0, 0}));
    expected.addWeekSchedule(Date{Year{2014}, Month{1}, Day{7}},
                             buildWeekSchedule({2, 2, 2, 2, 2, 0, 0}));
    expected.addWeekSchedule(Date{Year{2015}, Month{7}, Day{17}},
                             buildWeekSchedule({3, 3, 4, 3, 1, 7, 9}));
    expected.addWeekSchedule(Date{Year{2017}, Month{6}, Day{27}},
                             buildWeekSchedule({12, 12, 12, 12, 12, 0, 0}));
    expected.addWeekSchedule(Date{Year{2017}, Month{2}, Day{4}},
                             buildWeekSchedule({13, 13, 13, 13, 13, 0, 0}));
    expected.addWeekSchedule(Date{Year{2017}, Month{11}, Day{22}},
                             buildWeekSchedule({11, 11, 11, 11, 11, 11, 0}));
    expected.addWeekSchedule(Date{Year{2018}, Month{12}, Day{12}},
                             buildWeekSchedule({12, 12, 12, 12, 12, 0, 5}));
    expected.addExceptionalDay(Date{Year{2018}, Month{1}, Day{1}}, 0);
    expected.addExceptionalDay(Date{Year{2019}, Month{1}, Day{1}}, 0);
    expected.addExceptionalDay(Date{Year{2017}, Month{2}, Day{23}}, 12);
    expected.addExceptionalDay(Date{Year{2014}, Month{12}, Day{30}}, 12);

    scheduleStorage->updateSchedule(expected);
    const auto schedule = scheduleStorage->schedule();

    ASSERT_EQ(expected, schedule);
}
