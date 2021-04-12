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
#include "core/Observable.h"
#include "core/Observer.h"
#include "mocks/QueryHandlerMock.h"
#include "gmock/gmock.h"
#include <core/QueryHandler.h>
#include <core/use_cases/request_progress/RequestProgressQuery.h>
#include <qt_gui/presentation/ProgressPresenter.h>
#include <string_view>

using sprint_timer::GoalProgress;
using sprint_timer::ProgressOverPeriod;
using namespace sprint_timer::ui::contracts::DailyProgress;

namespace colors {

constexpr std::string_view normalEmptyColor{"#a0a0a4"};
constexpr std::string_view normalFilledColor{"#6baa15"};
constexpr std::string_view overworkEmptyColor{normalFilledColor};
constexpr std::string_view overworkFilledColor{"#ff0000"};
constexpr std::string_view restDayEmptyColor{"#ffa500"};
constexpr std::string_view blancColor{"#ffffff"};

} // namespace colors

namespace sprint_timer::ui::contracts::DailyProgress {

bool operator==(const GaugeValues& lhs, const GaugeValues& rhs)
{
    return std::tie(
               lhs.estimated, lhs.actual, lhs.emptyColor, lhs.filledColor) ==
           std::tie(rhs.estimated, rhs.actual, rhs.emptyColor, rhs.filledColor);
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const GaugeValues& gaugeValues)
{
    os << "GaugeValues{" << gaugeValues.actual << "/" << gaugeValues.estimated
       << " " << gaugeValues.emptyColor << " " << gaugeValues.filledColor
       << "}";
    return os;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const std::vector<GaugeValues>& values)
{
    os << "GaugeValuesVector{";
    for (const auto& element : values) {
        os << element;
        os << "; ";
    }
    return os;
}

bool operator==(const LegendData& lhs, const LegendData& rhs)
{
    return std::tie(lhs.count,
                    lhs.left,
                    lhs.difference,
                    lhs.average,
                    lhs.percentage) == std::tie(rhs.count,
                                                rhs.left,
                                                rhs.difference,
                                                rhs.average,
                                                rhs.percentage);
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const LegendData& ld)
{
    os << "LegendData{" << ld.count << ", " << ld.left << ", " << ld.difference
       << ", " << ld.average << ", " << ld.percentage << "}";
    return os;
}

bool operator==(const ProgressBarData& lhs, const ProgressBarData& rhs)
{
    return std::tie(lhs.actual,
                    lhs.estimated,
                    lhs.emptyColor,
                    lhs.filledColor,
                    lhs.isVisible) == std::tie(rhs.actual,
                                               rhs.estimated,
                                               rhs.emptyColor,
                                               rhs.filledColor,
                                               rhs.isVisible);
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const ProgressBarData& data)
{
    os << "ProgressBarData{" << data.actual << "/" << data.estimated << " "
       << data.emptyColor << " " << data.filledColor << " " << data.isVisible
       << "}";
    return os;
}

} // namespace sprint_timer::ui::contracts::DailyProgress

class ProgressWidgetMock
    : public sprint_timer::ui::contracts::DailyProgress::View {
public:
    MOCK_METHOD(void, displayLegend, (const LegendData&), (override));
    MOCK_METHOD(void,
                displayGauges,
                (const std::vector<GaugeValues>&),
                (override));
    MOCK_METHOD(void, displayProgressBar, (const ProgressBarData&), (override));
};

class ObservableMock : public sprint_timer::Observable {
public:
    MOCK_METHOD(void, attach, (sprint_timer::Observer&), (override));
    MOCK_METHOD(void, detach, (sprint_timer::Observer&), (override));
    MOCK_METHOD(void, notify, (), (override));
};

class ProgressPresenterFixture : public ::testing::Test {
public:
    ::testing::NiceMock<ProgressWidgetMock> viewMock;
    ::testing::NiceMock<
        mocks::QueryHandlerMock<sprint_timer::use_cases::RequestProgressQuery,
                                ProgressOverPeriod>>
        requestProgressHandlerMock;
    ::testing::NiceMock<ObservableMock> observableMock;
};

TEST_F(ProgressPresenterFixture, displays_gauges_with_correct_colors)
{
    using namespace colors;
    const ProgressOverPeriod progress{
        {GoalProgress{GoalProgress::Estimated{0}, GoalProgress::Actual{12}},
         GoalProgress{GoalProgress::Estimated{7}, GoalProgress::Actual{10}},
         GoalProgress{GoalProgress::Estimated{5}, GoalProgress::Actual{0}},
         GoalProgress{GoalProgress::Estimated{0}, GoalProgress::Actual{0}},
         GoalProgress{GoalProgress::Estimated{11}, GoalProgress::Actual{11}}}};
    const std::vector<GaugeValues> expected{{
        GaugeValues{0,
                    12,
                    std::string{restDayEmptyColor},
                    std::string{restDayEmptyColor}},
        GaugeValues{7,
                    10,
                    std::string{overworkEmptyColor},
                    std::string{overworkFilledColor}},
        GaugeValues{
            5, 0, std::string{normalEmptyColor}, std::string{normalEmptyColor}},
        GaugeValues{0,
                    0,
                    std::string{restDayEmptyColor},
                    std::string{restDayEmptyColor}},
        GaugeValues{11,
                    11,
                    std::string{normalFilledColor},
                    std::string{normalFilledColor}},
    }};
    ON_CALL(requestProgressHandlerMock, handle(::testing::_))
        .WillByDefault(::testing::Return(progress));

    EXPECT_CALL(viewMock, displayGauges(expected)).Times(1);

    sprint_timer::ui::ProgressPresenter presenter{requestProgressHandlerMock};
    presenter.attachView(viewMock);
}

TEST_F(ProgressPresenterFixture, displays_legend_for_empty_progress)
{
    const ProgressOverPeriod progress{{
        GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{0}},
        GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{0}},
        GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{0}},
    }};
    const LegendData expected{
        "0/30", "Left to complete:", "30", "0.00", "0.00%"};
    ON_CALL(requestProgressHandlerMock, handle(::testing::_))
        .WillByDefault(::testing::Return(progress));

    EXPECT_CALL(viewMock, displayLegend(expected)).Times(1);

    sprint_timer::ui::ProgressPresenter presenter{requestProgressHandlerMock};
    presenter.attachView(viewMock);
}

TEST_F(ProgressPresenterFixture, displays_legend_for_underwork_progress)
{
    const ProgressOverPeriod progress{{
        GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{5}},
        GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{3}},
        GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{7}},
    }};
    const LegendData expected{
        "15/30", "Left to complete:", "15", "5.00", "50.00%"};
    ON_CALL(requestProgressHandlerMock, handle(::testing::_))
        .WillByDefault(::testing::Return(progress));

    EXPECT_CALL(viewMock, displayLegend(expected)).Times(1);

    sprint_timer::ui::ProgressPresenter presenter{requestProgressHandlerMock};
    presenter.attachView(viewMock);
}

TEST_F(ProgressPresenterFixture, displays_legend_for_overwork_progress)
{
    const ProgressOverPeriod progress{{
        GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{10}},
        GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{20}},
        GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{15}},
    }};
    const LegendData expected{"45/30", "Overwork:", "15", "15.00", "150.00%"};
    ON_CALL(requestProgressHandlerMock, handle(::testing::_))
        .WillByDefault(::testing::Return(progress));

    EXPECT_CALL(viewMock, displayLegend(expected)).Times(1);

    sprint_timer::ui::ProgressPresenter presenter{requestProgressHandlerMock};
    presenter.attachView(viewMock);
}

TEST_F(ProgressPresenterFixture, displays_progress_bar_for_empty_progress)
{
    const ProgressOverPeriod progress{{}};
    // TODO it is a bit lazy - we actually do not care what colors are returned
    // when bar is hidden; test should reflect that
    const ProgressBarData expected{0,
                                   0,
                                   std::string{colors::blancColor},
                                   std::string{colors::blancColor},
                                   false};
    ON_CALL(requestProgressHandlerMock, handle(::testing::_))
        .WillByDefault(::testing::Return(progress));

    EXPECT_CALL(viewMock, displayProgressBar(expected)).Times(1);

    sprint_timer::ui::ProgressPresenter presenter{requestProgressHandlerMock};
    presenter.attachView(viewMock);
}

TEST_F(ProgressPresenterFixture, displays_progress_bar_underwork_progress)
{
    const ProgressOverPeriod progress{
        {GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{0}},
         GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{0}},
         GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{4}}}};
    const ProgressBarData expected{10,
                                   4,
                                   std::string{colors::blancColor},
                                   std::string{colors::normalEmptyColor},
                                   true};
    ON_CALL(requestProgressHandlerMock, handle(::testing::_))
        .WillByDefault(::testing::Return(progress));

    EXPECT_CALL(viewMock, displayProgressBar(expected)).Times(1);

    sprint_timer::ui::ProgressPresenter presenter{requestProgressHandlerMock};
    presenter.attachView(viewMock);
}

TEST_F(ProgressPresenterFixture,
       display_progress_bar_hides_bar_when_last_bin_estimation_is_zero)
{
    const ProgressOverPeriod progress{
        {GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{0}},
         GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{0}},
         GoalProgress{GoalProgress::Estimated{0}, GoalProgress::Actual{0}}}};
    // TODO it is a bit lazy - we actually do not care what colors are returned
    // when bar is hidden; test should reflect that
    const ProgressBarData expected{0,
                                   0,
                                   std::string{colors::blancColor},
                                   std::string{colors::normalFilledColor},
                                   false};
    ON_CALL(requestProgressHandlerMock, handle(::testing::_))
        .WillByDefault(::testing::Return(progress));

    EXPECT_CALL(viewMock, displayProgressBar(expected)).Times(1);

    sprint_timer::ui::ProgressPresenter presenter{requestProgressHandlerMock};
    presenter.attachView(viewMock);
}

TEST_F(ProgressPresenterFixture, displays_progress_bar_for_overwork_progress)
{
    const ProgressOverPeriod progress{
        {GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{0}},
         GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{0}},
         GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{14}}}};
    const ProgressBarData expected{10,
                                   14,
                                   std::string{colors::normalFilledColor},
                                   std::string{colors::overworkFilledColor},
                                   true};
    ON_CALL(requestProgressHandlerMock, handle(::testing::_))
        .WillByDefault(::testing::Return(progress));

    EXPECT_CALL(viewMock, displayProgressBar(expected)).Times(1);

    sprint_timer::ui::ProgressPresenter presenter{requestProgressHandlerMock};
    presenter.attachView(viewMock);
}

TEST_F(ProgressPresenterFixture, displays_progress_bar_when_goal_is_met)
{
    const ProgressOverPeriod progress{
        {GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{0}},
         GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{0}},
         GoalProgress{GoalProgress::Estimated{10}, GoalProgress::Actual{10}}}};
    const ProgressBarData expected{10,
                                   10,
                                   std::string{colors::blancColor},
                                   std::string{colors::normalFilledColor},
                                   true};
    ON_CALL(requestProgressHandlerMock, handle(::testing::_))
        .WillByDefault(::testing::Return(progress));

    EXPECT_CALL(viewMock, displayProgressBar(expected)).Times(1);

    sprint_timer::ui::ProgressPresenter presenter{requestProgressHandlerMock};
    presenter.attachView(viewMock);
}
