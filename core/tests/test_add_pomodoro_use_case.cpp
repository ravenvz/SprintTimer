#include "fixtures/FakePomodoroStorageReader.h"
#include "fixtures/FakePomodoroStorageWriter.h"
#include "fixtures/FakeTaskStorageWriter.h"
#include "use_cases/AddPomodoroTransaction.h"
#include <TestHarness.h>
#include <algorithm>
#include <vector>


// using namespace UseCases;


TEST_GROUP(AddPomodoroUseCase)
{
    const TimeSpan defaultTimeSpan
        = TimeSpan{DateTime::currentDateTime(), DateTime::currentDateTime()};
    TodoItem defaultTask{"Task name", 4, 2, {"Tag1", "Tag2"}, false};
};

TEST(AddPomodoroUseCase, test_adds_pomodoro_and_increments_spent)
{
    FakeStorage<Pomodoro> pomodoroStorage;
    FakePomodoroWriter pomodoroWriter{pomodoroStorage};
    FakePomodoroStorageReader pomodoroReader{pomodoroStorage};

    // FakePomodoroWriter pomodoroWriter;
    // FakeTaskStorageWriter taskWriter;
    // taskWriter.save(defaultTask);
    // const std::string taskUuid = defaultTask.uuid();
    //
    // CoreApi::addPomodoro(pomodoroWriter, taskWriter, defaultTimeSpan,
    // taskUuid);
    //
    // CHECK_EQUAL(1, pomodoroWriter.storage.size());
    // CHECK_EQUAL(3,
    //             taskWriter.storage.getItem(taskUuid).value().spentPomodoros());
}
