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
#include "QtStorageInitializer.h"
#include "core/SprintBuilder.h"
#include "core/TaskBuilder.h"
#include "core/WorkSchedule.h"
#include "gtest/gtest.h"

using namespace sprint_timer::storage::qt_storage;
using sprint_timer::SprintBuilder;
using sprint_timer::SprintStorageReader;
using sprint_timer::TaskBuilder;
using sprint_timer::TaskStorageReader;
using sprint_timer::entities::Sprint;
using sprint_timer::entities::Task;

sprint_timer::WeekSchedule
buildWeekSchedule(const std::array<int, 7>& raw_schedule)
{
    sprint_timer::WeekSchedule schedule;
    for (size_t i = 0; i < raw_schedule.size(); ++i)
        schedule.setTargetGoal(static_cast<dw::Weekday>(i), raw_schedule[i]);
    return schedule;
}

class QtStorageTestFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
};

TEST_F(QtStorageTestFixture, requests_all_tasks_in_given_date_range)
{
    using namespace dw;
    const auto taskStorage = initializer.factory.taskStorage();
    const DateRange targetDateRange{Date{Year{2018}, Month{10}, Day{10}},
                                    Date{Year{2018}, Month{10}, Day{15}}};
    const DateTime leftmostDateTime{DateTime{targetDateRange.start()}};
    const DateTime rightmostDateTime{DateTime{targetDateRange.finish()}};
    TaskBuilder builder;
    const Task taskOutOfRangeLeft =
        builder.withLastModificationStamp(leftmostDateTime - Days{1}).build();
    const Task taskLeftRangeBorder =
        builder.withLastModificationStamp(leftmostDateTime).build();
    const Task insideRangeTask1 =
        builder.withLastModificationStamp(leftmostDateTime + Days{1}).build();
    const Task insideRangeTask2 =
        builder.withLastModificationStamp(rightmostDateTime - Days{1}).build();
    const Task taskRightRangeBorder =
        builder.withLastModificationStamp(rightmostDateTime).build();
    const Task taskOutOfRangeRight =
        builder.withLastModificationStamp(rightmostDateTime + Days{1}).build();
    const std::vector<Task> expected{taskLeftRangeBorder,
                                     insideRangeTask1,
                                     insideRangeTask2,
                                     taskRightRangeBorder};

    taskStorage->save(taskOutOfRangeLeft);
    taskStorage->save(taskLeftRangeBorder);
    taskStorage->save(insideRangeTask1);
    taskStorage->save(insideRangeTask2);
    taskStorage->save(taskRightRangeBorder);
    taskStorage->save(taskOutOfRangeRight);

    const auto tasks = taskStorage->allTasks(targetDateRange);

    ASSERT_EQ(expected.size(), tasks.size());
    ASSERT_EQ(expected, tasks);
}

TEST_F(QtStorageTestFixture, reads_sprint_daily_distribution)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    const auto dailyDistReader = initializer.factory.dailyDistReader(30);
    using namespace dw;
    const Task someTask{TaskBuilder{}.build()};
    SprintBuilder sprintBuilder{SprintBuilder{}.withTaskUuid(someTask.uuid())};
    const DateTime someDate{Date{Year{2018}, Month{12}, Day{2}}};
    const DateRange range{(someDate - Days{29}).date(), someDate.date()};
    std::vector<Sprint> sprints;
    std::vector<int> expected{4, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2};

    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate - Days{30}};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    std::generate_n(
        std::back_inserter(sprints), 4, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate - Days{29}};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    std::generate_n(
        std::back_inserter(sprints), 3, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate - Days{20}};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    std::generate_n(
        std::back_inserter(sprints), 7, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate - Days{10}};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &someDate]() {
            return sprintBuilder.withTimeSpan(DateTimeRange{someDate, someDate})
                .build();
        });
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 3, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate + Days{1}};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });

    taskStorage->save(someTask);
    sprintStorage->save(sprints);

    const auto distribution = dailyDistReader->sprintDistribution(range);

    ASSERT_EQ(expected, distribution);
}

TEST_F(QtStorageTestFixture, retrieves_monthly_distribution)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    const auto monthlyDistReader = initializer.factory.monthlyDistReader();
    using namespace dw;
    auto first_day_of_month = [](const Date& date) {
        return Date{date.year(), date.month(), Day{1}};
    };
    const Task someTask{TaskBuilder{}.build()};
    SprintBuilder sprintBuilder{SprintBuilder{}.withTaskUuid(someTask.uuid())};
    const Date someDate{Year{2018}, Month{12}, Day{2}};
    Date lowerDate{first_day_of_month(someDate - Months{11})};
    const std::vector<int> expected{3, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 5};
    std::vector<Sprint> sprints;
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{
                last_day_of_month(someDate - Months(12))};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // On border
    std::generate_n(std::back_inserter(sprints),
                    3,
                    [&sprintBuilder, &someDate, &first_day_of_month]() {
                        const dw::DateTime timestamp{
                            first_day_of_month(someDate - Months{11})};
                        return sprintBuilder
                            .withTimeSpan(DateTimeRange{timestamp, timestamp})
                            .build();
                    });
    // Somewhere in the middle
    std::generate_n(
        std::back_inserter(sprints), 4, [&sprintBuilder, &someDate]() {
            const dw::DateTime timestamp{someDate - Months{5}};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // On border
    std::generate_n(
        std::back_inserter(sprints), 5, [&sprintBuilder, &someDate]() {
            return sprintBuilder
                .withTimeSpan(
                    DateTimeRange{DateTime{someDate}, DateTime{someDate}})
                .build();
        });
    // Out of range
    std::generate_n(std::back_inserter(sprints),
                    6,
                    [&sprintBuilder, &someDate, &first_day_of_month]() {
                        const dw::DateTime timestamp{
                            first_day_of_month(someDate + Months{1})};
                        return sprintBuilder
                            .withTimeSpan(DateTimeRange{timestamp, timestamp})
                            .build();
                    });

    taskStorage->save(someTask);
    sprintStorage->save(sprints);

    const auto distribution =
        monthlyDistReader->sprintDistribution(DateRange{lowerDate, someDate});

    ASSERT_EQ(expected, distribution);
}

TEST_F(QtStorageTestFixture,
       retrieves_sprint_weekly_distribution_with_monday_first_setting)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    const auto mondayFirstWeeklyDistributionReader =
        initializer.factory.weeklyDistReader(dw::Weekday::Monday);
    using namespace dw;
    const Task someTask{TaskBuilder{}.build()};
    SprintBuilder sprintBuilder{SprintBuilder{}.withTaskUuid(someTask.uuid())};
    const dw::DateTime upperDate{Date{Year{2016}, Month{2}, Day{12}}};
    const dw::DateTime lowerDate{upperDate - Weeks(11)};
    const std::vector<int> expected{3, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 6};
    std::vector<Sprint> sprints;
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &lowerDate]() {
            const dw::DateTime timestamp{lowerDate - Days{1}};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // On lower border
    std::generate_n(
        std::back_inserter(sprints), 3, [&sprintBuilder, &lowerDate]() {
            return sprintBuilder
                .withTimeSpan(DateTimeRange{lowerDate, lowerDate})
                .build();
        });
    // Week 53 in 2015
    std::generate_n(
        std::back_inserter(sprints), 4, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate +
                                         Days(-6 * 7 - 4)}; // Monday
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // Week 53 in 2016
    std::generate_n(
        std::back_inserter(sprints), 5, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate +
                                         Days(-6 * 7 + 2)}; // Sunday
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // On upper border
    std::generate_n(
        std::back_inserter(sprints), 6, [&sprintBuilder, &upperDate]() {
            return sprintBuilder
                .withTimeSpan(DateTimeRange{upperDate, upperDate})
                .build();
        });
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 7, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate + Days(1)};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });

    taskStorage->save(someTask);
    sprintStorage->save(sprints);

    const auto distribution =
        mondayFirstWeeklyDistributionReader->sprintDistribution(
            DateRange{lowerDate.date(), upperDate.date()});
    ASSERT_EQ(expected, distribution);
}

TEST_F(QtStorageTestFixture,
       retrieves_weekly_distribution_with_sunday_first_setting)
{
    const auto taskStorage = initializer.factory.taskStorage();
    const auto sprintStorage = initializer.factory.sprintStorage();
    const auto sundayFirstWeeklyDistributionReader =
        initializer.factory.weeklyDistReader(dw::Weekday::Sunday);
    using namespace dw;
    const Task someTask{TaskBuilder{}.build()};
    SprintBuilder sprintBuilder{SprintBuilder{}.withTaskUuid(someTask.uuid())};
    const dw::DateTime upperDate{Date{Year{2016}, Month{2}, Day{12}}};
    const dw::DateTime lowerDate{upperDate - Weeks(11)};
    const std::vector<int> expected{3, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 6};
    std::vector<Sprint> sprints;
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 2, [&sprintBuilder, &lowerDate]() {
            const dw::DateTime timestamp{lowerDate - Days(1)};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // On lower border
    std::generate_n(
        std::back_inserter(sprints), 3, [&sprintBuilder, &lowerDate]() {
            return sprintBuilder
                .withTimeSpan(DateTimeRange{lowerDate, lowerDate})
                .build();
        });
    // Week in 2015
    std::generate_n(
        std::back_inserter(sprints), 4, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate +
                                         Days(-6 * 7 - 5)}; // Sunday
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // Week in 2016
    std::generate_n(
        std::back_inserter(sprints), 5, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate +
                                         Days(-6 * 7 + 1)}; // Saturday
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });
    // On upper border
    std::generate_n(
        std::back_inserter(sprints), 6, [&sprintBuilder, &upperDate]() {
            return sprintBuilder
                .withTimeSpan(DateTimeRange{upperDate, upperDate})
                .build();
        });
    // Out of range
    std::generate_n(
        std::back_inserter(sprints), 7, [&sprintBuilder, &upperDate]() {
            const dw::DateTime timestamp{upperDate + Days(1)};
            return sprintBuilder
                .withTimeSpan(DateTimeRange{timestamp, timestamp})
                .build();
        });

    taskStorage->save(someTask);
    sprintStorage->save(sprints);

    const auto distribution =
        sundayFirstWeeklyDistributionReader->sprintDistribution(
            DateRange{lowerDate.date(), upperDate.date()});

    ASSERT_EQ(expected, distribution);
}

TEST_F(QtStorageTestFixture, stores_and_retrieves_work_schedule)
{
    auto scheduleStorage = initializer.factory.scheduleStorage();
    using namespace dw;
    using sprint_timer::WorkSchedule;
    WorkSchedule expected;
    expected.addWeekSchedule(Date{Year{2012}, Month{3}, Day{1}},
                             buildWeekSchedule({1, 1, 1, 1, 1, 0, 0}));
    expected.addWeekSchedule(Date{Year{2014}, Month{1}, Day{7}},
                             buildWeekSchedule({2, 2, 2, 2, 2, 0, 0}));
    expected.addWeekSchedule(Date{Year{2015}, Month{7}, Day{17}},
                             buildWeekSchedule({3, 3, 4, 3, 1, 7, 9}));
    expected.addWeekSchedule(Date{Year{2017}, Month{6}, Day{27}},
                             buildWeekSchedule({12, 12, 12, 12, 12, 0, 0}));
    expected.addWeekSchedule(Date{Year{2017}, Month{2}, Day{4}},
                             buildWeekSchedule({13, 13, 13, 13, 13, 0, 0}));
    expected.addWeekSchedule(Date{Year{2017}, Month{11}, Day{22}},
                             buildWeekSchedule({11, 11, 11, 11, 11, 11, 0}));
    expected.addWeekSchedule(Date{Year{2018}, Month{12}, Day{12}},
                             buildWeekSchedule({12, 12, 12, 12, 12, 0, 5}));
    expected.addExceptionalDay(Date{Year{2018}, Month{1}, Day{1}}, 0);
    expected.addExceptionalDay(Date{Year{2019}, Month{1}, Day{1}}, 0);
    expected.addExceptionalDay(Date{Year{2017}, Month{2}, Day{23}}, 12);
    expected.addExceptionalDay(Date{Year{2014}, Month{12}, Day{30}}, 12);

    scheduleStorage->updateSchedule(expected);
    const auto schedule = scheduleStorage->schedule();

    ASSERT_EQ(expected, schedule);
}
