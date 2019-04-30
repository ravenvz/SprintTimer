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
#include <core/WorkdayTracker.h>

using namespace sprint_timer::storage::qt_storage_impl;
using sprint_timer::ISprintStorageReader;
using sprint_timer::ITaskStorageReader;
using sprint_timer::SprintBuilder;
using sprint_timer::TaskBuilder;
using sprint_timer::entities::Sprint;
using sprint_timer::entities::Task;


sprint_timer::WeekSchedule buildSchedule(const std::array<int, 7>& raw_schedule)
{
    sprint_timer::WeekSchedule schedule;
    for (size_t i = 0; i < raw_schedule.size(); ++i)
        schedule.setTargetGoal(static_cast<dw::Weekday>(i), raw_schedule[i]);
    return schedule;
}


class QtStorageImplementIntegrationTestFixture : public ::testing::Test {
public:
    QtStorageInitializer initializer;
};

// TODO split into tests for specific readers/writers

TEST_F(QtStorageImplementIntegrationTestFixture, saves_task)
{
    const Task someTask = TaskBuilder{}.withName("Some name").build();

    initializer.taskWriter->save(someTask);

    initializer.taskReader->requestUnfinishedTasks(
        [& expected = someTask, this](const ITaskStorageReader::Items& items) {
            EXPECT_EQ(1, items.size());
            EXPECT_EQ(expected, items.front());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture, remove_task_with_no_sprints)
{
    const Task someTask = TaskBuilder{}.withUuid("123").build();

    initializer.taskWriter->save(someTask);
    initializer.taskWriter->remove(someTask.uuid());

    initializer.taskReader->requestUnfinishedTasks(
        [this](const ITaskStorageReader::Items& items) {
            EXPECT_EQ(0, items.size());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture, toggles_task_completion_status)
{
    const Task someTask
        = TaskBuilder{}
              .withUuid("123")
              .withCompletionStatus(true)
              .withLastModificationStamp(dw::current_date_time())
              .build();

    initializer.taskWriter->save(someTask);
    initializer.taskWriter->toggleTaskCompletionStatus(someTask.uuid(),
                                                       someTask.lastModified());

    initializer.taskReader->requestUnfinishedTasks(
        [this, &someTask](const ITaskStorageReader::Items& items) {
            EXPECT_EQ(1, items.size());
            EXPECT_FALSE(someTask.isCompleted() == items.front().isCompleted());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture, retrieves_unfinished_tasks)
{
    ITaskStorageReader::Items unfinishedTasks;
    ITaskStorageReader::Items finishedTasks;
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
        initializer.taskWriter->save(task);
    for (const auto& task : finishedTasks)
        initializer.taskWriter->save(task);

    initializer.taskReader->requestUnfinishedTasks(
        [&unfinishedTasks, this](const ITaskStorageReader::Items& items) {
            EXPECT_EQ(unfinishedTasks, items);
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       considers_recently_completed_tasks_as_unfinished)
{
    ITaskStorageReader::Items unfinishedTasks;
    ITaskStorageReader::Items finishedTasks;
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
    ITaskStorageReader::Items recentTasks = unfinishedTasks;
    std::copy(cbegin(finishedTasks),
              cend(finishedTasks),
              std::back_inserter(recentTasks));

    for (const auto& task : unfinishedTasks)
        initializer.taskWriter->save(task);
    for (const auto& task : finishedTasks)
        initializer.taskWriter->save(task);

    initializer.taskReader->requestUnfinishedTasks(
        [&recentTasks, this](const ITaskStorageReader::Items& items) {
            EXPECT_EQ(recentTasks, items);
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture, edits_task)
{
    const Task someTask
        = TaskBuilder{}
              .withName("Edit me")
              .withTag("Tag")
              .withEstimatedCost(4)
              .withActualCost(2)
              .withLastModificationStamp(dw::current_date_time() - dw::Days{3})
              .withCompletionStatus(true)
              .build();
    const Task expectedEditedTask
        = TaskBuilder{}
              .withUuid(someTask.uuid())
              .withName("I'm edited")
              .withTag("EditedTag")
              .withEstimatedCost(42)
              .withActualCost(someTask.actualCost())
              .withLastModificationStamp(dw::current_date_time())
              .withCompletionStatus(someTask.isCompleted())
              .build();

    initializer.taskWriter->save(someTask);
    initializer.taskWriter->edit(someTask, expectedEditedTask);

    initializer.taskReader->requestUnfinishedTasks(
        [this, &expectedEditedTask](const ITaskStorageReader::Items& items) {
            EXPECT_EQ(1, items.size());
            EXPECT_EQ(expectedEditedTask, items.front());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture, edits_tag)
{
    const std::string tagToEdit{"TagToEdit"};
    const std::string editedTag{"EditedTag"};
    const Task someTask = TaskBuilder{}.withTag(tagToEdit).build();

    initializer.taskWriter->save(someTask);
    initializer.taskWriter->editTag(tagToEdit, editedTag);

    initializer.taskReader->requestAllTags(
        [this, &editedTag](const std::vector<std::string>& tags) {
            EXPECT_EQ(1, tags.size());
            EXPECT_EQ(editedTag, tags.front());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture, updates_priorities)
{
    TaskBuilder builder;
    ITaskStorageReader::Items tasks;
    const std::vector<std::string> priorities{"3", "2", "1"};

    for (int i = 0; i < 3; ++i) {
        tasks.push_back(builder.withUuid(std::to_string(i + 1)).build());
        initializer.taskWriter->save(tasks.back());
    }
    initializer.taskWriter->updatePriorities(priorities);

    initializer.taskReader->requestUnfinishedTasks(
        [this, &priorities](const ITaskStorageReader::Items& items) {
            EXPECT_TRUE(std::equal(cbegin(priorities),
                                   cend(priorities),
                                   cbegin(items),
                                   cend(items),
                                   [](const auto& lhs, const auto& rhs) {
                                       return lhs == rhs.uuid();
                                   }));
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       remove_task_with_sprints_should_delete_sprints)
{
    const Task someTask = TaskBuilder{}.build();
    SprintBuilder sprintBuilder;
    const dw::DateTimeRange timeSpan{dw::current_date_time(),
                                     dw::current_date_time()};

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(sprintBuilder.withTaskUuid(someTask.uuid())
                                       .withTimeSpan(timeSpan)
                                       .build());
    initializer.taskWriter->remove(someTask.uuid());

    initializer.sprintReader->sprintsForTask(
        someTask.uuid(), [this](const ISprintStorageReader::Items& items) {
            EXPECT_TRUE(items.empty());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       requests_tasks_in_given_date_range)
{
    using namespace dw;
    const DateRange targetDateRange{Date{Year{2018}, Month{10}, Day{10}},
                                    Date{Year{2018}, Month{10}, Day{15}}};
    const DateTime leftmostDateTime{DateTime{targetDateRange.start()}};
    const DateTime rightmostDateTime{DateTime{targetDateRange.finish()}};
    TaskBuilder builder;
    const Task taskOutOfRangeLeft
        = builder.withLastModificationStamp(leftmostDateTime - Days{1}).build();
    const Task taskLeftRangeBorder
        = builder.withLastModificationStamp(leftmostDateTime).build();
    const Task insideRangeTask1
        = builder.withLastModificationStamp(leftmostDateTime + Days{1}).build();
    const Task insideRangeTask2
        = builder.withLastModificationStamp(rightmostDateTime - Days{1})
              .build();
    const Task taskRightRangeBorder
        = builder.withLastModificationStamp(rightmostDateTime).build();
    const Task taskOutOfRangeRight
        = builder.withLastModificationStamp(rightmostDateTime + Days{1})
              .build();
    const ITaskStorageReader::Items expected{taskLeftRangeBorder,
                                             insideRangeTask1,
                                             insideRangeTask2,
                                             taskRightRangeBorder};

    initializer.taskWriter->save(taskOutOfRangeLeft);
    initializer.taskWriter->save(taskLeftRangeBorder);
    initializer.taskWriter->save(insideRangeTask1);
    initializer.taskWriter->save(insideRangeTask2);
    initializer.taskWriter->save(taskRightRangeBorder);
    initializer.taskWriter->save(taskOutOfRangeRight);

    initializer.taskReader->requestTasks(
        targetDateRange,
        [this, &expected](const ITaskStorageReader::Items& items) {
            EXPECT_EQ(expected.size(), items.size());
            EXPECT_EQ(expected, items);
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       inserting_sprint_increments_associated_task_actual_count)
{
    const Task someTask = TaskBuilder{}.build();
    const Task expectedTask
        = TaskBuilder{}
              .withUuid(someTask.uuid())
              .withActualCost(2)
              .withLastModificationStamp(someTask.lastModified())
              .build();
    const dw::DateTimeRange timeSpan{dw::current_date_time(),
                                     dw::current_date_time()};
    SprintBuilder builder;

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(
        builder.forTask(someTask).withTimeSpan(timeSpan).build());
    initializer.sprintWriter->save(
        builder.forTask(someTask).withTimeSpan(timeSpan).build());

    initializer.taskReader->requestUnfinishedTasks(
        [this, &expectedTask](const ITaskStorageReader::Items& items) {
            EXPECT_EQ(1, items.size());
            EXPECT_EQ(expectedTask, items.front());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       removing_sprint_decrements_associated_task_actual_count)
{
    const Task someTask = TaskBuilder{}.build();
    const Task expectedTask
        = TaskBuilder{}
              .withUuid(someTask.uuid())
              .withActualCost(1)
              .withLastModificationStamp(someTask.lastModified())
              .build();
    const dw::DateTimeRange timeSpan{dw::current_date_time(),
                                     dw::current_date_time()};
    SprintBuilder builder;
    const Sprint sprint
        = builder.forTask(someTask).withTimeSpan(timeSpan).build();

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(sprint);
    initializer.sprintWriter->save(
        builder.forTask(someTask).withTimeSpan(timeSpan).build());
    initializer.sprintWriter->remove(sprint);

    initializer.taskReader->requestUnfinishedTasks(
        [this, &expectedTask](const ITaskStorageReader::Items& items) {
            EXPECT_EQ(1, items.size());
            EXPECT_EQ(expectedTask, items.front());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture, retrieves_tags)
{
    using sprint_timer::entities::Tag;
    TaskBuilder builder;

    initializer.taskWriter->save(
        builder.withExplicitTags({Tag{"Tag1"}, Tag{"Tag2"}}).build());
    initializer.taskWriter->save(
        builder.withExplicitTags({Tag{"Tag3"}, Tag{"Tag4"}}).build());
    const std::vector<std::string> expected{"Tag1", "Tag2", "Tag3", "Tag4"};

    initializer.taskReader->requestAllTags(
        [this, &expected](const std::vector<std::string>& tags) {
            auto sortedTags = tags;
            std::sort(begin(sortedTags), end(sortedTags));
            EXPECT_EQ(expected, sortedTags);
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       removing_task_does_not_remove_tag_if_some_other_task_has_it)
{
    TaskBuilder builder;
    const std::string someTag{"Tag"};
    const Task someTask = builder.withTag(someTag).build();
    const Task someOtherTask = builder.withTag(someTag).build();

    initializer.taskWriter->save(someTask);
    initializer.taskWriter->save(someOtherTask);
    initializer.taskWriter->remove(someTask.uuid());

    initializer.taskReader->requestAllTags(
        [this, &someTag](const std::vector<std::string>& tags) {
            EXPECT_EQ(1, tags.size());
            EXPECT_EQ(someTag, tags.front());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       removes_orphaned_tags_when_deleting_task)
{
    using sprint_timer::entities::Tag;
    const Task someTask
        = TaskBuilder{}.withExplicitTags({Tag{"Tag1"}, Tag{"Tag2"}}).build();

    initializer.taskWriter->save(someTask);
    initializer.taskWriter->remove(someTask.uuid());

    initializer.taskReader->requestAllTags(
        [this](const std::vector<std::string>& tags) {
            EXPECT_TRUE(tags.empty());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       retrieves_sprints_for_given_task)
{
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

    initializer.taskWriter->save(someTask);
    initializer.taskWriter->save(irrelevantTask);
    initializer.sprintWriter->save(sprints);

    initializer.sprintReader->sprintsForTask(
        someTask.uuid(),
        [this, &expectedSprints](const ISprintStorageReader::Items& items) {
            EXPECT_EQ(2, items.size());
            EXPECT_EQ(expectedSprints, items);
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       retrieves_sprints_in_given_timespan)
{
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

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(outOfRangeLeft);
    initializer.sprintWriter->save(onLeftBorder);
    initializer.sprintWriter->save(inRange);
    initializer.sprintWriter->save(onRightBorder);
    initializer.sprintWriter->save(outOfRangeRight);

    initializer.sprintReader->requestItems(
        range, [this, &expected](const ISprintStorageReader::Items& items) {
            EXPECT_EQ(3, items.size());
            EXPECT_EQ(expected, items);
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture, retrieves_year_range)
{
    using namespace dw;
    const Task someTask{TaskBuilder{}.build()};
    const dw::DateTime timestamp{
        dw::DateTime{Date{Year{2018}, Month{12}, Day{1}}}};
    const Sprint left{SprintBuilder{}
                          .withTaskUuid(someTask.uuid())
                          .withTimeSpan(DateTimeRange{timestamp, timestamp})
                          .build()};
    const Sprint right{SprintBuilder{}
                           .withTaskUuid(someTask.uuid())
                           .withTimeSpan(add_offset(
                               DateTimeRange{timestamp, timestamp}, -Years{4}))
                           .build()};
    const std::vector<std::string> expected{"2014", "2018"};

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(left);
    initializer.sprintWriter->save(right);

    initializer.yearRangeReader->requestYearRange(
        [this, &expected](const std::vector<std::string>& yearRange) {
            EXPECT_EQ(2, yearRange.size());
            EXPECT_EQ(expected, yearRange);
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture, saves_sprints_in_bulk)
{
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

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(sprintBulk);

    initializer.sprintReader->sprintsForTask(
        someTask.uuid(),
        [&sprintBulk, this](const ISprintStorageReader::Items& items) {
            EXPECT_EQ(sprintBulk, items);
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture, removes_sprints_in_bulk)
{
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

    initializer.taskWriter->save(someTask);
    initializer.taskWriter->save(otherTask);
    initializer.sprintWriter->save(sprintBulk);
    initializer.sprintWriter->remove(sprintBulk);

    initializer.sprintReader->requestItems(
        dateRange, [this](const ISprintStorageReader::Items& items) {
            EXPECT_TRUE(items.empty());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       reads_sprint_daily_distribution)
{
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

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(sprints);

    initializer.dailyDistributionReader->requestDistribution(
        range,
        [&expected, this](const sprint_timer::Distribution<int>& distribution) {
            EXPECT_EQ(expected, distribution.getDistributionVector());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture, retrieves_monthly_distribution)
{
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

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(sprints);

    initializer.monthlyDistributionReader->requestDistribution(
        DateRange{lowerDate, someDate},
        [&expected, this](const sprint_timer::Distribution<int>& distribution) {
            EXPECT_EQ(expected, distribution.getDistributionVector());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       retrieves_sprint_weekly_distribution_with_monday_first_setting)
{
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
            const dw::DateTime timestamp{upperDate
                                         + Days(-6 * 7 - 4)}; // Monday
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // Week 53 in 2016
    std::generate_n(
        std::back_inserter(sprints), 5, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate
                                         + Days(-6 * 7 + 2)}; // Sunday
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

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(sprints);

    initializer.mondayFirstWeeklyDistributionReader->requestDistribution(
        DateRange{lowerDate.date(), upperDate.date()},
        [&expected, this](const sprint_timer::Distribution<int>& distribution) {
            EXPECT_EQ(expected, distribution.getDistributionVector());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       retrieves_sprint_weekly_distribution_with_sunday_first_setting)
{
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
            const dw::DateTime timestamp{upperDate
                                         + Days(-6 * 7 - 5)}; // Sunday
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // Week in 2016
    std::generate_n(
        std::back_inserter(sprints), 5, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate
                                         + Days(-6 * 7 + 1)}; // Saturday
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

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(sprints);

    initializer.sundayFirstWeeklyDistributionReader->requestDistribution(
        DateRange{lowerDate.date(), upperDate.date()},
        [&expected, this](const sprint_timer::Distribution<int>& distribution) {
            EXPECT_EQ(expected, distribution.getDistributionVector());
            initializer.quit();
        });
    initializer.runEventLoop();
}

// TEST_F(QtStorageImplementIntegrationTestFixture,
//        extra_day_data_store_and_retrieve)
// {
//     using namespace dw;
//     using namespace sprint_timer;
//     using sprint_timer::utils::WeekdaySelection;
//     WorkdayTracker expected{WeekdaySelection{15}}; // Mon - Thu
//     const std::vector<Date> extraHolidays{Date{Year{2019}, Month{5}, Day{1}},
//                                           Date{Year{2019}, Month{5}, Day{2}},
//                                           Date{Year{2019}, Month{5},
//                                           Day{3}}};
//     const std::vector<Date> extraWorkdays{Date{Year{2019}, Month{5}, Day{4}},
//                                           Date{Year{2019}, Month{5},
//                                           Day{4}}};
//     for (const auto& day : extraHolidays)
//         expected.addExtraHoliday(day);
//     for (const auto& day : extraWorkdays)
//         expected.addExtraWorkday(day);
//
//     initializer.extraDaysWriter->changeWorkingDays(expected);
//
//     initializer.extraDaysReader->requestData(
//         [&expected, this](const WorkdayTracker& actual) {
//             EXPECT_EQ(expected, actual);
//             initializer.quit();
//         });
//     initializer.runEventLoop();
// }

TEST_F(QtStorageImplementIntegrationTestFixture,
       change_working_days_store_and_retrieve)
{
    using namespace dw;
    using sprint_timer::WorkdayTracker;
    WorkdayTracker expected;
    expected.addWeekSchedule(Date{Year{2012}, Month{3}, Day{1}},
                             buildSchedule({1, 1, 1, 1, 1, 0, 0}));
    expected.addWeekSchedule(Date{Year{2014}, Month{1}, Day{7}},
                             buildSchedule({2, 2, 2, 2, 2, 0, 0}));
    expected.addWeekSchedule(Date{Year{2015}, Month{7}, Day{17}},
                             buildSchedule({3, 3, 4, 3, 1, 7, 9}));
    expected.addWeekSchedule(Date{Year{2017}, Month{6}, Day{27}},
                             buildSchedule({12, 12, 12, 12, 12, 0, 0}));
    expected.addWeekSchedule(Date{Year{2017}, Month{2}, Day{4}},
                             buildSchedule({13, 13, 13, 13, 13, 0, 0}));
    expected.addWeekSchedule(Date{Year{2017}, Month{11}, Day{22}},
                             buildSchedule({11, 11, 11, 11, 11, 11, 0}));
    expected.addWeekSchedule(Date{Year{2018}, Month{12}, Day{12}},
                             buildSchedule({12, 12, 12, 12, 12, 0, 5}));
    expected.addExtraHoliday(Date{Year{2018}, Month{1}, Day{1}});
    expected.addExtraHoliday(Date{Year{2019}, Month{1}, Day{1}});
    expected.addExtraWorkday(Date{Year{2017}, Month{2}, Day{23}}, 12);
    expected.addExtraWorkday(Date{Year{2014}, Month{12}, Day{30}}, 12);

    initializer.workingDaysWriter->changeWorkingDays(expected);

    initializer.workingDaysReader->requestData(
        [&expected, this](const WorkdayTracker& actual) {
            EXPECT_EQ(expected, actual);
            // std::this_thread::sleep_for(std::chrono::seconds{4});
            initializer.quit();
        });
    initializer.runEventLoop();
}
