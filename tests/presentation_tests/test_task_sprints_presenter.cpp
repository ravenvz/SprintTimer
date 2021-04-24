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
#include "mocks/QueryHandlerMock.h"
#include "mocks/TaskSelectionColleagueMock.h"
#include "qt_gui/presentation/TaskSelectionMediator.h"
#include "qt_gui/presentation/TaskSprintsPresenter.h"

using sprint_timer::use_cases::SprintDTO;
using sprint_timer::use_cases::SprintsForTaskQuery;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using namespace std::chrono_literals;

namespace sprint_timer::use_cases {

bool operator==(const SprintsForTaskQuery& lhs, const SprintsForTaskQuery& rhs)
{
    return lhs.taskUuid == rhs.taskUuid;
}

} // namespace sprint_timer::use_cases

// TODO remove when DTOs are returned by query
using sprint_timer::entities::Sprint;
using sprint_timer::entities::Tag;

class TaskSprintsViewMock
    : public sprint_timer::ui::contracts::TaskSprintsContract::View {
public:
    MOCK_METHOD(void,
                displaySprints,
                (const std::vector<SprintDTO>&),
                (override));
};

class TaskSprintsPresenterFixture : public ::testing::Test {
public:
    TaskSprintsViewMock view;
    sprint_timer::ui::TaskSelectionMediator taskSelectionMediator;
    NiceMock<mocks::QueryHandlerMock<SprintsForTaskQuery>>
        sprintsForTaskHandler;
    sprint_timer::ui::TaskSprintsPresenter sut{sprintsForTaskHandler,
                                               taskSelectionMediator};
    NiceMock<mocks::TaskSelectionColleagueMock> fakeColleague;
    dw::DateTimeRange someTimeRange{dw::current_date_time(),
                                    dw::current_date_time() + 25min};
    std::vector<Sprint> someSprints{Sprint{"Some task name",
                                           someTimeRange,
                                           {Tag{"Tag1"}, Tag{"Tag2"}},
                                           "789",
                                           "123"},
                                    Sprint{"Some task name",
                                           dw::add_offset(someTimeRange, 2h),
                                           {Tag{"Tag1"}, Tag{"Tag2"}},
                                           "555",
                                           "123"}};
    std::vector<SprintDTO> someSprintDtos{
        SprintDTO{
            "789", "123", "Some task name", {"Tag1", "Tag2"}, someTimeRange},
        SprintDTO{"555",
                  "123",
                  "Some task name",
                  {"Tag1", "Tag2"},
                  dw::add_offset(someTimeRange, 2h)}};
};

TEST_F(TaskSprintsPresenterFixture,
       does_not_update_view_when_no_task_is_selected)
{
    EXPECT_CALL(view, displaySprints(_)).Times(0);

    sut.attachView(view);
}

TEST_F(TaskSprintsPresenterFixture,
       updates_view_with_sprints_for_task_that_is_currently_selected)
{
    taskSelectionMediator.changeSelection(&fakeColleague, 2, "123");
    ON_CALL(sprintsForTaskHandler, handle(SprintsForTaskQuery{"123"}))
        .WillByDefault(Return(someSprints));

    EXPECT_CALL(view, displaySprints(someSprintDtos));

    sut.attachView(view);
}
