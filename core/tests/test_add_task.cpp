#include "use_cases/AddTaskTransaction.h"
#include "fixtures/FakeTaskStorageWriter.h"
#include <TestHarness.h>
#include <algorithm>
#include <vector>

using namespace UseCases;


TEST_GROUP(AddTaskUseCase)
{
    TodoItem someTask{"Whatever", 5, 3, {"Tag1", "Tag2"}, false};
};

TEST(AddTaskUseCase, test_execute_and_undo)
{
    FakeTaskStorageWriter writer;
    AddTaskTransaction transaction{writer, someTask};

    CHECK(transaction.execute());
    CHECK_EQUAL(1, writer.storage.size());
    CHECK(transaction.undo());
    CHECK_EQUAL(0, writer.storage.size());
}

TEST(AddTaskUseCase, test_should_not_undo_if_was_not_executed)
{
    FakeTaskStorageWriter writer;
    AddTaskTransaction transaction{writer, someTask};

    CHECK(!transaction.undo());
}
