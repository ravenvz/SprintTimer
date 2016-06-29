#include "fixtures/FakeTaskStorageWriter.h"
#include "use_cases/IncrementSpentPomodoros.h"
#include <TestHarness.h>


TEST_GROUP(IncrementSpentPomodoros)
{
    Task defaultItem{
        "Item name", 4, 2, {Tag{"Tag 1"}, Tag{"Tag 2"}}, false};
};

TEST(IncrementSpentPomodoros, test_execute_and_undo)
{
    FakeStorage<Task> storage;
    FakeTaskStorageWriter writer{storage};
    writer.save(defaultItem);
    std::string uuid = defaultItem.uuid();

    UseCases::IncrementSpentPomodoros increment{writer, uuid};
    increment.execute();

    CHECK_EQUAL(3, writer.storage.getItem(uuid).value().spentPomodoros());

    increment.undo();

    CHECK_EQUAL(2, writer.storage.getItem(uuid).value().spentPomodoros());
}

TEST(IncrementSpentPomodoros, test_should_not_undo_if_was_not_executed)
{
    FakeStorage<Task> storage;
    FakeTaskStorageWriter writer{storage};
    writer.save(defaultItem);
    std::string uuid = defaultItem.uuid();

    UseCases::IncrementSpentPomodoros increment{writer, uuid};

    CHECK_EQUAL(2, writer.storage.getItem(uuid).value().spentPomodoros());

    increment.undo();

    CHECK_EQUAL(2, writer.storage.getItem(uuid).value().spentPomodoros());
}
