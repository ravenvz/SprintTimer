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
#include "QtStorageInitializer.h"
#include "gtest/gtest.h"
#include <core/SprintBuilder.h>
#include <core/TaskBuilder.h>

using namespace sprint_timer::storage::qt_storage_impl;
using sprint_timer::ISprintStorageReader;
using sprint_timer::ITaskStorageReader;
using sprint_timer::SprintBuilder;
using sprint_timer::TaskBuilder;
using sprint_timer::entities::Sprint;
using sprint_timer::entities::Task;

class QtStorageImplementIntegrationTestFixture : public ::testing::Test {
public:
    QtStorageInitializer initializer;
};

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
    initializer.taskWriter->remove(someTask);

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
              .withLastModificationStamp(dw::DateTime::currentDateTime())
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
            .withLastModificationStamp(
                dw::DateTime::currentDateTime().add(dw::DateTime::Days(-2)))
            .build());
    finishedTasks.push_back(
        builder.withCompletionStatus(true)
            .withLastModificationStamp(
                dw::DateTime::currentDateTime().add(dw::DateTime::Days(-2)))
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
            .withLastModificationStamp(
                dw::DateTime::currentDateTime().add(dw::DateTime::Days(-1)))
            .build());
    finishedTasks.push_back(
        builder.withCompletionStatus(true)
            .withLastModificationStamp(
                dw::DateTime::currentDateTime().add(dw::DateTime::Days(-1)))
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
              .withLastModificationStamp(
                  dw::DateTime::currentDateTime().add(dw::DateTime::Days(-3)))
              .withCompletionStatus(true)
              .build();
    const Task expectedEditedTask
        = TaskBuilder{}
              .withUuid(someTask.uuid())
              .withName("I'm edited")
              .withTag("EditedTag")
              .withEstimatedCost(42)
              .withActualCost(someTask.actualCost())
              .withLastModificationStamp(dw::DateTime::currentDateTime())
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
    const dw::TimeSpan timeSpan{dw::DateTime::currentDateTime(),
                                dw::DateTime::currentDateTime()};

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(sprintBuilder.withTaskUuid(someTask.uuid())
                                       .withTimeSpan(timeSpan)
                                       .build());
    initializer.taskWriter->remove(someTask);

    initializer.sprintReader->sprintsForTask(
        someTask.uuid(), [this](const ISprintStorageReader::Items& items) {
            EXPECT_TRUE(items.empty());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       requests_tasks_in_given_time_span)
{
    TaskBuilder builder;
    const dw::TimeSpan targetRange{dw::DateTime::fromYMD(2018, 10, 10),
                                   dw::DateTime::fromYMD(2018, 10, 15)};
    const Task taskOutOfRangeLeft
        = builder
              .withLastModificationStamp(
                  targetRange.start().add(dw::DateTime::Days(-1)))
              .build();
    const Task taskLeftRangeBorder
        = builder.withLastModificationStamp(targetRange.start()).build();
    const Task insideRangeTask1
        = builder
              .withLastModificationStamp(
                  targetRange.start().add(dw::DateTime::Days(1)))
              .build();
    const Task insideRangeTask2
        = builder
              .withLastModificationStamp(
                  targetRange.finish().add(dw::DateTime::Days(-1)))
              .build();
    const Task taskRightRangeBorder
        = builder.withLastModificationStamp(targetRange.finish()).build();
    const Task taskOutOfRangeRight
        = builder
              .withLastModificationStamp(
                  targetRange.finish().add(dw::DateTime::Days(1)))
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
        targetRange, [this, &expected](const ITaskStorageReader::Items& items) {
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
    const dw::TimeSpan timeSpan{dw::DateTime::currentDateTime(),
                                dw::DateTime::currentDateTime()};
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
    const dw::TimeSpan timeSpan{dw::DateTime::currentDateTime(),
                                dw::DateTime::currentDateTime()};
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
    initializer.taskWriter->remove(someTask);

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
    initializer.taskWriter->remove(someTask);

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
    const dw::TimeSpan timeSpan{dw::DateTime::currentDateTime(),
                                dw::DateTime::currentDateTime()};
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
    using dw::DateTime;
    using dw::TimeSpan;
    SprintBuilder sprintBuilder;
    const dw::DateTime timestamp{dw::DateTime::currentDateTime()};
    const Task someTask = TaskBuilder{}.build();
    const dw::TimeSpan range{timestamp.add(dw::DateTime::Days(-5)),
                             timestamp.add(dw::DateTime::Days(5))};
    const Sprint outOfRangeLeft{
        sprintBuilder.withTaskUuid(someTask.uuid())
            .withTimeSpan(dw::TimeSpan{timestamp.add(dw::DateTime::Days(-20)),
                                       timestamp.add(dw::DateTime::Days(-20))})
            .build()};
    const Sprint onLeftBorder{
        sprintBuilder.withTaskUuid(someTask.uuid())
            .withTimeSpan(dw::TimeSpan{range.start(), range.start()})
            .build()};
    const Sprint inRange{sprintBuilder.withTaskUuid(someTask.uuid())
                             .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                             .build()};
    const Sprint onRightBorder{
        sprintBuilder.withTaskUuid(someTask.uuid())
            .withTimeSpan(dw::TimeSpan{range.finish(), range.finish()})
            .build()};
    const Sprint outOfRangeRight{
        sprintBuilder.withTaskUuid(someTask.uuid())
            .withTimeSpan(
                dw::TimeSpan{range.finish().add(dw::DateTime::Days(1)),
                             range.finish().add(dw::DateTime::Days(1))})
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
    const Task someTask{TaskBuilder{}.build()};
    const dw::DateTime timestamp{dw::DateTime::fromYMD(2018, 12, 1)};
    const Sprint left{SprintBuilder{}
                          .withTaskUuid(someTask.uuid())
                          .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                          .build()};
    const Sprint right{
        SprintBuilder{}
            .withTaskUuid(someTask.uuid())
            .withTimeSpan(dw::TimeSpan{timestamp.add(dw::DateTime::Years(-4)),
                                       timestamp.add(dw::DateTime::Years(-4))})
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

TEST_F(QtStorageImplementIntegrationTestFixture, retrieves_sprint_distributions)
{
    // FAIL();
}

TEST_F(QtStorageImplementIntegrationTestFixture, saves_sprints_in_bulk)
{
    const Task someTask{TaskBuilder{}.build()};
    const dw::TimeSpan timeSpan{dw::DateTime::fromYMD(2018, 12, 1),
                                dw::DateTime::fromYMD(2018, 12, 1)};
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
    const Task someTask{TaskBuilder{}.build()};
    const Task otherTask{TaskBuilder{}.build()};
    const dw::TimeSpan timeSpan{dw::DateTime::fromYMD(2018, 12, 1),
                                dw::DateTime::fromYMD(2018, 12, 1)};
    std::vector<Sprint> sprintBulk;
    SprintBuilder sprintBuilder{SprintBuilder{}.withTimeSpan(timeSpan)};
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
        timeSpan, [this](const ISprintStorageReader::Items& items) {
            EXPECT_TRUE(items.empty());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       reads_sprint_daily_distribution)
{
    const Task someTask{TaskBuilder{}.build()};
    SprintBuilder sprintBuilder{SprintBuilder{}.withTaskUuid(someTask.uuid())};
    const dw::DateTime someDate{dw::DateTime::fromYMD(2018, 12, 2)};
    const dw::TimeSpan range{someDate.add(dw::DateTime::Days(-29)), someDate};
    std::vector<Sprint> sprints;
    std::vector<int> expected{4, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2};

    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate.add(dw::DateTime::Days(-30))};
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });
    std::generate_n(
        std::back_inserter(sprints), 4, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate.add(dw::DateTime::Days(-29))};
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });
    std::generate_n(
        std::back_inserter(sprints), 3, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate.add(dw::DateTime::Days(-20))};
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });
    std::generate_n(
        std::back_inserter(sprints), 7, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate.add(dw::DateTime::Days(-10))};
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &someDate]() {
            return sprintBuilder.withTimeSpan(dw::TimeSpan{someDate, someDate})
                .build();
        });
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 3, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate.add(dw::DateTime::Days(1))};
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
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
    const Task someTask{TaskBuilder{}.build()};
    SprintBuilder sprintBuilder{SprintBuilder{}.withTaskUuid(someTask.uuid())};
    const dw::DateTime someDate{dw::DateTime::fromYMD(2018, 12, 2)};
    dw::DateTime lowerDate{someDate.add(dw::DateTime::Months(-11))};
    lowerDate = lowerDate.add(dw::DateTime::Days{
        -static_cast<int>(std::min(lowerDate.day(), someDate.day())) + 1});
    const dw::TimeSpan range{lowerDate, someDate};
    const std::vector<int> expected{3, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 5};
    std::vector<Sprint> sprints;
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &someDate]() {
            dw::DateTime timestamp{someDate.add(dw::DateTime::Months(-11))};
            timestamp = timestamp.add(dw::DateTime::Days(
                -static_cast<int>(std::min(timestamp.day(), someDate.day()))));
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });
    // On border
    std::generate_n(
        std::back_inserter(sprints), 3, [&sprintBuilder, &someDate]() {
            dw::DateTime timestamp{someDate.add(dw::DateTime::Months(-11))};
            timestamp = timestamp.add(dw::DateTime::Days(
                -static_cast<int>(std::min(timestamp.day(), someDate.day()))
                + 1));
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });
    // Somewhere in the middle
    std::generate_n(
        std::back_inserter(sprints), 4, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{
                someDate.add(dw::DateTime::Months(-5))};
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });
    // On border
    std::generate_n(
        std::back_inserter(sprints), 5, [&sprintBuilder, &someDate]() {
            return sprintBuilder.withTimeSpan(dw::TimeSpan{someDate, someDate})
                .build();
        });
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 6, [&sprintBuilder, &someDate]() {
            dw::DateTime timestamp{someDate.add(dw::DateTime::Months(1))};
            timestamp = timestamp.add(dw::DateTime::Days(
                -static_cast<int>(std::min(timestamp.day(), someDate.day()))
                + 1));
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(sprints);

    initializer.monthlyDistributionReader->requestDistribution(
        range,
        [&expected, this](const sprint_timer::Distribution<int>& distribution) {
            EXPECT_EQ(expected, distribution.getDistributionVector());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       retrieves_sprint_weekly_distribution_with_monday_first_setting)
{
    const Task someTask{TaskBuilder{}.build()};
    SprintBuilder sprintBuilder{SprintBuilder{}.withTaskUuid(someTask.uuid())};
    const dw::DateTime upperDate{dw::DateTime::fromYMD(2016, 2, 12)};
    const dw::DateTime lowerDate{upperDate.add(dw::DateTime::Days(-11 * 7))};
    const dw::TimeSpan range{lowerDate, upperDate};
    const std::vector<int> expected{3, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 6};
    std::vector<Sprint> sprints;
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &lowerDate]() {
            const dw::DateTime timestamp{lowerDate.add(dw::DateTime::Days(-1))};
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });
    // On lower border
    std::generate_n(
        std::back_inserter(sprints), 3, [&sprintBuilder, &lowerDate]() {
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{lowerDate, lowerDate})
                .build();
        });
    // Week 53 in 2015
    std::generate_n(
        std::back_inserter(sprints), 4, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{
                upperDate.add(dw::DateTime::Days(-6 * 7 - 4))}; // Monday
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });
    // Week 53 in 2016
    std::generate_n(
        std::back_inserter(sprints), 5, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{
                upperDate.add(dw::DateTime::Days(-6 * 7 + 2))}; // Sunday
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });
    // On upper border
    std::generate_n(
        std::back_inserter(sprints), 6, [&sprintBuilder, &upperDate]() {
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{upperDate, upperDate})
                .build();
        });
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 7, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate.add(dw::DateTime::Days(1))};
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(sprints);

    initializer.mondayFirstWeeklyDistributionReader->requestDistribution(
        range,
        [&expected, this](const sprint_timer::Distribution<int>& distribution) {
            EXPECT_EQ(expected, distribution.getDistributionVector());
            initializer.quit();
        });
    initializer.runEventLoop();
}

TEST_F(QtStorageImplementIntegrationTestFixture,
       retrieves_sprint_weekly_distribution_with_sunday_first_setting)
{
    const Task someTask{TaskBuilder{}.build()};
    SprintBuilder sprintBuilder{SprintBuilder{}.withTaskUuid(someTask.uuid())};
    const dw::DateTime upperDate{dw::DateTime::fromYMD(2016, 2, 12)};
    const dw::DateTime lowerDate{upperDate.add(dw::DateTime::Days(-11 * 7))};
    const dw::TimeSpan range{lowerDate, upperDate};
    const std::vector<int> expected{3, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 6};
    std::vector<Sprint> sprints;
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &lowerDate]() {
            const dw::DateTime timestamp{lowerDate.add(dw::DateTime::Days(-1))};
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });
    // On lower border
    std::generate_n(
        std::back_inserter(sprints), 3, [&sprintBuilder, &lowerDate]() {
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{lowerDate, lowerDate})
                .build();
        });
    // Week in 2015
    std::generate_n(
        std::back_inserter(sprints), 4, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{
                upperDate.add(dw::DateTime::Days(-6 * 7 - 5))}; // Sunday
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });
    // Week in 2016
    std::generate_n(
        std::back_inserter(sprints), 5, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{
                upperDate.add(dw::DateTime::Days(-6 * 7 + 1))}; // Saturday
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });
    // On upper border
    std::generate_n(
        std::back_inserter(sprints), 6, [&sprintBuilder, &upperDate]() {
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{upperDate, upperDate})
                .build();
        });
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 7, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate.add(dw::DateTime::Days(1))};
            return sprintBuilder
                .withTimeSpan(dw::TimeSpan{timestamp, timestamp})
                .build();
        });

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(sprints);

    initializer.sundayFirstWeeklyDistributionReader->requestDistribution(
        range,
        [&expected, this](const sprint_timer::Distribution<int>& distribution) {
            EXPECT_EQ(expected, distribution.getDistributionVector());
            initializer.quit();
        });
    initializer.runEventLoop();
}
