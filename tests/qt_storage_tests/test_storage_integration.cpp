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
#include "core/WorkSchedule.h"
#include "gtest/gtest.h"

using namespace sprint_timer::storage::qt_storage;
using sprint_timer::SprintStorageReader;
using sprint_timer::TaskStorageReader;
using sprint_timer::entities::Sprint;
using sprint_timer::entities::Task;
using sprint_timer::entities::Tag;

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

// TEST_F(QtStorageTestFixture, requests_all_tasks_in_given_date_range)
// {
//     using namespace dw;
//     const auto taskStorage = initializer.factory.taskStorage();
//     const DateRange targetDateRange{Date{Year{2018}, Month{10}, Day{10}},
//                                     Date{Year{2018}, Month{10}, Day{15}}};
//     const DateTime leftmostDateTime{DateTime{targetDateRange.start()}};
//     const DateTime rightmostDateTime{DateTime{targetDateRange.finish()}};
//     TaskBuilder builder;
//     const std::string taskName{"Some name"};
//     const std::string taskUuid{generator.generateUUID()};
//     const std::list<Tag> tags{Tag{"Tag"}, Tag{"OtherTag"}};
//     const Task taskOutOfRangeLeft = Task{taskName, 5, 0, taskUuid, tags, false, fakeTimeSource};
//         builder.withLastModificationStamp(leftmostDateTime - Days{1}).build();
//     const Task taskLeftRangeBorder =
//         builder.withLastModificationStamp(leftmostDateTime).build();
//     const Task insideRangeTask1 =
//         builder.withLastModificationStamp(leftmostDateTime + Days{1}).build();
//     const Task insideRangeTask2 =
//         builder.withLastModificationStamp(rightmostDateTime - Days{1}).build();
//     const Task taskRightRangeBorder =
//         builder.withLastModificationStamp(rightmostDateTime).build();
//     const Task taskOutOfRangeRight =
//         builder.withLastModificationStamp(rightmostDateTime + Days{1}).build();
//     const std::vector<Task> expected{taskLeftRangeBorder,
//                                      insideRangeTask1,
//                                      insideRangeTask2,
//                                      taskRightRangeBorder};
//
//     taskStorage->save(taskOutOfRangeLeft);
//     taskStorage->save(taskLeftRangeBorder);
//     taskStorage->save(insideRangeTask1);
//     taskStorage->save(insideRangeTask2);
//     taskStorage->save(taskRightRangeBorder);
//     taskStorage->save(taskOutOfRangeRight);
//
//     const auto tasks = taskStorage->allTasks(targetDateRange);
//
//     ASSERT_EQ(expected.size(), tasks.size());
//     ASSERT_EQ(expected, tasks);
// }

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
