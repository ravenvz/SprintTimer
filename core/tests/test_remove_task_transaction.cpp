#include "use_cases/RemoveTaskTransaction.h"
#include "fixtures/FakeTaskStorageWriter.h"
#include <TestHarness.h>

using namespace UseCases;

TEST_GROUP(RemoveTaskTransaction)
{
    TodoItem someTask{"Whatever", 5, 3, {"Tag1", "Tag2"}, false};
};

TEST(RemoveTaskTransaction, test_execute_and_undo)
{
    FakeTaskStorageWriter writer;
    writer.storage.store(someTask);
    RemoveTaskTransaction transaction{writer, someTask};

    CHECK_EQUAL(1, writer.storage.size());
    CHECK(transaction.execute());
    CHECK_EQUAL(0, writer.storage.size());
    CHECK(transaction.undo());
    CHECK_EQUAL(1, writer.storage.size());
}

TEST(RemoveTaskTransaction,
     test_undo_transaction_that_has_not_been_executed_should_return_false)
{
    FakeTaskStorageWriter writer;
    RemoveTaskTransaction transaction{writer, someTask};

    CHECK(!transaction.undo());
}
