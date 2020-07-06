/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "mocks/CommandHandlerMock.h"
#include "mocks/QueryHandlerMock.h"
#include <qt_gui/presentation/TodaySprintsPresenter.h>

using namespace ::testing;
using namespace sprint_timer;

namespace {

std::vector<entities::Sprint> makeSomeSprints();

} // namespace

namespace sprint_timer::use_cases {

bool operator==(const DeleteSprintCommand& lhs, const DeleteSprintCommand& rhs)
{
    return lhs.sprint == rhs.sprint;
}

} // namespace sprint_timer::use_cases

class SprintOutlineViewMock : public ui::contracts::TodaySprints::View {
public:
    MOCK_METHOD(void,
                displaySprints,
                (const std::vector<entities::Sprint>&),
                (override));

    MOCK_METHOD(void,
                displayAddSprintDialog,
                (const std::vector<entities::Task>&,
                 dw::Weekday,
                 std::chrono::minutes),
                (override));
};

class TodaySprintsPresenterFixture : public ::testing::Test {
public:
    NiceMock<mocks::QueryHandlerMock<use_cases::RequestSprintsQuery,
                                     std::vector<entities::Sprint>>>
        requestSprintsHandler;
    NiceMock<mocks::CommandHandlerMock<use_cases::DeleteSprintCommand>>
        deleteSprintHandler;
    NiceMock<SprintOutlineViewMock> viewMock;
    const std::chrono::minutes sprintDuration{25};
    const dw::Weekday firstDayOfWeek{dw::Weekday::Monday};
    ui::TodaySprintsPresenter presenter{deleteSprintHandler,
                                        requestSprintsHandler};
};

TEST_F(TodaySprintsPresenterFixture, updates_view_with_sprints)
{
    const auto sprints = makeSomeSprints();
    ON_CALL(requestSprintsHandler, handle(_)).WillByDefault(Return(sprints));

    EXPECT_CALL(viewMock, displaySprints(sprints));

    presenter.attachView(viewMock);
}

TEST_F(TodaySprintsPresenterFixture, deletes_sprint)
{
    using entities::Tag;
    const entities::Sprint sprint{
        "Task 1",
        dw::DateTimeRange{dw::current_date_time(), dw::current_date_time()},
        {Tag{"Tag 1"}, Tag{"Tag 2"}},
        "1",
        "1"};

    EXPECT_CALL(deleteSprintHandler,
                handle(use_cases::DeleteSprintCommand{sprint}));

    presenter.onSprintDelete(sprint);
}

namespace {

std::vector<entities::Sprint> makeSomeSprints()
{
    using namespace dw;
    using namespace std::chrono_literals;
    using entities::Sprint;
    using entities::Tag;
    const Date someDate{Year{2020}, Month{7}, Day{6}};
    const DateTime someDateTime{someDate};
    const DateTimeRange someRange{someDateTime, someDateTime + 25min};
    std::vector<Sprint> sprints{
        Sprint{"Task 1", someRange, {Tag{"Tag 1"}, Tag{"Tag 2"}}, "1", "1"},
        Sprint{"Task 1",
               add_offset(someRange, 25min),
               {Tag{"Tag 1"}, Tag{"Tag 2"}},
               "2",
               "1"},
        Sprint{"Task 2", add_offset(someRange, 2h), {Tag{"Tag 3"}}, "3", "2"}};
    return sprints;
}

} // namespace
