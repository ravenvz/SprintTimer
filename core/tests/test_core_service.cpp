/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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
#include "core/CoreService.h"
#include "fixtures/FakeSprintDistributionReader.h"
#include "fixtures/FakeSprintStorageReader.h"
#include "fixtures/FakeYearRangeReader.h"
#include "fixtures/FakeTaskStorageReader.h"
#include "fixtures/FakeTaskStorageWriter.h"
#include "fixtures/FakeSprintStorageWriter.h"
#include "core/TaskBuilder.h"
#include "core/SprintBuilder.h"
#include "gtest/gtest.h"

using dw::DateTime;

// TODO this mess should be cleaned

class CoreServiceFixture : public ::testing::Test {
public:
    const TimeSpan defaultTimeSpan
            = TimeSpan{DateTime::currentDateTime(), DateTime::currentDateTime()};

    Task defaultTask{"Task name",
                     4,
                     2,
                     "550e8400-e29b-41d4-a716-446655440000",
                     {Tag{"Tag1"}, Tag{"Tag2"}},
                     false,
                     DateTime::fromYMD(2015, 11, 10)};

    FakeStorage<Sprint> sprintStorage;
    FakeStorage<Task> taskStorage;
    FakeSprintWriter sprintStorageWriter{sprintStorage};
    FakeSprintStorageReader sprintStorageReader{sprintStorage};
    FakeSprintDistributionReader sprintDistributionReader{sprintStorage};
    FakeYearRangeReader yearRangeReader;
    FakeTaskStorageReader taskStorageReader{taskStorage};
    FakeTaskStorageWriter taskStorageWriter{taskStorage};

    Core::CoreService coreService{sprintStorageReader,
                                  sprintStorageWriter,
                                  yearRangeReader,
                                  taskStorageReader,
                                  taskStorageWriter,
                                  sprintDistributionReader,
                                  sprintDistributionReader,
                                  sprintDistributionReader};

    bool sprintDistributionHandlerCalled{false};
    bool yearRangeHandlerCalled{false};
    bool tagHandlerCalled{false};
    std::vector<Sprint> sprintBuffer;
    std::vector<Task> taskBuffer;

    void assertStorageReturnedCorrectSprints(const std::vector<Sprint>& result)
    {
        EXPECT_EQ(sprintBuffer.size(), result.size());
        std::vector<Sprint> sprintOrdByDate(result.cbegin(), result.cend());
        std::sort(sprintOrdByDate.begin(), sprintOrdByDate.end(),
                [](const auto& lhs, const auto& rhs) {
                    return lhs.startTime() < rhs.startTime();
                });
        EXPECT_TRUE(std::equal(sprintBuffer.cbegin(),
                    sprintBuffer.cend(),
                    sprintOrdByDate.cbegin(),
                    [](const auto& lhs, const auto& rhs) {
                        std::cout << lhs.uuid() << " == " << rhs.uuid() << std::endl;
                        return lhs.uuid() == rhs.uuid();
                    }));
    }

    void assertStorageReturnedCorrectTask(const std::vector<Task>& tasks)
    {
        EXPECT_EQ(taskBuffer.size(), tasks.size());
        auto sortByUuid = [](const auto& lhs, const auto& rhs) {
            return lhs.uuid() < rhs.uuid();
        };
        auto taskEqualByUuid = [](const auto& lhs, const auto& rhs) {
            return lhs.uuid() == rhs.uuid();
        };
        std::vector<Task> tasksByUuid(tasks.cbegin(), tasks.cend());
        std::sort(tasksByUuid.begin(), tasksByUuid.end(), sortByUuid);
        std::sort(taskBuffer.begin(), taskBuffer.end(), sortByUuid);
        EXPECT_TRUE(std::equal(taskBuffer.cbegin(),
                    taskBuffer.cend(),
                    tasksByUuid.cbegin(),
                    taskEqualByUuid));
    }

    void yearRangeHandler(const std::vector<std::string>& result)
    {
        yearRangeHandlerCalled = true;
    }

    void sprintDistributionHandler(const Distribution<int>& result)
    {
        sprintDistributionHandlerCalled = true;
    }

    void tagHandler(const std::vector<std::string>& tags)
    {
        tagHandlerCalled = true;
    }
protected:
    void SetUp() override
    {
        sprintStorage.clear();
        taskStorage.clear();
        sprintBuffer.clear();
        taskBuffer.clear();
        sprintDistributionHandlerCalled = false;
        yearRangeHandlerCalled = false;
        tagHandlerCalled = false;
    }
};

TEST_F(CoreServiceFixture,
       test_registers_sprint_and_increments_tasks_sprints)
{
    taskStorageWriter.save(defaultTask);
    const std::string taskUuid = defaultTask.uuid();

    coreService.registerSprint(defaultTimeSpan, taskUuid);

    EXPECT_EQ(1, sprintStorage.size());
    EXPECT_EQ(3, (*taskStorage.getItem(taskUuid)).actualCost());
}

TEST_F(CoreServiceFixture,
       test_removes_sprint_and_decrements_tasks_sprint)
{
    taskStorageWriter.save(defaultTask);
    const std::string taskUuid = defaultTask.uuid();
    Sprint sprint{taskUuid, defaultTimeSpan};
    sprintStorageWriter.save(sprint);

    coreService.removeSprint(sprint);

    EXPECT_EQ(0, sprintStorage.size());
    EXPECT_EQ(1, (*taskStorage.getItem(taskUuid)).actualCost());
}

TEST_F(CoreServiceFixture, test_request_sprints_in_time_range)
{
    std::string taskUuid{"1234"};
    SprintBuilder builder;
    builder.withTaskUuid(taskUuid);
    std::vector<Sprint> sprints;
    DateTime dt = DateTime::fromYMD(2016, 5, 1);
    for (int i = 0; i < 10; ++i) {
        TimeSpan timeSpan{dt.add(DateTime::Days{i}), dt.add(DateTime::Days{i})};
        auto sprint = builder.withTimeSpan(timeSpan).build();
        sprints.push_back(sprint);
        sprintStorage.store(sprint);
    }
    // Expected result
    sprintBuffer.push_back(sprints[2]);
    sprintBuffer.push_back(sprints[3]);
    sprintBuffer.push_back(sprints[4]);
    TimeSpan timeRange{dt.add(DateTime::Days{2}), dt.add(DateTime::Days{4})};

    coreService.sprintsInTimeRange(
            timeRange, [this](const std::vector<Sprint>& result) {
                assertStorageReturnedCorrectSprints(result);
            });
}

TEST_F(CoreServiceFixture, test_request_year_range_calls_handler)
{
    coreService.yearRange(
            [this](const std::vector<std::string>& result) {
                yearRangeHandler(result);
            });

    EXPECT_TRUE(yearRangeHandlerCalled);
}

TEST_F(CoreServiceFixture, test_request_daily_distribution_calls_handler)
{
    coreService.requestSprintDailyDistribution(
            defaultTimeSpan, [this](const Distribution<int>& result) {
                sprintDistributionHandler(result);
            });
    EXPECT_TRUE(sprintDistributionHandlerCalled);
}

TEST_F(CoreServiceFixture, test_request_weekly_distribution_calls_handler)
{
    coreService.requestSprintWeeklyDistribution(
            defaultTimeSpan, [this](const Distribution<int>& result) {
                sprintDistributionHandler(result);
            });
    EXPECT_TRUE(sprintDistributionHandlerCalled);
}

TEST_F(CoreServiceFixture, test_request_monthly_distribution_calls_handler)
{
    coreService.requestSprintMonthlyDistribution(
            defaultTimeSpan, [this](const Distribution<int>& result) {
                sprintDistributionHandler(result);
            });
    EXPECT_TRUE(sprintDistributionHandlerCalled);
}

TEST_F(CoreServiceFixture, test_register_task)
{
    coreService.registerTask(defaultTask);

    EXPECT_EQ(1, taskStorage.size());
}

TEST_F(CoreServiceFixture, test_undo_functions_properly)
{
    coreService.registerTask(defaultTask);
    const std::string taskUuid = defaultTask.uuid();
    coreService.registerSprint(defaultTimeSpan, taskUuid);
    coreService.registerSprint(defaultTimeSpan, taskUuid);
    coreService.registerSprint(defaultTimeSpan, taskUuid);

    EXPECT_EQ(1, taskStorage.size());
    EXPECT_EQ(3, sprintStorage.size());

    coreService.undoLast();
    EXPECT_EQ(2, sprintStorage.size());

    coreService.undoLast();
    EXPECT_EQ(1, sprintStorage.size());

    coreService.undoLast();
    EXPECT_EQ(0, sprintStorage.size());

    coreService.undoLast();
    EXPECT_EQ(0, taskStorage.size());
}

TEST_F(CoreServiceFixture, test_edit_task_should_only_alter_allowed_parameters)
{
    coreService.registerTask(defaultTask);
    const std::string editedTaskName{"Edited"};
    std::list<Tag> editedTags{Tag{"Tag2"}, Tag{"New Tag"}};
    const int editedEstimated{7};
    const int editedSpent{5};
    const bool editedCompletionStatus{true};
    Task editedTask = TaskBuilder{}
                      .withName(editedTaskName)
                      .withEstimatedCost(editedEstimated)
                      .withActualCost(editedSpent)
                      .withCompletionStatus(editedCompletionStatus)
                      .withExplicitTags(editedTags)
                      .build();
    const DateTime editedTaskLastModified = editedTask.lastModified();

    coreService.editTask(defaultTask, editedTask);
    Task& actual = taskStorage.itemRef(defaultTask.uuid());

    EXPECT_TRUE(defaultTask.uuid() == actual.uuid());
    EXPECT_TRUE(editedTaskName == actual.name());
    editedTags.sort();
    auto actualTags = actual.tags();
    actualTags.sort();
    EXPECT_TRUE(editedTags == actualTags);
    EXPECT_TRUE(editedTaskLastModified == actual.lastModified());
    // Uuid, actualCost and completion status should not be editable
    EXPECT_EQ(editedEstimated, actual.estimatedCost());
    EXPECT_EQ(defaultTask.actualCost(), actual.actualCost());
    EXPECT_EQ(defaultTask.isCompleted(), actual.isCompleted());

    coreService.undoLast();

    Task& afterUndo = taskStorage.itemRef(defaultTask.uuid());

    EXPECT_TRUE(defaultTask.uuid() == afterUndo.uuid());
    EXPECT_TRUE(defaultTask.name() == afterUndo.name());
    EXPECT_TRUE(defaultTask.tags() == afterUndo.tags());
    EXPECT_EQ(defaultTask.estimatedCost(),
                afterUndo.estimatedCost());
    EXPECT_EQ(defaultTask.actualCost(), afterUndo.actualCost());
    EXPECT_EQ(defaultTask.isCompleted(), afterUndo.isCompleted());
    // Timestamp of modification should not be changed when edition
    // cancelled
    EXPECT_TRUE(defaultTask.lastModified() == afterUndo.lastModified());
}

TEST_F(CoreServiceFixture, test_request_finished_and_unfinished_tasks)
{
    TaskBuilder builder;
    DateTime dt = DateTime::fromYMD(2016, 11, 1);
    TimeSpan timeSpan{dt.add(DateTime::Days{1}), dt.add(DateTime::Days{3})};
    std::vector<Task> expectedFinishedTasks;
    std::vector<Task> expectedUnfinishedTasks;
    // Create 5 finished and 5 unfinished tasks.
    for (int i = 0; i < 5; ++i) {
        Task finishedTask = builder
                .withLastModificationStamp(dt.add(DateTime::Days{i}))
                .withCompletionStatus(true)
                .build();
        Task unfinishedTask = builder
                .withLastModificationStamp(dt.add(DateTime::Days{i}))
                .withCompletionStatus(false)
                .build();
        taskStorage.store(finishedTask);
        taskStorage.store(unfinishedTask);
        // Push only these to expected buffer as they fit the timeSpan.
        if (1 <= i && i <=3)
            expectedFinishedTasks.push_back(finishedTask);
        expectedUnfinishedTasks.push_back(unfinishedTask);
    }

    taskBuffer = expectedFinishedTasks;
    coreService.requestFinishedTasks(
        timeSpan,
        [this](const std::vector<Task>& result) {
            assertStorageReturnedCorrectTask(result);
        });

    taskBuffer = expectedUnfinishedTasks;
    coreService.requestUnfinishedTasks(
        [this](const std::vector<Task>& result) {
            assertStorageReturnedCorrectTask(result);
        });
}

TEST_F(CoreServiceFixture, test_toggle_task_competion_status)
{
    coreService.registerTask(defaultTask);
    const std::string taskUuid = defaultTask.uuid();

    coreService.toggleTaskCompletionStatus(defaultTask);
    EXPECT_TRUE(taskStorage.itemRef(taskUuid).isCompleted());
    EXPECT_TRUE(taskStorage.itemRef(taskUuid).lastModified()
          == DateTime::currentDateTimeLocal());

    // Should revert time stamp of last modification when
    // undoing
    coreService.undoLast();
    EXPECT_TRUE(!taskStorage.itemRef(taskUuid).isCompleted());
    EXPECT_TRUE(taskStorage.itemRef(taskUuid).lastModified()
          == defaultTask.lastModified());
}

TEST_F(CoreServiceFixture, test_request_all_tags_calls_handler)
{
    coreService.requestAllTags(
        [this](const std::vector<std::string>& tags) { tagHandler(tags); });
    EXPECT_TRUE(tagHandlerCalled);
}

TEST_F(CoreServiceFixture, test_edit_tag_should_edit_tag_for_all_items)
{
    TaskBuilder taskBuilder;
    const std::list<Tag> tags1{Tag{"Tag1"}, Tag{"Tag2"}};
    const std::list<Tag> tags2{Tag{"Tag2"}, Tag{"Tag3"}};
    const std::list<Tag> tags3{Tag{"Tag1"}, Tag{"Tag5"}};
    auto item1 = taskBuilder.withExplicitTags(tags1).build();
    auto item2 = taskBuilder.withExplicitTags(tags2).build();
    auto item3 = taskBuilder.withExplicitTags(tags3).build();
    const std::list<Tag> exp_tags1{Tag{"EditedTag"}, Tag{"Tag2"}};
    const std::list<Tag> exp_tags2{Tag{"Tag2"}, Tag{"Tag3"}};
    const std::list<Tag> exp_tags3{Tag{"EditedTag"}, Tag{"Tag5"}};
    coreService.registerTask(item1);
    coreService.registerTask(item2);
    coreService.registerTask(item3);

    coreService.editTag("Tag1", "EditedTag");

    auto tags_act1 = taskStorage.itemRef(item1.uuid()).tags();
    auto tags_act2 = taskStorage.itemRef(item2.uuid()).tags();
    auto tags_act3 = taskStorage.itemRef(item3.uuid()).tags();
    tags_act1.sort();
    tags_act2.sort();
    tags_act3.sort();

    EXPECT_EQ(exp_tags1.size(), tags_act1.size());
    EXPECT_TRUE(std::equal(exp_tags1.begin(), exp_tags1.end(), tags_act1.begin()));
    EXPECT_EQ(exp_tags2.size(), tags_act2.size());
    EXPECT_TRUE(std::equal(exp_tags2.begin(), exp_tags2.end(), tags_act2.begin()));
    EXPECT_EQ(exp_tags3.size(), tags_act3.size());
    EXPECT_TRUE(std::equal(exp_tags3.begin(), exp_tags3.end(), tags_act3.begin()));

    // Undo command should revert tag back for all items
    coreService.undoLast();

    auto tags_act_un1 = taskStorage.itemRef(item1.uuid()).tags();
    auto tags_act_un2 = taskStorage.itemRef(item2.uuid()).tags();
    auto tags_act_un3 = taskStorage.itemRef(item3.uuid()).tags();
    tags_act_un1.sort();
    tags_act_un2.sort();
    tags_act_un3.sort();

    EXPECT_EQ(tags1.size(), tags_act_un1.size());
    EXPECT_TRUE(std::equal(tags1.begin(), tags1.end(), tags_act_un1.begin()));
    EXPECT_EQ(tags2.size(), tags_act_un2.size());
    EXPECT_TRUE(std::equal(tags2.begin(), tags2.end(), tags_act_un2.begin()));
    EXPECT_EQ(tags3.size(), tags_act_un3.size());
    EXPECT_TRUE(std::equal(tags3.begin(), tags3.end(), tags_act_un3.begin()));
}
