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
#include "api_tests/fixtures/StatisticsDataFixture.h"
#include "api_tests/utils/utils.h"

using namespace std::chrono_literals;
using namespace dw;

namespace {

constexpr DateTime initialDateTime{Date{Year{2022}, Month{1}, Day{15}}};

const std::vector<DateTimeRange> firstTaskSprints{
    createRange(initialDateTime + Days{3} + 10h),
    createRange(initialDateTime + Days{3} + 2h),
    createRange(initialDateTime + Days{4} + 9h),
    createRange(initialDateTime + Days{7} + 13h)};
const std::vector<DateTimeRange> secondTaskSprints{
    createRange(initialDateTime + Days{4} + 18h),
    createRange(initialDateTime + Days{10} + 5h)};
const std::vector<DateTimeRange> thirdTaskSprints{
    createRange(initialDateTime + 12h),
    createRange(initialDateTime + 20h),
    createRange(initialDateTime + Days{2} + 10h),
    createRange(initialDateTime + Days{2} + 17h),
    createRange(initialDateTime + Days{9} + 21h)};
const std::vector<DateTimeRange> fourthTaskSprints{
    createRange(initialDateTime + 1h),
    createRange(initialDateTime + Days{3} + 7h),
    createRange(initialDateTime + Days{6} + 5h),
    createRange(initialDateTime + Days{7} + 7h),
    createRange(initialDateTime + Days{7} + 20h),
    createRange(initialDateTime + Days{10} + 6h)};
const std::vector<DateTimeRange> fifthTaskSprints{
    createRange(initialDateTime + Days{3} + 23h),
    createRange(initialDateTime + Days{5} + 7h),
    createRange(initialDateTime + Days{8} + 14h),
    createRange(initialDateTime + Days{9} + 16h)};
const std::vector<DateTimeRange> sixthTaskSprints{
    createRange(initialDateTime + Days{3} + 12h),
    createRange(initialDateTime + Days{5} + 4h),
    createRange(initialDateTime + Days{5} + 19h)};

} // namespace

namespace fixtures {

dw::DateTimeRange givenStatisticsDataFixtureLoaded(
    asp::CommandHandler<sprint_timer::api::CreateTaskCommand>&
        createTaskHandler,
    asp::CommandHandler<sprint_timer::api::RegisterSprintBulkCommand>&
        registerSprintsHandler)
{
    using namespace sprint_timer::api;

    createTaskHandler.handle(CreateTaskCommand{"First task", {"Tag1"}, 4});
    createTaskHandler.handle(CreateTaskCommand{"Second task", {"Tag2"}, 2});
    createTaskHandler.handle(CreateTaskCommand{"Third task", {"Tag3"}, 5});
    createTaskHandler.handle(
        CreateTaskCommand{"Fourth task", {"Tag2", "Tag1"}, 6});
    createTaskHandler.handle(CreateTaskCommand{"Fifth task", {"Tag4"}, 4});
    createTaskHandler.handle(
        CreateTaskCommand{"Sixth task", {"Tag5", "Tag4"}, 3});

    registerSprintsHandler.handle(
        RegisterSprintBulkCommand{"0", firstTaskSprints});
    registerSprintsHandler.handle(
        RegisterSprintBulkCommand{"1", secondTaskSprints});
    registerSprintsHandler.handle(
        RegisterSprintBulkCommand{"2", thirdTaskSprints});
    registerSprintsHandler.handle(
        RegisterSprintBulkCommand{"3", fourthTaskSprints});
    registerSprintsHandler.handle(
        RegisterSprintBulkCommand{"4", fifthTaskSprints});
    registerSprintsHandler.handle(
        RegisterSprintBulkCommand{"5", sixthTaskSprints});

    return dw::DateTimeRange{initialDateTime, initialDateTime + Days{10}};
}

} // namespace fixtures
