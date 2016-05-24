#include "core/entities/Pomodoro.h"
#include "core/use_cases/use_cases.h"
#include "fixtures/FakePomodoroStorageWriter.h"
#include "fixtures/FakeTaskStorageWriter.h"
#include <TestHarness.h>

using namespace UseCases;

TEST_GROUP(RemovePomodoroUseCase)
{
    const TimeSpan defaultTimeSpan
        = TimeSpan{DateTime::currentDateTime(), DateTime::currentDateTime()};
    TodoItem someTask{"Task name", 4, 2, {"Tag1", "Tag2"}, false};
    Pomodoro somePomodoro{someTask.uuid(), defaultTimeSpan};
};

// TEST(RemovePomodoroUseCase, test_removes_pomodoro_and_decrements_spent)
// {
//     FakePomodoroWriter pomodoroWriter;
//     FakeTaskStorageWriter taskWriter;
//     taskWriter.save(someTask);
//     pomodoroWriter.save(somePomodoro);
//     const std::string taskUuid = someTask.uuid();
//     const std::string pomoUuid = somePomodoro.uuid();
//
//     CoreApi::removePomodoro(pomodoroWriter, taskWriter, somePomodoro);
//
//     CHECK_EQUAL(0, pomodoroWriter.storage.size());
//     CHECK_EQUAL(1,
//                 taskWriter.storage.getItem(taskUuid).value().spentPomodoros());
// }
