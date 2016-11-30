/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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
#include <TestHarness.h>

TEST_GROUP(TestCoreService)
{
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

    void setup()
    {
        sprintStorage.clear();
        taskStorage.clear();
        sprintBuffer.clear();
        taskBuffer.clear();
        sprintDistributionHandlerCalled = false;
        yearRangeHandlerCalled = false;
        tagHandlerCalled = false;
    }

    void assertStorageReturnedCorrectSprints(const std::vector<Sprint>& result)
    {
        CHECK_EQUAL(sprintBuffer.size(), result.size())
        std::vector<Sprint> sprintOrdByDate(result.cbegin(), result.cend());
        std::sort(sprintOrdByDate.begin(), sprintOrdByDate.end(),
                [](const auto& lhs, const auto& rhs) {
                    return lhs.startTime() < rhs.startTime();
                });
        CHECK(std::equal(sprintBuffer.cbegin(),
                         sprintBuffer.cend(),
                         sprintOrdByDate.cbegin(),
                         [](const auto& lhs, const auto& rhs) {
                             std::cout << lhs.uuid() << " == " << rhs.uuid() << std::endl;
                             return lhs.uuid() == rhs.uuid();
                         }));
    }

    void assertStorageReturnedCorrectTask(const std::vector<Task>& tasks)
    {
        CHECK_EQUAL(taskBuffer.size(), tasks.size());
        auto sortByUuid = [](const auto& lhs, const auto& rhs) {
            return lhs.uuid() < rhs.uuid();
        };
        auto taskEqualByUuid = [](const auto& lhs, const auto& rhs) {
            return lhs.uuid() == rhs.uuid();
        };
        std::vector<Task> tasksByUuid(tasks.cbegin(), tasks.cend());
        std::sort(tasksByUuid.begin(), tasksByUuid.end(), sortByUuid);
        std::sort(taskBuffer.begin(), taskBuffer.end(), sortByUuid);
        CHECK(std::equal(taskBuffer.cbegin(),
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
};

TEST(TestCoreService,
     test_registers_sprint_and_increments_tasks_sprints)
{
    taskStorageWriter.save(defaultTask);
    const std::string taskUuid = defaultTask.uuid();

    coreService.registerSprint(defaultTimeSpan, taskUuid);

    CHECK_EQUAL(1, sprintStorage.size());
    CHECK_EQUAL(3, (*taskStorage.getItem(taskUuid)).actualCost());
}

TEST(TestCoreService,
     test_removes_sprint_and_decrements_tasks_sprint)
{
    taskStorageWriter.save(defaultTask);
    const std::string taskUuid = defaultTask.uuid();
    Sprint sprint{taskUuid, defaultTimeSpan};
    sprintStorageWriter.save(sprint);

    coreService.removeSprint(sprint);

    CHECK_EQUAL(0, sprintStorage.size());
    CHECK_EQUAL(1, (*taskStorage.getItem(taskUuid)).actualCost());
}

TEST(TestCoreService, test_request_sprints_in_time_range)
{
    std::string taskUuid{"1234"};
    SprintBuilder builder;
    builder.withTaskUuid(taskUuid);
    std::vector<Sprint> sprints;
    DateTime dt = DateTime::fromYMD(2016, 5, 1);
    for (int i = 0; i < 10; ++i) {
        TimeSpan timeSpan{dt.addDays(i), dt.addDays(i)};
        auto sprint = builder.withTimeSpan(timeSpan).build();
        sprints.push_back(sprint);
        sprintStorage.store(sprint);
    }
    // Expected result
    sprintBuffer.push_back(sprints[2]);
    sprintBuffer.push_back(sprints[3]);
    sprintBuffer.push_back(sprints[4]);
    TimeSpan timeRange{dt.addDays(2), dt.addDays(4)};

    coreService.sprintsInTimeRange(
            timeRange, [this](const std::vector<Sprint>& result) {
                assertStorageReturnedCorrectSprints(result);
            });
}

TEST(TestCoreService, test_request_year_range_calls_handler)
{
    coreService.yearRange(
            [this](const std::vector<std::string>& result) {
                yearRangeHandler(result);
            });

    CHECK(yearRangeHandlerCalled);
}

TEST(TestCoreService, test_request_daily_distribution_calls_handler)
{
    coreService.requestSprintDailyDistribution(
            defaultTimeSpan, [this](const Distribution<int>& result) {
                sprintDistributionHandler(result);
            });
    CHECK(sprintDistributionHandlerCalled);
}

TEST(TestCoreService, test_request_weekly_distribution_calls_handler)
{
    coreService.requestSprintWeeklyDistribution(
            defaultTimeSpan, [this](const Distribution<int>& result) {
                sprintDistributionHandler(result);
            });
    CHECK(sprintDistributionHandlerCalled);
}

TEST(TestCoreService, test_request_monthly_distribution_calls_handler)
{
    coreService.requestSprintMonthlyDistribution(
            defaultTimeSpan, [this](const Distribution<int>& result) {
                sprintDistributionHandler(result);
            });
    CHECK(sprintDistributionHandlerCalled);
}

TEST(TestCoreService, test_register_task)
{
    coreService.registerTask(defaultTask);

    CHECK_EQUAL(1, taskStorage.size());
}

TEST(TestCoreService, test_undo_functions_properly)
{
    coreService.registerTask(defaultTask);
    const std::string taskUuid = defaultTask.uuid();
    coreService.registerSprint(defaultTimeSpan, taskUuid);
    coreService.registerSprint(defaultTimeSpan, taskUuid);
    coreService.registerSprint(defaultTimeSpan, taskUuid);

    CHECK_EQUAL(1, taskStorage.size());
    CHECK_EQUAL(3, sprintStorage.size());

    coreService.undoLast();
    CHECK_EQUAL(2, sprintStorage.size());

    coreService.undoLast();
    CHECK_EQUAL(1, sprintStorage.size());

    coreService.undoLast();
    CHECK_EQUAL(0, sprintStorage.size());

    coreService.undoLast();
    CHECK_EQUAL(0, taskStorage.size());
}

TEST(TestCoreService, test_edit_task_should_only_alter_allowed_parameters)
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

    CHECK(defaultTask.uuid() == actual.uuid());
    CHECK(editedTaskName == actual.name());
    editedTags.sort();
    auto actualTags = actual.tags();
    actualTags.sort();
    CHECK(editedTags == actualTags);
    CHECK(editedTaskLastModified == actual.lastModified());
    // Uuid, actualCost and completion status should not be editable
    CHECK_EQUAL(editedEstimated, actual.estimatedCost());
    CHECK_EQUAL(defaultTask.actualCost(), actual.actualCost());
    CHECK_EQUAL(defaultTask.isCompleted(), actual.isCompleted());

    coreService.undoLast();

    Task& afterUndo = taskStorage.itemRef(defaultTask.uuid());

    CHECK(defaultTask.uuid() == afterUndo.uuid());
    CHECK(defaultTask.name() == afterUndo.name());
    CHECK(defaultTask.tags() == afterUndo.tags());
    CHECK_EQUAL(defaultTask.estimatedCost(),
                afterUndo.estimatedCost());
    CHECK_EQUAL(defaultTask.actualCost(), afterUndo.actualCost());
    CHECK_EQUAL(defaultTask.isCompleted(), afterUndo.isCompleted());
    // Timestamp of modification should not be changed when edition
    // cancelled
    CHECK(defaultTask.lastModified() == afterUndo.lastModified());
}

TEST(TestCoreService, test_request_finished_and_unfinished_tasks)
{
    TaskBuilder builder;
    DateTime dt = DateTime::fromYMD(2016, 11, 1);
    TimeSpan timeSpan{dt.addDays(1), dt.addDays(3)};
    std::vector<Task> expectedFinishedTasks;
    std::vector<Task> expectedUnfinishedTasks;
    // Create 5 finished and 5 unfinished tasks.
    for (int i = 0; i < 5; ++i) {
        Task finishedTask = builder
                .withLastModificationStamp(dt.addDays(i))
                .withCompletionStatus(true)
                .build();
        std::cout << dt.addDays(i) << " But get " << finishedTask.lastModified() << std::endl;
        Task unfinishedTask = builder
                .withLastModificationStamp(dt.addDays(i))
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

TEST(TestCoreService, test_toggle_task_competion_status)
{
    coreService.registerTask(defaultTask);
    const std::string taskUuid = defaultTask.uuid();

    coreService.toggleTaskCompletionStatus(defaultTask);
    CHECK(taskStorage.itemRef(taskUuid).isCompleted());
    CHECK(taskStorage.itemRef(taskUuid).lastModified()
          == DateTime::currentDateTimeLocal());

    // Should revert time stamp of last modification when
    // undoing
    coreService.undoLast();
    CHECK(!taskStorage.itemRef(taskUuid).isCompleted());
    CHECK(taskStorage.itemRef(taskUuid).lastModified()
          == defaultTask.lastModified());
}

TEST(TestCoreService, test_request_all_tags_calls_handler)
{
    coreService.requestAllTags(
        [this](const std::vector<std::string>& tags) { tagHandler(tags); });
    CHECK(tagHandlerCalled);
}

TEST(TestCoreService, test_edit_tag_should_edit_tag_for_all_items)
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

    CHECK_EQUAL(exp_tags1.size(), tags_act1.size())
    CHECK(std::equal(exp_tags1.begin(), exp_tags1.end(), tags_act1.begin()));
    CHECK_EQUAL(exp_tags2.size(), tags_act2.size())
    CHECK(std::equal(exp_tags2.begin(), exp_tags2.end(), tags_act2.begin()));
    CHECK_EQUAL(exp_tags3.size(), tags_act3.size())
    CHECK(std::equal(exp_tags3.begin(), exp_tags3.end(), tags_act3.begin()));

    // Undo command should revert tag back for all items
    coreService.undoLast();

    auto tags_act_un1 = taskStorage.itemRef(item1.uuid()).tags();
    auto tags_act_un2 = taskStorage.itemRef(item2.uuid()).tags();
    auto tags_act_un3 = taskStorage.itemRef(item3.uuid()).tags();
    tags_act_un1.sort();
    tags_act_un2.sort();
    tags_act_un3.sort();

    CHECK_EQUAL(tags1.size(), tags_act_un1.size())
    CHECK(std::equal(tags1.begin(), tags1.end(), tags_act_un1.begin()));
    CHECK_EQUAL(tags2.size(), tags_act_un2.size())
    CHECK(std::equal(tags2.begin(), tags2.end(), tags_act_un2.begin()));
    CHECK_EQUAL(tags3.size(), tags_act_un3.size())
    CHECK(std::equal(tags3.begin(), tags3.end(), tags_act_un3.begin()));
}
