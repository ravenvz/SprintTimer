#include "core/entities/Pomodoro.h"
#include "core/entities/TodoItem.h"
#include "qt_storage_impl/QtPomoStorageWriter.h"
#include "qt_storage_impl/db_service.h"
#include <TestHarness.h>


TEST_GROUP(TestQtStorageImplementation)
{
    // DBService testDbService{":memory:"};
    // QtPomoStorageWriter pomoWriter{testDbService};
};

TEST(TestQtStorageImplementation, test_name)
{
    // TodoItem item{"Test item", 4, 3, {"Tag1", "Tag2"}, false};
    // Pomodoro pomo{item,
    //               TimeSpan{DateTime::currentDateTimeLocal(),
    //                        DateTime::currentDateTimeLocal()}};
    //
    // pomoWriter.save(pomo);
};
