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

class RequestingSprintsFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    CommandHandler<RegisterSprintBulkCommand>& registerSprintBulkHandler{
        commandComposer.registerSprintBulkHandler()};
    QueryHandler<RequestSprintsQuery>& requestSprintsHandler{
        queryComposer.requestSprintsHandler()};
    QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
};

TEST_F(RequestingSprintsFixture, requesting_sprints_in_given_date_range)
{
    using ::testing::Truly;
    createTaskHandler.handle(CreateTaskCommand{"Some task", {"Tag1"}, 17});
    createTaskHandler.handle(
        CreateTaskCommand{"Another task", {"Tag3", "Tag5"}, 8});
    const DateTime someDateTime{current_date_time_local()};
    const DateTime rangeStart = someDateTime - Days{5};
    const DateTime rangeEnd = someDateTime + Days{5};
    const DateRange range{rangeStart.date(), rangeEnd.date()};
    const DateTimeRange outOfRangeLeft =
        add_offset({someDateTime, someDateTime}, -Days{20});
    const DateTimeRange onLeftBorder{DateTime{rangeStart},
                                     DateTime{rangeStart}};
    const DateTimeRange inRange{someDateTime, someDateTime};
    const DateTimeRange onRightBorder{DateTime{rangeEnd}, DateTime{rangeEnd}};
    const DateTimeRange outOfRangeRight{
        add_offset({someDateTime, someDateTime}, Days{6})};
    const auto uuid =
        extractUuids(activeTasksHandler.handle(ActiveTasksQuery{})).front();
    registerSprintBulkHandler.handle(
        RegisterSprintBulkCommand{uuid,
                                  {outOfRangeLeft,
                                   onLeftBorder,
                                   inRange,
                                   onRightBorder,
                                   outOfRangeRight}});

    const auto sprints =
        requestSprintsHandler.handle(RequestSprintsQuery{range});

    EXPECT_THAT(
        sprints,
        ::testing::ElementsAre(
            Truly(matchers::MatchesSprintIgnoringUuid(SprintDTO{
                "irrelevant", uuid, "Some task", {"Tag1"}, onLeftBorder})),
            Truly(matchers::MatchesSprintIgnoringUuid(
                SprintDTO{"irrelevant", uuid, "Some task", {"Tag1"}, inRange})),
            Truly(matchers::MatchesSprintIgnoringUuid(SprintDTO{
                "irrelevant", uuid, "Some task", {"Tag1"}, onRightBorder}))));
}
