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
#include "use_cases_tests/matchers/MatchesTaskIgnoringUuid.h"
#include "gmock/gmock.h"

using namespace sprint_timer;
using namespace sprint_timer::use_cases;
using namespace sprint_timer::entities;
using namespace sprint_timer::compose;
using namespace dw;

namespace {

sprint_timer::WeekSchedule
buildWeekSchedule(const std::array<int, 7>& raw_schedule)
{
    sprint_timer::WeekSchedule schedule;
    for (size_t i = 0; i < raw_schedule.size(); ++i) {
        schedule.setTargetGoal(static_cast<dw::Weekday>(i), raw_schedule[i]);
    }
    return schedule;
}

} // namespace

class RetrivingWorkScheduleFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    CommandHandler<ChangeWorkScheduleCommand>& changeWorkScheduleHandler{
        commandComposer.changeWorkScheduleHandler()};
    QueryHandler<WorkScheduleQuery>& workScheduleHandler{
        queryComposer.workScheduleHandler()};
};

TEST_F(RetrivingWorkScheduleFixture, requesting_finished_tasks)
{
    WorkSchedule oldSchedule;
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
    changeWorkScheduleHandler.handle(
        ChangeWorkScheduleCommand{oldSchedule, expected});

    const auto actual = workScheduleHandler.handle(WorkScheduleQuery{});

    EXPECT_EQ(expected, actual);
}
