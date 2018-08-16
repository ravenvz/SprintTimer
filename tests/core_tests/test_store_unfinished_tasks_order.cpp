#include "mocks/TaskStorageWriterMock.h"
#include "gtest/gtest.h"
#include <core/CommandInvoker.h>
#include <core/use_cases/StoreUnfinishedTasksOrder.h>

using namespace sprint_timer;
using namespace sprint_timer::use_cases;

class StoreUnfinishedTasksOrderFixture : public ::testing::Test {
public:
    TaskStorageWriterMock task_writer_mock;
    CommandInvoker commandInvoker;
};

TEST_F(StoreUnfinishedTasksOrderFixture, test_name)
{
    const std::vector<std::string> new_priorities{"2", "1", "3"};
    EXPECT_CALL(task_writer_mock, updatePriorities(new_priorities)).Times(1);

    commandInvoker.executeCommand(std::make_unique<StoreUnfinishedTasksOrder>(
        task_writer_mock,
        std::vector<std::string>{"1", "2", "3"},
        std::vector<std::string>{"2", "1", "3"}));
}

TEST_F(StoreUnfinishedTasksOrderFixture, undo_changing_tasks_order)
{
    const std::vector<std::string> old_priorities{"1", "2", "3"};
    const std::vector<std::string> new_priorities{"2", "1", "3"};
    EXPECT_CALL(task_writer_mock, updatePriorities(new_priorities)).Times(1);

    commandInvoker.executeCommand(std::make_unique<StoreUnfinishedTasksOrder>(
        task_writer_mock,
        std::vector<std::string>{"1", "2", "3"},
        std::vector<std::string>{"2", "1", "3"}));

    EXPECT_CALL(task_writer_mock, updatePriorities(old_priorities)).Times(1);
    commandInvoker.undo();
}
