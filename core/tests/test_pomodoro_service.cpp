#include "core/PomodoroService.h"
#include "fixtures/FakePomodoroDistributionReader.h"
#include "fixtures/FakePomodoroStorageReader.h"
#include "fixtures/FakePomodoroStorageWriter.h"
#include "fixtures/FakePomodoroYearRangeReader.h"
#include "fixtures/FakeTaskStorageWriter.h"
#include "use_cases/AddPomodoroTransaction.h"
#include <TestHarness.h>
#include <algorithm>
#include <vector>


TEST_GROUP(TestPomodoroService)
{
    const TimeSpan defaultTimeSpan
        = TimeSpan{DateTime::currentDateTime(), DateTime::currentDateTime()};
    TodoItem defaultTask{"Task name", 4, 2, {"Tag1", "Tag2"}, false};

    FakeStorage<Pomodoro> pomodoroStorage;
    FakeStorage<TodoItem> taskStorage;
    FakePomodoroWriter pomodoroStorageWriter{pomodoroStorage};
    FakePomodoroStorageReader pomodoroStorageReader{pomodoroStorage};
    FakePomodoroDistributionReader pomodoroDistributionReader{pomodoroStorage};
    FakePomodoroYearRangeReader pomodoroYearRangeReader;
    FakeTaskStorageWriter taskStorageWriter{taskStorage};

    CoreApi::PomodoroService pomodoroService{pomodoroStorageReader,
                                             pomodoroStorageWriter,
                                             pomodoroYearRangeReader,
                                             taskStorageWriter,
                                             pomodoroDistributionReader,
                                             pomodoroDistributionReader,
                                             pomodoroDistributionReader};

    bool pomodoroHandlerCalled{false};
    bool pomoDistributionHandlerCalled{false};
    bool pomodoroYearRangeHandlerCalled{false};

    void setup()
    {
        pomodoroStorage.clear();
        taskStorage.clear();
        pomodoroHandlerCalled = false;
        pomoDistributionHandlerCalled = false;
        pomodoroYearRangeHandlerCalled = false;
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

    // TODO uncomment when Task save/remove is implemented
    pomodoroService.undoLast();
    CHECK_EQUAL(0, taskStorage.size());
}
