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
#include "api_tests/matchers/MatchesTaskIgnoringUuid.h"
#include "gmock/gmock.h"

using namespace sprint_timer;
using namespace sprint_timer::api;
using namespace sprint_timer;
using namespace sprint_timer::compose;
using namespace dw;

class ChangingWorkScheduleFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    asp::CommandHandler<ChangeWorkScheduleCommand>& changeWorkScheduleHandler{
        commandComposer.changeWorkScheduleHandler()};
    asp::QueryHandler<WorkScheduleQuery>& workScheduleHandler{
        queryComposer.workScheduleHandler()};
    asp::CommandHandler<UndoLastCommand>& undoHandler{
        commandComposer.undoHandler()};
};

TEST_F(ChangingWorkScheduleFixture, changing_work_schedule)
{
    WorkSchedule oldSchedule;
    WorkSchedule newSchedule;
    newSchedule.addWeekSchedule(Date{Year{2012}, Month{3}, Day{1}},
                                WeekSchedule{{1, 1, 1, 1, 1, 0, 0}});
    newSchedule.addWeekSchedule(Date{Year{2018}, Month{12}, Day{12}},
                                WeekSchedule{{12, 12, 12, 12, 12, 0, 5}});
    newSchedule.addExceptionalDay(Date{Year{2018}, Month{1}, Day{1}}, 0);
    newSchedule.addExceptionalDay(Date{Year{2019}, Month{1}, Day{1}}, 0);

    changeWorkScheduleHandler.handle(
        ChangeWorkScheduleCommand{oldSchedule, newSchedule});

    EXPECT_EQ(newSchedule, workScheduleHandler.handle(WorkScheduleQuery{}));
}

TEST_F(ChangingWorkScheduleFixture, undoing_changing_work_schedule)
{
    WorkSchedule oldSchedule;
    oldSchedule.addWeekSchedule(Date{Year{2014}, Month{2}, Day{3}},
                                WeekSchedule{{12, 12, 12, 12, 12, 0, 0}});
    oldSchedule.addWeekSchedule(Date{Year{2022}, Month{7}, Day{26}},
                                WeekSchedule{{12, 0, 0, 12, 12, 0, 0}});
    WorkSchedule newSchedule;
    newSchedule.addWeekSchedule(Date{Year{2012}, Month{3}, Day{1}},
                                WeekSchedule{{1, 1, 1, 1, 1, 0, 0}});
    newSchedule.addWeekSchedule(Date{Year{2018}, Month{12}, Day{12}},
                                WeekSchedule{{12, 12, 12, 12, 12, 0, 5}});
    newSchedule.addExceptionalDay(Date{Year{2018}, Month{1}, Day{1}}, 0);
    newSchedule.addExceptionalDay(Date{Year{2019}, Month{1}, Day{1}}, 0);

    changeWorkScheduleHandler.handle(
        ChangeWorkScheduleCommand{oldSchedule, newSchedule});
    undoHandler.handle(UndoLastCommand{});

    EXPECT_EQ(oldSchedule, workScheduleHandler.handle(WorkScheduleQuery{}));
}
