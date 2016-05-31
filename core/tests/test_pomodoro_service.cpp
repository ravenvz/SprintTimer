#include "core/PomodoroService.h"
#include "fixtures/FakePomodoroDistributionReader.h"
#include "fixtures/FakePomodoroStorageReader.h"
#include "fixtures/FakePomodoroStorageWriter.h"
#include "fixtures/FakePomodoroYearRangeReader.h"
#include "fixtures/FakeTaskStorageReader.h"
#include "fixtures/FakeTaskStorageWriter.h"
#include "use_cases/AddPomodoroTransaction.h"
#include <TestHarness.h>
#include <algorithm>
#include <vector>


TEST_GROUP(TestPomodoroService)
{
    const TimeSpan defaultTimeSpan
        = TimeSpan{DateTime::currentDateTime(), DateTime::currentDateTime()};
    TodoItem defaultTask{"Task name",
                         4,
                         2,
                         "550e8400-e29b-41d4-a716-446655440000",
                         {"Tag1", "Tag2"},
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

    void setup()
    {
        pomodoroStorage.clear();
        taskStorage.clear();
        pomodoroHandlerCalled = false;
        pomoDistributionHandlerCalled = false;
        pomodoroYearRangeHandlerCalled = false;
        taskHandlerCalled = false;
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
    std::list<std::string> editedTags{"Tag2", "New Tag"};
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

TEST(TestPomodoroService, test_request_tasks_calls_handler)
{
    pomodoroService.requestTasks(
        defaultTimeSpan,
        [this](const std::vector<TodoItem>& result) { taskHandler(result); });
    CHECK(taskHandlerCalled);
}
