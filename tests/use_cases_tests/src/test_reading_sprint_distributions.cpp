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

class ReadingSprintDistributionsFixture : public ::testing::Test {
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
    QueryHandler<RequestSprintDistributionQuery>& dailyDistHandler{
        queryComposer.dailyDistHandler()};
    QueryHandler<RequestSprintDistributionQuery>& mondayFirstDistHandler{
        queryComposer.weeklyDistHandler(Weekday::Monday)};
    QueryHandler<RequestSprintDistributionQuery>& sundayFirstDistHandler{
        queryComposer.weeklyDistHandler(Weekday::Sunday)};
    QueryHandler<RequestSprintDistributionQuery>& monthlyDistHandler{
        queryComposer.monthlyDistHandler()};
};

// TEST_F(ReadingSprintDistributionsFixture, reads_sprint_daily_distribution)
// {
//     createTaskHandler.handle(
//         CreateTaskCommand{"Some task", {"Tag1", "Tag2"}, 28});
//     const auto taskUuid =
//         activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
//     std::vector<dw::DateRange> sprintRanges;
//     const DateTime someDate{Date{Year{2018}, Month{12}, Day{2}}};
//     const DateRange range{(someDate - Days{29}).date(), someDate.date()};
//
//     // Out of range
//     std::generate_n(std::back_inserter(sprintRanges), 2, []() {});
// }
