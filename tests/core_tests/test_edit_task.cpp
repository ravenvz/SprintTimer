#include "mocks/TaskStorageWriterMock.h"
#include "gtest/gtest.h"
#include <core/CommandInvoker.h>
#include <core/TaskBuilder.h>
#include <core/use_cases/EditTask.h>

using namespace sprint_timer;
using namespace sprint_timer::use_cases;
using namespace sprint_timer::entities;

class EditTaskFixture : public ::testing::Test {
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

TEST_F(EditTaskFixture, edit_task_should_only_alter_allowed_parameters)
{
    Task editedTask = TaskBuilder{}
                          .withName("Edited")
                          .withEstimatedCost(someTask.estimatedCost() + 3)
                          .withActualCost(someTask.actualCost() + 2)
                          .withCompletionStatus(!someTask.isCompleted())
                          .withExplicitTags({Tag{"Tag2"}, Tag{"New Tag"}})
                          .build();
    Task restrictedTask
        = TaskBuilder{}
              .withUuid(someTask.uuid()) // Should not be editable
              .withActualCost(someTask.actualCost()) // Should not be editable
              .withCompletionStatus(
                  someTask.isCompleted()) // Should not be editable
              .withName(editedTask.name())
              .withEstimatedCost(editedTask.estimatedCost())
              .withExplicitTags(editedTask.tags())
              .build();
    EXPECT_CALL(task_writer_mock, edit(someTask, restrictedTask)).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<EditTask>(task_writer_mock, someTask, editedTask));
}

TEST_F(EditTaskFixture, undo_edit_task)
{
    Task editedTask = TaskBuilder{}
                          .withUuid(someTask.uuid())
                          .withExplicitTags({Tag{"New tag"}})
                          .withName("Edited")
                          .withActualCost(someTask.actualCost())
                          .withEstimatedCost(someTask.estimatedCost() + 2)
                          .build();
    EXPECT_CALL(task_writer_mock, edit(someTask, editedTask)).Times(1);

    commandInvoker.executeCommand(
        std::make_unique<EditTask>(task_writer_mock, someTask, editedTask));

    EXPECT_CALL(task_writer_mock, edit(editedTask, someTask));

    commandInvoker.undo();
}
