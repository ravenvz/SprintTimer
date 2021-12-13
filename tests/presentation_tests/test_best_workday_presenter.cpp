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
#include "mocks/StatisticsMediatorMock.h"
#include "qt_gui/presentation/BestWorkdayContract.h"
#include "qt_gui/presentation/BestWorkdayPresenter.h"

using sprint_timer::ui::BestWorkdayPresenter;
using sprint_timer::ui::StatisticsContext;
using sprint_timer::ui::contracts::BestWorkday::View;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using namespace dw;

namespace {

constexpr std::string_view barBorderColor{"#f63c0d"};
constexpr std::string_view barColor{"#73c245"};

} // namespace

namespace sprint_timer::ui::contracts::BestWorkday {

bool operator==(const View::LegendData& lhs, const View::LegendData& rhs)
{
    return std::tie(lhs.dayNum, lhs.percentage) ==
           std::tie(rhs.dayNum, rhs.percentage);
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const View::LegendData& legendData)
{
    os << "LegendData{" << legendData.dayNum << " " << legendData.percentage
       << "}";
    return os;
}

bool operator==(const View::BarD& lhs, const View::BarD& rhs)
{
    return lhs.barColor == rhs.barColor && lhs.borderColor == rhs.borderColor &&
           std::equal(cbegin(lhs.barValues),
                      cend(lhs.barValues),
                      cbegin(rhs.barValues),
                      cend(rhs.barValues)) &&
           std::equal(cbegin(lhs.dayOrder),
                      cend(lhs.dayOrder),
                      cbegin(rhs.dayOrder),
                      cend(rhs.dayOrder));
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const View::BarD& data)
{
    os << "BarData{"
       << "barColor: " << data.barColor << ", borderColor: " << data.borderColor
       << ", values: ";
    std::copy(cbegin(data.barValues),
              cend(data.barValues),
              std::ostream_iterator<typename std::iterator_traits<
                  decltype(cbegin(data.barValues))>::value_type>(os, ", "));
    os << " dayOrder: ";
    std::copy(cbegin(data.dayOrder),
              cend(data.dayOrder),
              std::ostream_iterator<typename std::iterator_traits<
                  decltype(cbegin(data.dayOrder))>::value_type>(os, ", "));
    os << "}";
    return os;
}

} // namespace sprint_timer::ui::contracts::BestWorkday

namespace sprint_timer::use_cases {

} // namespace sprint_timer::use_cases

class BestWorkdayViewMock : public View {
public:
    MOCK_METHOD(void, displayLegend, (const View::LegendData&), (override));
    MOCK_METHOD(void, displayBars, (const View::BarD&), (override));
};

class BestWorkdayPresenterFixture : public ::testing::Test {
public:
    sprint_timer::ui::StatisticsMediator mediator;
    const DateRange someDateRange{dw::current_date(), dw::current_date()};
    NiceMock<BestWorkdayViewMock> view;
    NiceMock<mocks::QueryHandlerMock<
        sprint_timer::use_cases::WorkdayStatisticsQuery>>
        workdayStatisticsHandlerMock;
};

TEST_F(BestWorkdayPresenterFixture,
       updates_legend_with_placeholder_data_when_no_date_range_is_supplied)
{
    const View::LegendData expected{-1, "No data"};
    StatisticsContext statisticsContext;
    BestWorkdayPresenter sut{workdayStatisticsHandlerMock,
                             mediator,
                             statisticsContext,
                             dw::Weekday::Monday};
    sut.attachView(view);

    EXPECT_CALL(workdayStatisticsHandlerMock, handle(_)).Times(0);
    EXPECT_CALL(view, displayLegend(expected)).Times(1);

    sut.updateView();
}

TEST_F(BestWorkdayPresenterFixture,
       updates_legend_with_placeholder_data_when_distribution_empty)
{
    const View::LegendData expected{-1, "No data"};
    StatisticsContext statisticsContext{
        dw::DateRange{dw::current_date(), dw::current_date()}};
    BestWorkdayPresenter sut{workdayStatisticsHandlerMock,
                             mediator,
                             statisticsContext,
                             dw::Weekday::Monday};
    sut.attachView(view);
    ON_CALL(workdayStatisticsHandlerMock, handle(_))
        .WillByDefault(Return(std::nullopt));

    EXPECT_CALL(view, displayLegend(expected));

    sut.updateView();
}

TEST_F(BestWorkdayPresenterFixture, updates_legend_with_generic_data)
{
    const View::LegendData expected{2, "63"};
    const StatisticsContext statisticsContext{someDateRange};
    BestWorkdayPresenter sut{workdayStatisticsHandlerMock,
                             mediator,
                             statisticsContext,
                             dw::Weekday::Monday};
    sprint_timer::use_cases::WorkdayStatisticsDTO workdayStatistics{
        {2.0, 3.5, 2.0, 3.0, 1.0, 3.0, 0.5}, dw::Weekday::Tuesday, 63};
    ON_CALL(workdayStatisticsHandlerMock, handle(_))
        .WillByDefault(Return(workdayStatistics));

    EXPECT_CALL(view, displayLegend(expected)).Times(1);

    sut.attachView(view);
}

TEST_F(BestWorkdayPresenterFixture, updates_legend_when_date_range_is_changed)
{
    const View::LegendData expected{2, "63"};
    const StatisticsContext statisticsContext{someDateRange};
    BestWorkdayPresenter sut{workdayStatisticsHandlerMock,
                             mediator,
                             statisticsContext,
                             dw::Weekday::Monday};
    sut.attachView(view);
    sprint_timer::use_cases::WorkdayStatisticsDTO workdayStatistics{
        {2.0, 3.5, 2.0, 3.0, 1.0, 3.0, 0.5}, dw::Weekday::Tuesday, 63};
    ON_CALL(workdayStatisticsHandlerMock, handle(_))
        .WillByDefault(Return(workdayStatistics));

    EXPECT_CALL(view, displayLegend(expected)).Times(1);

    sut.onSharedDataChanged();
}

TEST_F(BestWorkdayPresenterFixture, updates_bars_when_distribution_is_empty)
{
    const std::array<double, 7> barValues{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    const std::array<int, 7> dayOrder{1, 2, 3, 4, 5, 6, 7};
    const View::BarD expected{std::string{barBorderColor},
                              std::string{barColor},
                              barValues,
                              dayOrder};
    const StatisticsContext statisticsContext{someDateRange};
    ON_CALL(workdayStatisticsHandlerMock, handle(_))
        .WillByDefault(Return(sprint_timer::use_cases::WorkdayStatisticsDTO{}));
    BestWorkdayPresenter sut{workdayStatisticsHandlerMock,
                             mediator,
                             statisticsContext,
                             dw::Weekday::Monday};

    EXPECT_CALL(view, displayBars(expected));

    sut.attachView(view);
}

TEST_F(BestWorkdayPresenterFixture, updates_bar_with_generic_data)
{
    const std::array<double, 7> barValues{4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5};
    const std::array<int, 7> dayOrder{1, 2, 3, 4, 5, 6, 7};
    const View::BarD expected{std::string{barBorderColor},
                              std::string{barColor},
                              barValues,
                              dayOrder};
    const StatisticsContext statisticsContext{someDateRange};
    BestWorkdayPresenter sut{workdayStatisticsHandlerMock,
                             mediator,
                             statisticsContext,
                             dw::Weekday::Monday};
    ON_CALL(workdayStatisticsHandlerMock, handle(_))
        .WillByDefault(Return(sprint_timer::use_cases::WorkdayStatisticsDTO{
            {4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5}, dw::Weekday::Sunday, 40}));

    EXPECT_CALL(view, displayBars(expected));

    sut.attachView(view);
}

TEST_F(BestWorkdayPresenterFixture, updates_bar_when_date_range_is_changed)
{
    const std::array<double, 7> barValues{4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5};
    const std::array<int, 7> dayOrder{1, 2, 3, 4, 5, 6, 7};
    const View::BarD expected{std::string{barBorderColor},
                              std::string{barColor},
                              barValues,
                              dayOrder};
    const StatisticsContext statisticsContext{someDateRange};
    BestWorkdayPresenter sut{workdayStatisticsHandlerMock,
                             mediator,
                             statisticsContext,
                             dw::Weekday::Monday};
    sut.attachView(view);
    ON_CALL(workdayStatisticsHandlerMock, handle(_))
        .WillByDefault(Return(sprint_timer::use_cases::WorkdayStatisticsDTO{
            {4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5}, dw::Weekday::Sunday, 40}));

    EXPECT_CALL(view, displayBars(expected));

    sut.onSharedDataChanged();
}

TEST_F(BestWorkdayPresenterFixture,
       updates_bar_for_generic_data_when_first_day_of_week_is_sunday)
{
    const std::array<double, 7> barValues{10.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5};
    const std::array<int, 7> dayOrder{7, 1, 2, 3, 4, 5, 6};
    const View::BarD expected{std::string{barBorderColor},
                              std::string{barColor},
                              barValues,
                              dayOrder};
    const StatisticsContext statisticsContext{someDateRange};
    BestWorkdayPresenter sut{workdayStatisticsHandlerMock,
                             mediator,
                             statisticsContext,
                             dw::Weekday::Sunday};
    ON_CALL(workdayStatisticsHandlerMock, handle(_))
        .WillByDefault(Return(sprint_timer::use_cases::WorkdayStatisticsDTO{
            {4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5}, dw::Weekday::Sunday, 40}));

    EXPECT_CALL(view, displayBars(expected));

    sut.attachView(view);
}
