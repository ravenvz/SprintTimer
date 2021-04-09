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
#include <qt_gui/presentation/TodayProgressPresenter.h>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

using sprint_timer::ProgressOverPeriod;
using sprint_timer::use_cases::RequestProgressQuery;

namespace {

const std::string normalStyle{"QLabel { color: black; }"};
const std::string overworkStyle{"QLabel { color: red; }"};
const std::string matchStyle{"QLabel { color: green; }"};

} // namespace

class TodayProgressViewMock
    : public sprint_timer::ui::contracts::TodayProgressContract::View {
public:
    MOCK_METHOD(void,
                displayProgress,
                (const std::string&, const std::string&),
                (override));
};

class TodayProgressPresenterFixture : public ::testing::Test {
public:
    NiceMock<mocks::QueryHandlerMock<RequestProgressQuery, ProgressOverPeriod>>
        requestProgressHandler;
    TodayProgressViewMock view;
    sprint_timer::ui::TodayProgressPresenter sut{requestProgressHandler};
};

TEST_F(TodayProgressPresenterFixture,
       displays_progress_in_normal_style_when_underwork)
{
    using namespace sprint_timer;
    ProgressOverPeriod progress{
        {GoalProgress{GoalProgress::Estimated{12}, GoalProgress::Actual{3}}}};
    ON_CALL(requestProgressHandler, handle(_)).WillByDefault(Return(progress));

    EXPECT_CALL(view,
                displayProgress("Daily goal progress: 3/12", normalStyle));

    sut.attachView(view);
}

TEST_F(TodayProgressPresenterFixture,
       displays_progress_in_overwork_style_when_overwork)
{
    using namespace sprint_timer;
    ProgressOverPeriod progress{
        {GoalProgress{GoalProgress::Estimated{12}, GoalProgress::Actual{17}}}};
    ON_CALL(requestProgressHandler, handle(_)).WillByDefault(Return(progress));

    EXPECT_CALL(view,
                displayProgress("Daily goal progress: 17/12", overworkStyle));

    sut.attachView(view);
}

TEST_F(TodayProgressPresenterFixture,
       displays_progress_in_match_style_when_goal_matched_exactly)
{
    using namespace sprint_timer;
    ProgressOverPeriod progress{
        {GoalProgress{GoalProgress::Estimated{12}, GoalProgress::Actual{12}}}};
    ON_CALL(requestProgressHandler, handle(_)).WillByDefault(Return(progress));

    EXPECT_CALL(view,
                displayProgress("Daily goal progress: 12/12", matchStyle));

    sut.attachView(view);
}

TEST_F(TodayProgressPresenterFixture,
       displays_empty_progress_for_non_working_day)
{
    using namespace sprint_timer;
    ProgressOverPeriod progress{
        {GoalProgress{GoalProgress::Estimated{0}, GoalProgress::Actual{12}}}};
    ON_CALL(requestProgressHandler, handle(_)).WillByDefault(Return(progress));

    EXPECT_CALL(view, displayProgress("", ""));

    sut.attachView(view);
}

