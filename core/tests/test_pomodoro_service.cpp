#include "core/PomodoroService.h"
#include "fixtures/FakePomodoroDistributionReader.h"
#include "fixtures/FakePomodoroStorageReader.h"
#include "fixtures/FakePomodoroStorageWriter.h"
#include "fixtures/FakePomodoroYearRangeReader.h"
#include "fixtures/FakeTaskStorageReader.h"
#include "fixtures/FakeTaskStorageWriter.h"
#include "use_cases/RegisterNewPomodoro.h"
#include <TestHarness.h>
#include <algorithm>
#include <vector>

#include <iostream>
#include <iterator>

TEST_GROUP(TestPomodoroService)
{
    const TimeSpan defaultTimeSpan
        = TimeSpan{DateTime::currentDateTime(), DateTime::currentDateTime()};
    TodoItem defaultTask{"Task name",
                         4,
                         2,
                         "550e8400-e29b-41d4-a716-446655440000",
                         {Tag{"Tag1"}, Tag{"Tag2"}},
                         false,
                         DateTime::fromYMD(2015, 11, 10)};

    FakeStorage<Pomodoro> pomodoroStorage;
    FakeStorage<TodoItem> taskStorage;
    FakePomodoroWriter pomodoroStorageWriter{pomodoroStorage};
    FakePomodoroStorageReader pomodoroStorageReader{pomodoroStorage};
    FakePomodoroDistributionReader pomodoroDistributionReader{pomodoroStorage};
    FakePomodoroYearRangeReader pomodoroYearRangeReader;
    FakeTaskStorageReader taskStorageReader{taskStorage};
    FakeTaskStorageWriter taskStorageWriter{taskStorage};

    CoreApi::PomodoroService pomodoroService{pomodoroStorageReader,
                                             pomodoroStorageWriter,
                                             pomodoroYearRangeReader,
                                             taskStorageReader,
                                             taskStorageWriter,
                                             pomodoroDistributionReader,
                                             pomodoroDistributionReader,
                                             pomodoroDistributionReader};

    bool pomodoroHandlerCalled{false};
    bool pomoDistributionHandlerCalled{false};
    bool pomodoroYearRangeHandlerCalled{false};
    bool taskHandlerCalled{false};
    bool tagHandlerCalled{false};

    void setup()
    {
        pomodoroStorage.clear();
        taskStorage.clear();
        pomodoroHandlerCalled = false;
        pomoDistributionHandlerCalled = false;
        pomodoroYearRangeHandlerCalled = false;
        taskHandlerCalled = false;
        tagHandlerCalled = false;
    }

    void pomodoroTimeRangeHandler(const std::vector<Pomodoro>& result)
    {
        pomodoroHandlerCalled = true;
    }

    void pomodoroYearRangeHandler(const std::vector<std::string>& result)
    {
        pomodoroYearRangeHandlerCalled = true;
    }

    void pomodoroDistributionHandler(const Distribution<int>& result)
    {
        pomoDistributionHandlerCalled = true;
    }

    void taskHandler(const std::vector<TodoItem>& result)
    {
        taskHandlerCalled = true;
    }

    void tagHandler(const std::vector<std::string>& tags)
    {
        tagHandlerCalled = true;
    }
};

TEST(TestPomodoroService,
     test_register_pomodoro_adds_pomodoro_and_increments_spent)
{
    taskStorageWriter.save(defaultTask);
    const std::string taskUuid = defaultTask.uuid();

    pomodoroService.registerPomodoro(defaultTimeSpan, taskUuid);

    CHECK_EQUAL(1, pomodoroStorage.size());
    CHECK_EQUAL(3, taskStorage.getItem(taskUuid).value().spentPomodoros());
}

TEST(TestPomodoroService,
     test_remove_pomodoro_removes_pomodoro_and_decrements_spent)
{
    taskStorageWriter.save(defaultTask);
    const std::string taskUuid = defaultTask.uuid();
    Pomodoro pomodoro{taskUuid, defaultTimeSpan};
    pomodoroStorageWriter.save(pomodoro);

    pomodoroService.removePomodoro(pomodoro);

    CHECK_EQUAL(0, pomodoroStorage.size());
    CHECK_EQUAL(1, taskStorage.getItem(taskUuid).value().spentPomodoros());
}

TEST(TestPomodoroService, test_request_pomodoros_in_time_range_calls_handler)
{
    pomodoroService.pomodorosInTimeRange(
        defaultTimeSpan, [this](const std::vector<Pomodoro>& result) {
            pomodoroTimeRangeHandler(result);
        });

    CHECK(pomodoroHandlerCalled);
}

TEST(TestPomodoroService, test_request_pomodoro_year_range_calls_handler)
{
    pomodoroService.pomodoroYearRange(
        [this](const std::vector<std::string>& result) {
            pomodoroYearRangeHandler(result);
        });

    CHECK(pomodoroYearRangeHandlerCalled);
}

TEST(TestPomodoroService, test_request_daily_distribution_calls_handler)
{
    pomodoroService.requestPomodoroDailyDistribution(
        defaultTimeSpan, [this](const Distribution<int>& result) {
            pomodoroDistributionHandler(result);
        });
    CHECK(pomoDistributionHandlerCalled);
}

TEST(TestPomodoroService, test_request_weekly_distribution_calls_handler)
{
    pomodoroService.requestPomodoroWeeklyDistribution(
        defaultTimeSpan, [this](const Distribution<int>& result) {
            pomodoroDistributionHandler(result);
        });
    CHECK(pomoDistributionHandlerCalled);
}

TEST(TestPomodoroService, test_request_monthly_distribution_calls_handler)
{
    pomodoroService.requestPomodoroMonthlyDistribution(
        defaultTimeSpan, [this](const Distribution<int>& result) {
            pomodoroDistributionHandler(result);
        });
    CHECK(pomoDistributionHandlerCalled);
}

TEST(TestPomodoroService, test_register_task)
{
    pomodoroService.registerTask(defaultTask);

    CHECK_EQUAL(1, taskStorage.size());
}

TEST(TestPomodoroService, test_undo_functions_properly)
{
    pomodoroService.registerTask(defaultTask);
    const std::string taskUuid = defaultTask.uuid();
    pomodoroService.registerPomodoro(defaultTimeSpan, taskUuid);
    pomodoroService.registerPomodoro(defaultTimeSpan, taskUuid);
    pomodoroService.registerPomodoro(defaultTimeSpan, taskUuid);

    CHECK_EQUAL(1, taskStorage.size());
    CHECK_EQUAL(3, pomodoroStorage.size());

    pomodoroService.undoLast();
    CHECK_EQUAL(2, pomodoroStorage.size());

    pomodoroService.undoLast();
    CHECK_EQUAL(1, pomodoroStorage.size());

    pomodoroService.undoLast();
    CHECK_EQUAL(0, pomodoroStorage.size());

    pomodoroService.undoLast();
    CHECK_EQUAL(0, taskStorage.size());
}

TEST(TestPomodoroService, test_edit_task_should_only_alter_allowed_parameters)
{
    pomodoroService.registerTask(defaultTask);
    const std::string taskUuid = defaultTask.uuid();

    const std::string editedTaskName{"Edited"};
    std::list<Tag> editedTags{Tag{"Tag2"}, Tag{"New Tag"}};
    const int editedEstimated{7};
    const int editedSpent{5};
    const bool editedCompletionStatus{true};

    TodoItem editedTask{editedTaskName,
                        editedEstimated,
                        editedSpent,
                        editedTags,
                        editedCompletionStatus};

    const DateTime editedTaskLastModified = editedTask.lastModified();

    pomodoroService.editTask(defaultTask, editedTask);
    TodoItem& actual = taskStorage.itemRef(taskUuid);

    // TODO write method to compare Tasks

    CHECK(taskUuid == actual.uuid());
    CHECK(editedTaskName == actual.name());
    editedTags.sort();
    auto actualTags = actual.tags();
    actualTags.sort();
    CHECK(editedTags == actualTags);
    CHECK(editedTaskLastModified == actual.lastModified());

    // Uuid, spentPomodoros and completion status should not be editable
    CHECK_EQUAL(editedEstimated, actual.estimatedPomodoros());
    CHECK_EQUAL(defaultTask.spentPomodoros(), actual.spentPomodoros());
    CHECK_EQUAL(defaultTask.isCompleted(), actual.isCompleted());

    pomodoroService.undoLast();

    TodoItem& afterUndo = taskStorage.itemRef(taskUuid);

    CHECK(taskUuid == afterUndo.uuid());
    CHECK(defaultTask.name() == afterUndo.name());
    CHECK(defaultTask.tags() == afterUndo.tags());
    CHECK_EQUAL(defaultTask.estimatedPomodoros(),
                afterUndo.estimatedPomodoros());
    CHECK_EQUAL(defaultTask.spentPomodoros(), afterUndo.spentPomodoros());
    CHECK_EQUAL(defaultTask.isCompleted(), afterUndo.isCompleted());
    // Timestamp of modification should not be changed when edition
    // cancelled
    CHECK(defaultTask.lastModified() == afterUndo.lastModified());
}

TEST(TestPomodoroService, test_request_finished_tasks_calls_handler)
{
    pomodoroService.requestFinishedTasks(
        defaultTimeSpan,
        [this](const std::vector<TodoItem>& result) { taskHandler(result); });
    CHECK(taskHandlerCalled);
}

TEST(TestPomodoroService, test_request_unfinished_tasks_calls_handler)
{
    pomodoroService.requestUnfinishedTasks(
        [this](const std::vector<TodoItem>& result) { taskHandler(result); });
    CHECK(taskHandlerCalled);
}

TEST(TestPomodoroService, test_toggle_task_competion_status)
{
    pomodoroService.registerTask(defaultTask);
    const std::string taskUuid = defaultTask.uuid();

    pomodoroService.toggleTaskCompletionStatus(defaultTask);
    CHECK(taskStorage.itemRef(taskUuid).isCompleted());
    CHECK(taskStorage.itemRef(taskUuid).lastModified()
          == DateTime::currentDateTimeLocal());

    // Should revert time stamp of last modification when
    // undoing
    pomodoroService.undoLast();
    CHECK(!taskStorage.itemRef(taskUuid).isCompleted());
    CHECK(taskStorage.itemRef(taskUuid).lastModified()
          == defaultTask.lastModified());
}

TEST(TestPomodoroService, test_request_all_tags_calls_handler)
{
    pomodoroService.requestAllTags(
        [this](const std::vector<std::string>& tags) { tagHandler(tags); });
    CHECK(tagHandlerCalled);
}

TEST(TestPomodoroService, test_edit_tag_should_edit_tag_for_all_items)
{
    const std::list<Tag> tags1{Tag{"Tag1"}, Tag{"Tag2"}};
    const std::list<Tag> tags2{Tag{"Tag2"}, Tag{"Tag3"}};
    const std::list<Tag> tags3{Tag{"Tag1"}, Tag{"Tag5"}};
    TodoItem item1{"Item 1", 4, 0, tags1, false};
    TodoItem item2{"Item 2", 4, 0, tags2, false};
    TodoItem item3{"Item 3", 4, 0, tags3, false};
    const std::list<Tag> exp_tags1{Tag{"EditedTag"}, Tag{"Tag2"}};
    const std::list<Tag> exp_tags2{Tag{"Tag2"}, Tag{"Tag3"}};
    const std::list<Tag> exp_tags3{Tag{"EditedTag"}, Tag{"Tag5"}};
    pomodoroService.registerTask(item1);
    pomodoroService.registerTask(item2);
    pomodoroService.registerTask(item3);

    pomodoroService.editTag("Tag1", "EditedTag");
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
    pomodoroService.undoLast();
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
