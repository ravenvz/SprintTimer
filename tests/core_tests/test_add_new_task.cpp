#include "mocks/TaskStorageWriterMock.h"
#include "gtest/gtest.h"
#include <core/CommandInvoker.h>
#include <core/use_cases/AddNewTask.h>

using namespace sprint_timer;
using namespace sprint_timer::entities;
using namespace sprint_timer::use_cases;

class RegisterTaskFixture : public ::testing::Test {
public:
    Task someTask{"Task name",
                  4,
                  2,
                  "550e8400-e29b-41d4-a716-446655440000",
                  {Tag{"Tag1"}, Tag{"Tag2"}},
                  false,
                  dw::DateTime::fromYMD(2015, 11, 10)};
    TaskStorageWriterMock task_writer_mock;
    CommandInvoker commandInvoker;
};

TEST_F(RegisterTaskFixture, registers_task)
{
    EXPECT_CALL(task_writer_mock, save(someTask)).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<AddNewTask>(task_writer_mock, someTask));
}

TEST_F(RegisterTaskFixture, undo_adding_new_task)
{
    EXPECT_CALL(task_writer_mock, save(someTask)).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<AddNewTask>(task_writer_mock, someTask));

    EXPECT_CALL(task_writer_mock, remove(someTask)).Times(1);

    commandInvoker.undo();
}
