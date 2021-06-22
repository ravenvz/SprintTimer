/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include "use_cases_tests/QtStorageInitializer.h"
#include "use_cases_tests/matchers/MatchesSprintIgnoringUuid.h"
#include "use_cases_tests/utils/utils.h"
#include "gmock/gmock.h"

using namespace sprint_timer;
using namespace sprint_timer::use_cases;
using namespace sprint_timer::entities;
using namespace sprint_timer::compose;
using namespace dw;

class RetrievingOperationRangeFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    CommandHandler<RegisterSprintBulkCommand>& registerSprintBulkHandler{
        commandComposer.registerSprintBulkHandler()};
    QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
    QueryHandler<OperationalRangeQuery>& operationalRangeHandler{
        queryComposer.operationalRangeHandler()};
};

TEST_F(RetrievingOperationRangeFixture,
       retrieves_operation_range_using_current_date_as_upper_bound)
{
    createTaskHandler.handle(
        CreateTaskCommand{"Some task", {"Tag1", "Tag2"}, 4});
    const auto taskUuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const dw::DateTime timestamp{
        dw::DateTime{Date{Year{2018}, Month{12}, Day{1}}}};
    registerSprintBulkHandler.handle(RegisterSprintBulkCommand{
        taskUuid, {DateTimeRange{timestamp, timestamp}}});
    const DateRange expected{Date{Year{2018}, Month{12}, Day{1}},
                             current_date_local()};

    const auto actual = operationalRangeHandler.handle(OperationalRangeQuery{});

    EXPECT_EQ(expected, actual);
}

TEST_F(RetrievingOperationRangeFixture, retrieves_operation_range)
{
    createTaskHandler.handle(
        CreateTaskCommand{"Some task", {"Tag1", "Tag2"}, 4});
    const auto taskUuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const dw::DateTime timestamp{
        dw::DateTime{Date{Year{2018}, Month{12}, Day{1}}}};
    registerSprintBulkHandler.handle(RegisterSprintBulkCommand{
        taskUuid,
        {add_offset(DateTimeRange{timestamp, timestamp}, -Years{4})}});
    registerSprintBulkHandler.handle(RegisterSprintBulkCommand{
        taskUuid,
        {add_offset(DateTimeRange{timestamp, timestamp}, +Years{5})}});
    const DateRange expected{Date{Year{2014}, Month{12}, Day{1}},
                             Date{Year{2023}, Month{12}, Day{1}}};

    const auto actual = operationalRangeHandler.handle(OperationalRangeQuery{});

    EXPECT_EQ(expected, actual);
}
