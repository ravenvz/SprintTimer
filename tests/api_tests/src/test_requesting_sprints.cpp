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
#include "api_tests/QtStorageInitializer.h"
#include "api_tests/utils/utils.h"
#include "gmock/gmock.h"

using namespace sprint_timer;
using namespace sprint_timer::api;
using namespace sprint_timer;
using namespace sprint_timer::compose;
using namespace dw;
using namespace std::chrono_literals;

class RequestingSprintsFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    asp::CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    asp::CommandHandler<RegisterSprintBulkCommand>& registerSprintBulkHandler{
        commandComposer.registerSprintBulkHandler()};
    asp::QueryHandler<RequestSprintsQuery>& requestSprintsHandler{
        queryComposer.requestSprintsHandler()};
    asp::QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
    asp::CommandHandler<DeleteSprintCommand>& deleteSprintHandler{
        commandComposer.deleteSprintHandler()};
};

TEST_F(RequestingSprintsFixture, requesting_sprints_in_given_date_range)
{
    const DateTime someDateTime{DateTime{Date{Year{2025}, Month{1}, Day{28}}} +
                                17h + 31min};
    createTaskHandler.handle(CreateTaskCommand{"Some task",
                                               {"Tag1"},
                                               17,
                                               TaskTypeDTO::Regular,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{someDateTime}});
    createTaskHandler.handle(CreateTaskCommand{"Another task",
                                               {"Tag3", "Tag5"},
                                               8,
                                               TaskTypeDTO::Regular,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{someDateTime}});
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

    EXPECT_THAT(sprints,
                ::testing::ElementsAre(
                    SprintDTO{"Some task", {"Tag1"}, onLeftBorder},
                    SprintDTO{"Some task", {"Tag1"}, inRange},
                    SprintDTO{"Some task", {"Tag1"}, onRightBorder}));
}

TEST_F(RequestingSprintsFixture,
       requesting_sprints_in_given_date_range_ignores_deleted_sprints)
{
    using namespace std::chrono_literals;
    const DateTime someDateTime =
        DateTime{Date{Year{2022}, Month{7}, Day{21}}} + 3h;
    const DateTimeRange first{someDateTime, someDateTime + 25min};
    const DateTimeRange second{someDateTime + 3h, someDateTime + 3h + 25min};
    const DateTimeRange third{someDateTime + 7h, someDateTime + 7h + 25min};
    createTaskHandler.handle(CreateTaskCommand{"Some task",
                                               {"Tag1", "Tag2"},
                                               8,
                                               TaskTypeDTO::Regular,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{someDateTime}});
    const auto uuid =
        extractUuids(activeTasksHandler.handle(ActiveTasksQuery{})).front();
    registerSprintBulkHandler.handle(
        RegisterSprintBulkCommand{uuid, {first, second, third}});

    deleteSprintHandler.handle(DeleteSprintCommand{second});
    const auto actual = requestSprintsHandler.handle(RequestSprintsQuery{
        DateRange{first.start().date(), third.start().date()}});

    EXPECT_THAT(actual,
                ::testing::ElementsAre(
                    SprintDTO{"Some task", {"Tag1", "Tag2"}, first},
                    SprintDTO{"Some task", {"Tag1", "Tag2"}, third}));
}
