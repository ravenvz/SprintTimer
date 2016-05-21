#include "core/use_cases/RemovePomodoroTransaction.h"
#include "core/entities/Pomodoro.h"
#include "fixtures/FakePomodoroStorageWriter.h"
#include <TestHarness.h>

using namespace UseCases;

TEST_GROUP(RemovePomodoroTransaction)
{
    const TimeSpan defaultTimeSpan
        = TimeSpan{DateTime::currentDateTime(), DateTime::currentDateTime()};
};

TEST(RemovePomodoroTransaction,
     test_undo_should_not_add_pomodoro_if_it_was_not_removed_beforehand)
{
    FakePomodoroWriter writer;
    Pomodoro pomodoro{defaultTimeSpan};
    writer.storage.store(pomodoro);
    RemovePomodoroTransaction removePomodoro{writer, pomodoro};

    CHECK(!removePomodoro.undo());
    CHECK_EQUAL(1, writer.storage.size())
}

TEST(RemovePomodoroTransaction, test_remove_and_undo)
{
    FakePomodoroWriter writer;
    Pomodoro pomodoro{defaultTimeSpan};
    writer.storage.store(pomodoro);
    RemovePomodoroTransaction removePomodoro{writer, pomodoro};

    CHECK(removePomodoro.execute());
    CHECK(writer.storage.size() == 0);
    CHECK(removePomodoro.undo());
    CHECK_EQUAL(1, writer.storage.size());
}
