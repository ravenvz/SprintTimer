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

    bool sprintHandlerCalled{false};
    bool sprintDistributionHandlerCalled{false};
    bool yearRangeHandlerCalled{false};
    bool taskHandlerCalled{false};
    bool tagHandlerCalled{false};

    void setup()
    {
        sprintStorage.clear();
        taskStorage.clear();
        sprintHandlerCalled = false;
        sprintDistributionHandlerCalled = false;
        yearRangeHandlerCalled = false;
        taskHandlerCalled = false;
        tagHandlerCalled = false;
    }

    void sprintTimeRangeHandler(const std::vector<Sprint>& result)
    {
        sprintHandlerCalled = true;
    }

    void yearRangeHandler(const std::vector<std::string>& result)
    {
        yearRangeHandlerCalled = true;
    }

    void sprintDistributionHandler(const Distribution<int>& result)
    {
        sprintDistributionHandlerCalled = true;
    }

    void taskHandler(const std::vector<Task>& result)
    {
        taskHandlerCalled = true;
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
    CHECK_EQUAL(3, taskStorage.getItem(taskUuid).value().actualCost());
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
    CHECK_EQUAL(1, taskStorage.getItem(taskUuid).value().actualCost());
}

TEST(TestCoreService, test_request_sprints_in_time_range_calls_handler)
{
    coreService.sprintsInTimeRange(
            defaultTimeSpan, [this](const std::vector<Sprint>& result) {
                sprintTimeRangeHandler(result);
            });

    CHECK(sprintHandlerCalled);
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
    const std::string taskUuid = defaultTask.uuid();

    const std::string editedTaskName{"Edited"};
    std::list<Tag> editedTags{Tag{"Tag2"}, Tag{"New Tag"}};
    const int editedEstimated{7};
    const int editedSpent{5};
    const bool editedCompletionStatus{true};

    Task editedTask{editedTaskName,
                        editedEstimated,
                        editedSpent,
                        editedTags,
                        editedCompletionStatus};

    const DateTime editedTaskLastModified = editedTask.lastModified();

    coreService.editTask(defaultTask, editedTask);
    Task& actual = taskStorage.itemRef(taskUuid);

    // TODO write method to compare Tasks

    CHECK(taskUuid == actual.uuid());
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

    Task& afterUndo = taskStorage.itemRef(taskUuid);

    CHECK(taskUuid == afterUndo.uuid());
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

TEST(TestCoreService, test_request_finished_tasks_calls_handler)
{
    coreService.requestFinishedTasks(
        defaultTimeSpan,
        [this](const std::vector<Task>& result) { taskHandler(result); });
    CHECK(taskHandlerCalled);
}

TEST(TestCoreService, test_request_unfinished_tasks_calls_handler)
{
    coreService.requestUnfinishedTasks(
        [this](const std::vector<Task>& result) { taskHandler(result); });
    CHECK(taskHandlerCalled);
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
    const std::list<Tag> tags1{Tag{"Tag1"}, Tag{"Tag2"}};
    const std::list<Tag> tags2{Tag{"Tag2"}, Tag{"Tag3"}};
    const std::list<Tag> tags3{Tag{"Tag1"}, Tag{"Tag5"}};
    Task item1{"Item 1", 4, 0, tags1, false};
    Task item2{"Item 2", 4, 0, tags2, false};
    Task item3{"Item 3", 4, 0, tags3, false};
    const std::list<Tag> exp_tags1{Tag{"EditedTag"}, Tag{"Tag2"}};
    const std::list<Tag> exp_tags2{Tag{"Tag2"}, Tag{"Tag3"}};
    const std::list<Tag> exp_tags3{Tag{"EditedTag"}, Tag{"Tag5"}};
    coreService.registerTask(item1);
    coreService.registerTask(item2);
    coreService.registerTask(item3);

    coreService.editTag("Tag1", "EditedTag");
    auto item_act1 = taskStorage.itemRef(item1.uuid());
    auto item_act2 = taskStorage.itemRef(item2.uuid());
    auto item_act3 = taskStorage.itemRef(item3.uuid());
    auto tags_act1 = item_act1.tags();
    auto tags_act2 = item_act2.tags();
    auto tags_act3 = item_act3.tags();
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
    auto item_act_un1 = taskStorage.itemRef(item1.uuid());
    auto item_act_un2 = taskStorage.itemRef(item2.uuid());
    auto item_act_un3 = taskStorage.itemRef(item3.uuid());
    auto tags_act_un1 = item_act_un1.tags();
    auto tags_act_un2 = item_act_un2.tags();
    auto tags_act_un3 = item_act_un3.tags();
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
