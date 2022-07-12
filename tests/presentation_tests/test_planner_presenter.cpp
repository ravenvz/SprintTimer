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
#include "api/requests/ReadTaskTreeQuery.h"
#include "mocks/QueryHandlerMock.h"
#include "qt_gui/presentation/PlannerPresenter.h"
#include "gmock/gmock.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

class PlannerWindowMock
    : public sprint_timer::ui::contracts::PlannerContract::View {
public:
    MOCK_METHOD(void,
                displayPlanner,
                (const sprint_timer::api::TaskTreeDTO&),
                (override));
};

class PlannerPresenterFixture : public ::testing::Test {
public:
    NiceMock<PlannerWindowMock> view;
    NiceMock<
        mocks::QueryHandlerMock<sprint_timer::api::ReadTaskTreeQuery>>
        readPlannerHandler;
    sprint_timer::ui::PlannerPresenter sut{readPlannerHandler};
};

TEST_F(PlannerPresenterFixture, does_nothing_when_view_is_not_attached)
{
    EXPECT_CALL(view, displayPlanner(_)).Times(0);

    sut.updateView();
}

TEST_F(PlannerPresenterFixture, updates_view_when_view_is_attached)
{
    // sprint_timer::api::TaskTreeDTO tree;
    auto matches_tree = [](const sprint_timer::api::TaskTreeDTO& tree) {
        return tree == sprint_timer::api::TaskTreeDTO{};
    };
    ON_CALL(readPlannerHandler,
            handle(sprint_timer::api::ReadTaskTreeQuery{}))
        .WillByDefault(
            Return(::testing::ByMove(sprint_timer::api::TaskTreeDTO{})));

    EXPECT_CALL(view, displayPlanner(::testing::Truly(matches_tree)));

    sut.attachView(view);
}
