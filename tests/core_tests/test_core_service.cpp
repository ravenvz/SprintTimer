/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

// TODO remove when Gtest drops std::tr1
// Workaround for C++17 as std::tr1 no longer available and Gtest uses it
#define GTEST_LANG_CXX11 1

#include "core/CommandInvoker.h"
#include "core/CoreService.h"
#include "core/SprintBuilder.h"
#include "core/TaskBuilder.h"
#include "mocks/SprintDistributionReaderMock.h"
#include "mocks/SprintStorageReaderMock.h"
#include "mocks/SprintStorageWriterMock.h"
#include "mocks/TaskStorageReaderMock.h"
#include "mocks/TaskStorageWriterMock.h"
#include "mocks/YearRangeReaderMock.h"
#include "gtest/gtest.h"

using dw::DateTime;
using dw::TimeSpan;
using ::testing::_;
using namespace sprint_timer;
using namespace sprint_timer::entities;

// TODO remove when DateWrapper implements equality
namespace dw {

inline bool operator==(const TimeSpan& lhs, const TimeSpan& rhs)
{
    return lhs.start() == rhs.start() && lhs.finish() == rhs.finish();
}

} // namespace dw

class CoreServiceFixture : public ::testing::Test {
public:
    const TimeSpan defaultTimeSpan
        = TimeSpan{DateTime::currentDateTime().add(DateTime::Days(-1)),
                   DateTime::currentDateTime().add(DateTime::Days(-1))};

    Task defaultTask{"Task name",
                     4,
                     2,
                     "550e8400-e29b-41d4-a716-446655440000",
                     {Tag{"Tag1"}, Tag{"Tag2"}},
                     false,
                     DateTime::fromYMD(2015, 11, 10)};

    TaskStorageWriterMock task_storage_writer_mock;
    TaskStorageReaderMock task_storage_reader_mock;
    SprintStorageWriterMock sprint_storage_writer_mock;
    SprintStorageReaderMock sprint_storage_reader_mock;
    YearRangeReaderMock year_range_reader_mock;
    SprintDistributionReaderMock sprint_distribution_reader_mock;
    CommandInvoker command_invoker;
    QueryExecutor query_executor;

    CoreService coreService{sprint_storage_reader_mock,
                            sprint_storage_writer_mock,
                            year_range_reader_mock,
                            task_storage_reader_mock,
                            task_storage_writer_mock,
                            sprint_distribution_reader_mock,
                            sprint_distribution_reader_mock,
                            sprint_distribution_reader_mock,
                            command_invoker,
                            query_executor};
};

TEST_F(CoreServiceFixture, registers_sprint_and_increments_tasks_sprints)
{
    EXPECT_CALL(sprint_storage_writer_mock, save(_)).Times(1);
    EXPECT_CALL(task_storage_writer_mock, incrementSprints(defaultTask.uuid()))
        .Times(1);

    coreService.registerSprint(defaultTimeSpan, defaultTask.uuid());
}

TEST_F(CoreServiceFixture, removes_sprint_and_decrements_tasks_sprint)
{
    const std::string taskUuid = defaultTask.uuid();
    const Sprint sprint{taskUuid, defaultTimeSpan};
    EXPECT_CALL(sprint_storage_writer_mock, remove(sprint)).Times(1);
    EXPECT_CALL(task_storage_writer_mock, decrementSprints(taskUuid)).Times(1);

    coreService.removeSprint(sprint);
}

TEST_F(CoreServiceFixture, request_year_range)
{
    EXPECT_CALL(year_range_reader_mock, requestYearRange(_));

    coreService.yearRange([](const std::vector<std::string>& result) {});
}

TEST_F(CoreServiceFixture, request_daily_distribution)
{
    EXPECT_CALL(sprint_distribution_reader_mock,
                requestDistribution(defaultTimeSpan, _));

    coreService.requestSprintDailyDistribution(
        defaultTimeSpan, [](const Distribution<int>& result) {});
}

TEST_F(CoreServiceFixture, request_weekly_distribution)
{
    EXPECT_CALL(sprint_distribution_reader_mock,
                requestDistribution(defaultTimeSpan, _));

    coreService.requestSprintWeeklyDistribution(
        defaultTimeSpan, [](const Distribution<int>& result) {});
}

TEST_F(CoreServiceFixture, request_monthly_distribution)
{
    EXPECT_CALL(sprint_distribution_reader_mock,
                requestDistribution(defaultTimeSpan, _));

    coreService.requestSprintMonthlyDistribution(
        defaultTimeSpan, [](const Distribution<int>& result) {});
}

TEST_F(CoreServiceFixture, undo_register_sprint)
{
    const std::string taskUuid = defaultTask.uuid();
    EXPECT_CALL(sprint_storage_writer_mock, save(_)).Times(1);
    EXPECT_CALL(task_storage_writer_mock, incrementSprints(taskUuid)).Times(1);

    coreService.registerSprint(defaultTimeSpan, taskUuid);

    EXPECT_CALL(sprint_storage_writer_mock, remove(_)).Times(1);
    EXPECT_CALL(task_storage_writer_mock, decrementSprints(taskUuid)).Times(1);

    coreService.undoLast();
}

TEST_F(CoreServiceFixture, request_finished_tasks)
{
    EXPECT_CALL(task_storage_reader_mock,
                requestFinishedTasks(defaultTimeSpan, _))
        .Times(1);

    coreService.requestFinishedTasks(defaultTimeSpan,
                                     [](const std::vector<Task>&) {});
}

TEST_F(CoreServiceFixture, request_all_tags)
{
    EXPECT_CALL(task_storage_reader_mock, requestAllTags(_)).Times(1);

    coreService.requestAllTags([](const std::vector<std::string>& tags) {});
}

TEST_F(CoreServiceFixture, edit_tag)
{
    EXPECT_CALL(task_storage_writer_mock, editTag("OldName", "NewName"))
        .Times(1);

    coreService.editTag("OldName", "NewName");
}

TEST_F(CoreServiceFixture, undo_edit_tag)
{
    EXPECT_CALL(task_storage_writer_mock, editTag("OldName", "NewName"))
        .Times(1);

    coreService.editTag("OldName", "NewName");

    EXPECT_CALL(task_storage_writer_mock, editTag("NewName", "OldName"))
        .Times(1);
    coreService.undoLast();
}

TEST_F(CoreServiceFixture, request_sprints_for_task)
{
    EXPECT_CALL(sprint_storage_reader_mock, sprintsForTask("123", _)).Times(1);

    coreService.requestSprintsForTask(
        "123", [](const std::vector<Sprint>& sprints) {});
}
