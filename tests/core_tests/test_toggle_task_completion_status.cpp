#include "mocks/TaskStorageWriterMock.h"
#include "gtest/gtest.h"
#include <core/CommandInvoker.h>
#include <core/use_cases/ToggleTaskCompletionStatus.h>

using namespace sprint_timer;
using namespace sprint_timer::entities;
using namespace sprint_timer::use_cases;

class ToggleTaskCompletionFixture : public ::testing::Test {
public:
    Task someTask{"Task name",
                  4,
                  2,
                  "550e8400-e29b-41d4-a716-446655440000",
                  {Tag{"Tag1"}, Tag{"Tag2"}},
                  false,
                  dw::DateTime::fromYMD(2015, 11, 10)};

    CommandInvoker commandInvoker;
    TaskStorageWriterMock task_writer_mock;
};

TEST_F(ToggleTaskCompletionFixture, toggle_task_completion)
{
    EXPECT_CALL(task_writer_mock,
                toggleTaskCompletionStatus(
                    someTask.uuid(), dw::DateTime::currentDateTimeLocal()))
        .Times(1);

    commandInvoker.executeCommand(std::make_unique<ToggleTaskCompletionStatus>(
        task_writer_mock, someTask));
}

TEST_F(ToggleTaskCompletionFixture,
       undo_toggle_task_completion_status_should_not_modify_timestamp)
{
    EXPECT_CALL(task_writer_mock,
                toggleTaskCompletionStatus(
                    someTask.uuid(), dw::DateTime::currentDateTimeLocal()))
        .Times(1);

    commandInvoker.executeCommand(std::make_unique<ToggleTaskCompletionStatus>(
        task_writer_mock, someTask));

    EXPECT_CALL(
        task_writer_mock,
        toggleTaskCompletionStatus(someTask.uuid(), someTask.lastModified()))
        .Times(1);

    commandInvoker.undo();
}
