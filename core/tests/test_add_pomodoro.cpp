#include "core/use_cases/AddPomodoroTransaction.h"
#include "fixtures/FakePomodoroStorage.h"
#include "fixtures/FakePomodoroWriter.h"
#include <algorithm>
#include <vector>
#include <TestHarness.h>


using namespace UseCases;


TEST_GROUP(TestAddPomodoroTransaction)
{
    const TimeSpan defaultTimeSpan
        = TimeSpan{DateTime::currentDateTime(), DateTime::currentDateTime()};
};

TEST(TestAddPomodoroTransaction,
     test_undo_transaction_that_has_not_been_executed_should_return_false)
{
    FakePomodoroWriter writer;
    Pomodoro pomodoro{defaultTimeSpan};
    AddPomodoroTransaction add_pomodoro{writer, pomodoro};

    CHECK(!add_pomodoro.undo());
}

TEST(TestAddPomodoroTransaction, test_execute_and_undo)
{
    FakePomodoroWriter writer;
    Pomodoro pomodoro{defaultTimeSpan};
    AddPomodoroTransaction add_pomodoro{writer, pomodoro};

    CHECK(add_pomodoro.execute());
    CHECK_EQUAL(1, writer.storage.size());
    CHECK(add_pomodoro.undo());
    CHECK_EQUAL(0, writer.storage.size());
}

