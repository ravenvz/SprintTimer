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

TEST_F(ReadingSprintDistributionsFixture, reads_sprint_daily_distribution)
{
    createTaskHandler.handle(
        CreateTaskCommand{"Some task", {"Tag1", "Tag2"}, 28});
    const auto taskUuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    std::vector<dw::DateTimeRange> sprintRanges;
    const DateTime someDate{Date{Year{2018}, Month{12}, Day{2}}};
    const DateRange range{(someDate - Days{29}).date(), someDate.date()};
    const std::vector<int> expected{4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 2};
    // Out of range
    std::generate_n(std::back_inserter(sprintRanges), 2, [&]() {
        return DateTimeRange{DateTime{someDate - Days{30}},
                             DateTime{someDate - Days{30}}};
    });
    // On left border
    std::generate_n(std::back_inserter(sprintRanges), 4, [&]() {
        return DateTimeRange{DateTime{someDate - Days{29}},
                             DateTime{someDate - Days{29}}};
    });
    // In range
    std::generate_n(std::back_inserter(sprintRanges), 3, [&]() {
        return DateTimeRange{DateTime{someDate - Days{20}},
                             DateTime{someDate - Days{20}}};
    });
    // In range
    std::generate_n(std::back_inserter(sprintRanges), 7, [&]() {
        return DateTimeRange{DateTime{someDate - Days{10}},
                             DateTime{someDate - Days{10}}};
    });
    // On right border
    std::generate_n(std::back_inserter(sprintRanges), 2, [&]() {
        return DateTimeRange{DateTime{someDate}, DateTime{someDate}};
    });
    // Out of range
    std::generate_n(std::back_inserter(sprintRanges), 3, [&]() {
        return DateTimeRange{DateTime{someDate + Days{1}},
                             DateTime{someDate + Days{1}}};
    });
    registerSprintBulkHandler.handle(
        RegisterSprintBulkCommand{taskUuid, sprintRanges});

    const auto distribution =
        dailyDistHandler.handle(RequestSprintDistributionQuery{range});

    ASSERT_EQ(expected, distribution);
}

TEST_F(ReadingSprintDistributionsFixture, reads_monthly_distribution)
{
    auto first_day_of_month = [](const Date& date) {
        return Date{date.year(), date.month(), Day{1}};
    };
    createTaskHandler.handle(
        CreateTaskCommand{"Some task", std::vector<std::string>{}, 25});
    const auto taskUuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const Date someDate{Year{2018}, Month{12}, Day{2}};
    const Date lowerDate{first_day_of_month(someDate - Months{11})};
    const std::vector<int> expected{3, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 5};
    std::vector<DateTimeRange> sprintRanges;
    // Out of range
    std::generate_n(std::back_inserter(sprintRanges), 2, [&]() {
        const DateTime timestamp{last_day_of_month(someDate - Months{12})};
        return DateTimeRange{timestamp, timestamp};
    });
    // On border
    std::generate_n(std::back_inserter(sprintRanges), 3, [&]() {
        const DateTime timestamp{first_day_of_month(someDate - Months{11})};
        return DateTimeRange{timestamp, timestamp};
    });
    // In range
    std::generate_n(std::back_inserter(sprintRanges), 4, [&]() {
        const DateTime timestamp{someDate - Months{5}};
        return DateTimeRange{timestamp, timestamp};
    });
    // On border
    std::generate_n(std::back_inserter(sprintRanges), 5, [&]() {
        return DateTimeRange{DateTime{someDate}, DateTime{someDate}};
    });
    // Out of range
    std::generate_n(std::back_inserter(sprintRanges), 6, [&]() {
        const DateTime timestamp{first_day_of_month(someDate + Months{1})};
        return DateTimeRange{timestamp, timestamp};
    });
    registerSprintBulkHandler.handle(
        RegisterSprintBulkCommand{taskUuid, sprintRanges});

    const auto distribution = monthlyDistHandler.handle(
        RequestSprintDistributionQuery{DateRange{lowerDate, someDate}});

    ASSERT_EQ(expected, distribution);
}

TEST_F(ReadingSprintDistributionsFixture,
       reads_sprint_weekly_distribution_with_monday_first_setting)
{
    createTaskHandler.handle(
        CreateTaskCommand{"Some task", std::vector<std::string>{}, 12});
    const auto taskUuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const dw::DateTime upperDate{Date{Year{2016}, Month{2}, Day{12}}};
    const dw::DateTime lowerDate{upperDate - Weeks(11)};
    const std::vector<int> expected{3, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 6};
    std::vector<DateTimeRange> sprintRanges;
    // Out of range
    std::generate_n(std::back_inserter(sprintRanges), 2, [&]() {
        const DateTime timeStamp{lowerDate - Days{1}};
        return DateTimeRange{timeStamp, timeStamp};
    });
    // On border
    std::generate_n(std::back_inserter(sprintRanges), 3, [&]() {
        return DateTimeRange{lowerDate, lowerDate};
    });
    // Week 53 in 2015
    std::generate_n(std::back_inserter(sprintRanges), 4, [&]() {
        const DateTime timeStamp{upperDate + Days{-6 * 7 - 4}}; // Monday
        return DateTimeRange{timeStamp, timeStamp};
    });
    // Week 53 in 2016
    std::generate_n(std::back_inserter(sprintRanges), 5, [&]() {
        const DateTime timeStamp{upperDate + Days{-6 * 7 + 2}}; // Sunday
        return DateTimeRange{timeStamp, timeStamp};
    });
    // On border
    std::generate_n(std::back_inserter(sprintRanges), 6, [&]() {
        return DateTimeRange{upperDate, upperDate};
    });
    // Out of range
    std::generate_n(std::back_inserter(sprintRanges), 7, [&]() {
        const DateTime timeStamp{upperDate + Days{1}};
        return DateTimeRange{timeStamp, timeStamp};
    });
    registerSprintBulkHandler.handle(
        RegisterSprintBulkCommand{taskUuid, sprintRanges});

    const auto distribution =
        mondayFirstDistHandler.handle(RequestSprintDistributionQuery{
            DateRange{lowerDate.date(), upperDate.date()}});

    ASSERT_EQ(expected, distribution);
}

TEST_F(ReadingSprintDistributionsFixture,
       reads_sprint_weekly_distribution_with_sunday_first_setting)
{
    createTaskHandler.handle(
        CreateTaskCommand{"Some task", std::vector<std::string>{}, 77});
    const auto taskUuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const dw::DateTime upperDate{Date{Year{2016}, Month{2}, Day{12}}};
    const dw::DateTime lowerDate{upperDate - Weeks(11)};
    const std::vector<int> expected{3, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 6};
    std::vector<DateTimeRange> sprintRanges;
    // Out of range
    std::generate_n(std::back_inserter(sprintRanges), 2, [&]() {
        const DateTime timestamp{lowerDate - Days{1}};
        return DateTimeRange{timestamp, timestamp};
    });
    // On lower border
    std::generate_n(std::back_inserter(sprintRanges), 3, [&]() {
        return DateTimeRange{lowerDate, lowerDate};
    });
    // Some week in 2015
    std::generate_n(std::back_inserter(sprintRanges), 4, [&]() {
        const DateTime timestamp{upperDate + Days{-6 * 7 - 5}}; // Sunday
        return DateTimeRange{timestamp, timestamp};
    });
    // Some week in 2016
    std::generate_n(std::back_inserter(sprintRanges), 5, [&]() {
        const DateTime timestamp{upperDate + Days{-6 * 7 + 1}}; // Saturday
        return DateTimeRange{timestamp, timestamp};
    });
    // On upper border
    std::generate_n(std::back_inserter(sprintRanges), 6, [&]() {
        return DateTimeRange{upperDate, upperDate};
    });
    // Out of range
    std::generate_n(std::back_inserter(sprintRanges), 7, [&]() {
        const DateTime timestamp{upperDate + Days{1}};
        return DateTimeRange{timestamp, timestamp};
    });
    registerSprintBulkHandler.handle(
        RegisterSprintBulkCommand{taskUuid, sprintRanges});

    const auto distribution =
        sundayFirstDistHandler.handle(RequestSprintDistributionQuery{
            DateRange{lowerDate.date(), upperDate.date()}});

    ASSERT_EQ(expected, distribution);
}
