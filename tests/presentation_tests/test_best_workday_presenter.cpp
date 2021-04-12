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
#include "mocks/StatisticsMediatorMock.h"
#include <core/SprintBuilder.h>
#include <qt_gui/presentation/BestWorkdayContract.h>
#include <qt_gui/presentation/BestWorkdayPresenter.h>

using sprint_timer::Distribution;
using sprint_timer::entities::Sprint;
using sprint_timer::ui::BestWorkdayPresenter;
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
    return std::tie(
               lhs.barColor, lhs.borderColor, lhs.barValues, lhs.dayOrder) ==
           std::tie(rhs.barColor, rhs.borderColor, rhs.barValues, rhs.dayOrder);
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
              std::ostream_iterator<typename std::iterator_traits<decltype(
                  cbegin(data.barValues))>::value_type>(os, ", "));
    os << " dayOrder: ";
    std::copy(cbegin(data.dayOrder),
              cend(data.dayOrder),
              std::ostream_iterator<typename std::iterator_traits<decltype(
                  cbegin(data.dayOrder))>::value_type>(os, ", "));
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
    const size_t numTopTags{20};
    NiceMock<mocks::StatisticsMediatorMock> mediator_mock;
    const DateRange someDateRange{dw::current_date(), dw::current_date()};
    const DateRange specificDateRange{Date{Year{2015}, Month{6}, Day{1}},
                                      Date{Year{2015}, Month{6}, Day{14}}};
    NiceMock<BestWorkdayViewMock> view;

    std::vector<Sprint> buildSprintsFixture()
    {
        using namespace dw;
        std::vector<Sprint> sprints;
        sprint_timer::SprintBuilder builder;
        builder.withTaskUuid("123");
        // (2015, 6, 1) is Monday, so each weekday occures exactly twice
        // in 14-day timeSpan
        // {Date{Year{2015}, Month{6}, Day{1}},
        // Date{Year{2015}, Month{6}, Day{14}}};
        // This generates workday distribution
        // {4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5}
        for (unsigned i = 1; i < 15; ++i) {
            for (unsigned j = 0; j < i; ++j) {
                const DateTime sprintDateTime =
                    DateTime{Date{Year{2015}, Month{6}, Day{i}}};
                sprints.push_back(builder
                                      .withTimeSpan(DateTimeRange{
                                          sprintDateTime, sprintDateTime})
                                      .build());
            }
        }
        return sprints;
    }
};

TEST_F(BestWorkdayPresenterFixture,
       updates_legend_with_placeholder_data_when_no_date_range_is_supplied)
{
    const Distribution<double> distribution{
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}};
    const View::LegendData expected{-1, "No data"};
    ON_CALL(mediator_mock, range()).WillByDefault(Return(std::nullopt));
    BestWorkdayPresenter sut{mediator_mock, dw::Weekday::Monday};
    sut.attachView(view);

    EXPECT_CALL(view, displayLegend(expected)).Times(1);

    sut.updateView();
}

TEST_F(BestWorkdayPresenterFixture,
       updates_placeholder_legend_when_there_are_no_sprints)
{
    const View::LegendData expected{-1, "No data"};
    const std::vector<Sprint> sprints;
    ON_CALL(mediator_mock, range()).WillByDefault(Return(specificDateRange));
    ON_CALL(mediator_mock, sprints()).WillByDefault(ReturnRef(sprints));
    BestWorkdayPresenter sut{mediator_mock, dw::Weekday::Monday};
    sut.attachView(view);

    EXPECT_CALL(view, displayLegend(expected));

    sut.updateView();
}

TEST_F(BestWorkdayPresenterFixture, updates_legend_for_generic_data)
{
    const auto sprints = buildSprintsFixture();
    const View::LegendData expected{7, "40"};
    BestWorkdayPresenter sut{mediator_mock, dw::Weekday::Monday};
    ON_CALL(mediator_mock, range()).WillByDefault(Return(specificDateRange));
    ON_CALL(mediator_mock, sprints()).WillByDefault(ReturnRef(sprints));
    sut.attachView(view);

    EXPECT_CALL(view, displayLegend(expected)).Times(1);

    sut.updateView();
}

TEST_F(BestWorkdayPresenterFixture,
       requeries_mediator_when_shared_data_is_changed)
{
    using sprint_timer::entities::Tag;
    const dw::DateRange newDateRange{dw::current_date(), dw::current_date()};
    BestWorkdayPresenter sut{mediator_mock, dw::Weekday::Monday};
    const std::vector<Sprint> stubSprints;
    ON_CALL(mediator_mock, range()).WillByDefault(Return(specificDateRange));
    ON_CALL(mediator_mock, sprints()).WillByDefault(ReturnRef(stubSprints));
    sut.attachView(view);

    EXPECT_CALL(view, displayLegend(_));
    EXPECT_CALL(view, displayBars(_));

    sut.onSharedDataChanged();
}

TEST_F(BestWorkdayPresenterFixture, updates_bars_when_there_are_no_sprints)
{
    const View::BarD expected{std::string{barBorderColor},
                              std::string{barColor},
                              {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                              {1, 2, 3, 4, 5, 6, 7}};
    BestWorkdayPresenter sut{mediator_mock, dw::Weekday::Monday};
    const std::vector<Sprint> sprints;
    ON_CALL(mediator_mock, range()).WillByDefault(Return(specificDateRange));
    ON_CALL(mediator_mock, sprints()).WillByDefault(ReturnRef(sprints));

    EXPECT_CALL(view, displayBars(expected));

    sut.attachView(view);
}

TEST_F(BestWorkdayPresenterFixture, updates_bar_for_generic_data)
{
    const auto sprints = buildSprintsFixture();
    const View::BarD expected{std::string{barBorderColor},
                              std::string{barColor},
                              {4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5},
                              {1, 2, 3, 4, 5, 6, 7}};
    ON_CALL(mediator_mock, range()).WillByDefault(Return(specificDateRange));
    ON_CALL(mediator_mock, sprints()).WillByDefault(ReturnRef(sprints));
    BestWorkdayPresenter sut{mediator_mock, dw::Weekday::Monday};

    EXPECT_CALL(view, displayBars(expected));

    sut.attachView(view);
}

TEST_F(BestWorkdayPresenterFixture,
       updates_bar_for_generic_data_when_first_day_of_week_is_sunday)
{
    const auto sprints = buildSprintsFixture();
    const View::BarD expected{std::string{barBorderColor},
                              std::string{barColor},
                              {10.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5},
                              {7, 1, 2, 3, 4, 5, 6}};
    ON_CALL(mediator_mock, range()).WillByDefault(Return(specificDateRange));
    ON_CALL(mediator_mock, sprints()).WillByDefault(ReturnRef(sprints));
    BestWorkdayPresenter sut{mediator_mock, dw::Weekday::Sunday};

    EXPECT_CALL(view, displayBars(expected));

    sut.attachView(view);
}
