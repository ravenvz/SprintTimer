#include "mocks/TaskStorageReaderMock.h"
#include "gtest/gtest.h"
#include <core/use_cases/RequestUnfinishedTasks.h>

using ::testing::_;

class RequestUnfinishedTasksFixture : public ::testing::Test {
public:
    TaskStorageReaderMock task_storage_reader_mock;
};

TEST_F(RequestUnfinishedTasksFixture, wires_request_to_storage)
{
    using namespace sprint_timer::use_cases;
    using namespace sprint_timer::entities;
    RequestUnfinishedTasks query{task_storage_reader_mock,
                                 [](const std::vector<Task>&) {}};
    EXPECT_CALL(task_storage_reader_mock, requestUnfinishedTasks(_)).Times(1);

    query.execute();
}
