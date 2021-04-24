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
#include "mocks/CommandHandlerMock.h"
#include "mocks/QueryHandlerMock.h"
#include "qt_gui/presentation/TodaySprintsPresenter.h"

using namespace ::testing;
using namespace sprint_timer;

namespace {

std::vector<use_cases::SprintDTO> makeSomeSprints();

} // namespace

class SprintOutlineViewMock : public ui::contracts::TodaySprints::View {
public:
    MOCK_METHOD(void,
                displaySprints,
                (const std::vector<use_cases::SprintDTO>&),
                (override));
};

class TodaySprintsPresenterFixture : public ::testing::Test {
public:
    NiceMock<mocks::QueryHandlerMock<use_cases::RequestSprintsQuery>>
        requestSprintsHandler;
    NiceMock<mocks::CommandHandlerMock<use_cases::DeleteSprintCommand>>
        deleteSprintHandler;
    NiceMock<SprintOutlineViewMock> viewMock;
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
    const std::string someUuid{"123"};

    EXPECT_CALL(deleteSprintHandler,
                handle(use_cases::DeleteSprintCommand{someUuid}));

    presenter.onSprintDelete(someUuid);
}

namespace {

std::vector<use_cases::SprintDTO> makeSomeSprints()
{
    using namespace dw;
    using namespace std::chrono_literals;
    const Date someDate{Year{2020}, Month{7}, Day{6}};
    const DateTime someDateTime{someDate};
    const DateTimeRange someRange{someDateTime, someDateTime + 25min};

    std::vector<use_cases::SprintDTO> dtos{
        use_cases::SprintDTO{"1", "1", "Task 1", {"Tag1", "Tag2"}, someRange},
        use_cases::SprintDTO{
            "2", "1", "Task 1", {"Tag1", "Tag2"}, add_offset(someRange, 25min)},
        use_cases::SprintDTO{
            "3", "2", "Task 2", {"Tag3"}, add_offset(someRange, 2h)}};

    return dtos;
}

} // namespace
