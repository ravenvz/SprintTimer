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
#include "mocks/StatisticsColleagueMock.h"
#include "qt_gui/presentation/DailyStatisticsGraphPresenter.h"
#include <string_view>

using sprint_timer::ui::DailyStatisticsGraphPresenter;
using sprint_timer::ui::StatisticsContext;
using sprint_timer::ui::contracts::DailyStatisticGraphContract::GraphData;
using sprint_timer::ui::contracts::DailyStatisticGraphContract::LegendData;
using ::testing::Return;
using ::testing::ReturnRef;
using namespace ::dw;

namespace constants {

constexpr std::string_view dailyGraphColor{"#f63c0d"};
constexpr std::string_view averageColor{"#3949c4"};
constexpr std::string_view goalColor{"#39c473"};
constexpr std::string_view pointColor{"#ffffff"};
constexpr double penWidthF{2.2};

} // namespace constants

namespace sprint_timer::ui::contracts::DailyStatisticGraphContract {

bool operator==(const LegendData& lhs, const LegendData& rhs)
{
    return lhs.total == rhs.total && lhs.average == rhs.average;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const LegendData& data)
{
    os << "LegendData{total: " << data.total << ", average: " << data.average
       << "}";
    return os;
}

bool operator==(const GraphOptions& lhs, const GraphOptions& rhs)
{
    return lhs.penColor == rhs.penColor &&
           std::abs(lhs.penWidth - rhs.penWidth) < 0.001 &&
           lhs.showPoints == rhs.showPoints &&
           lhs.pointColor == rhs.pointColor && lhs.style == rhs.style;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const GraphOptions& options)
{
    os << "GraphOptions{"
       << "\n\t"
       << "Pen color: " << options.penColor << "\n\t"
       << "Pen width: " << options.penWidth << "\n\t"
       << "Points visible: " << options.showPoints << "\n\t"
       << "Point color: " << options.pointColor << "\n\t"
       << "LineStyle: " << static_cast<int>(options.style) << "\n}";
    return os;
}

bool operator==(const DayNumber& lhs, const DayNumber& rhs)
{
    return lhs.value == rhs.value;
}

bool operator==(const Value& lhs, const Value& rhs)
{
    return std::abs(lhs.value - rhs.value) < 0.001;
}

bool operator==(const GraphValue& lhs, const GraphValue& rhs)
{
    return lhs.xValue == rhs.xValue && lhs.yValue == rhs.yValue &&
           lhs.label == rhs.label;
}

bool operator==(const std::pair<DayNumber, Value>& lhs,
                const std::pair<DayNumber, Value>& rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

bool operator==(const GraphData& lhs, const GraphData& rhs)
{
    return lhs.options == rhs.options && lhs.values == rhs.values;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const GraphData& data)
{
    os << "\nGraphData{"
       << "\n\t" << data.options;
    for (const auto& element : data.values) {
        os << "[" << element.xValue << ", " << element.yValue << ", "
           << element.label << "], ";
    }
    os << "\n}";
    return os;
}

template <class CharT, class Traits, class WrappedType>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const WrappedType& type)
{
    os << "WrappedType{" << type.value << "}";
    return os;
}

} // namespace sprint_timer::ui::contracts::DailyStatisticGraphContract

class DailyStatisticsViewMock
    : public sprint_timer::ui::contracts::DailyStatisticGraphContract::View {
public:
    MOCK_METHOD(void, updateLegend, (const LegendData&), (override));
    MOCK_METHOD(void, drawGraph, (const GraphData&), (override));
    MOCK_METHOD(void, clearGraphs, (), (override));
};

class DailyStatisticsSharedDataFetcherFixture : public ::testing::Test {
public:
    sprint_timer::ui::StatisticsMediator mediator;
    ::testing::NiceMock<DailyStatisticsViewMock> viewMock;
    ::testing::NiceMock<
        mocks::QueryHandlerMock<sprint_timer::api::DailyStatisticsQuery>>
        dailyStatisticsHandler;
    DateRange someDateRange{Date{Year{2020}, Month{2}, Day{1}},
                            Date{Year{2020}, Month{2}, Day{5}}};
    size_t someNumTopTags{5};
    sprint_timer::ui::StatisticsContext statisticsContext{
        someNumTopTags, someDateRange, std::nullopt};
    sprint_timer::ui::DailyStatisticsGraphPresenter sut{
        dailyStatisticsHandler, mediator, statisticsContext};
};

TEST_F(DailyStatisticsSharedDataFetcherFixture,
       sets_placeholder_data_when_not_provided_date_range)
{
    using sprint_timer::api::DailyStatisticsDTO;
    using namespace sprint_timer::ui::contracts::DailyStatisticGraphContract;
    const GraphOptions expectedDailyOptions{
        constants::penWidthF,
        std::string{constants::dailyGraphColor},
        std::string{constants::pointColor},
        true,
        LineStyle::Solid};
    const GraphData expectedDaily{expectedDailyOptions,
                                  {GraphValue{Value{0}, Value{0}, "1"},
                                   GraphValue{Value{1}, Value{0}, "2"},
                                   GraphValue{Value{2}, Value{0}, "3"},
                                   GraphValue{Value{3}, Value{0}, "4"},
                                   GraphValue{Value{4}, Value{0}, "5"}}};
    const GraphOptions expectedAverageOptions{constants::penWidthF,
                                              std::string{constants::goalColor},
                                              "",
                                              false,
                                              LineStyle::Dash};
    const GraphData expectedAverage{expectedAverageOptions,
                                    {GraphValue{Value{0}, Value{0}, ""},
                                     GraphValue{Value{4}, Value{0}, ""}}};
    const GraphOptions actualAverageOptions{
        constants::penWidthF,
        std::string{constants::averageColor},
        "",
        false,
        LineStyle::Solid};
    const GraphData actualAverage{actualAverageOptions,
                                  {GraphValue{Value{0}, Value{0}, ""},
                                   GraphValue{Value{4}, Value{0}, ""}}};
    mocks::given_handler_returns(
        dailyStatisticsHandler,
        DailyStatisticsDTO{0.0, 0.0, 0, {0, 0, 0, 0, 0}});
    EXPECT_CALL(viewMock, updateLegend(LegendData{"0", "0.00"}));
    EXPECT_CALL(viewMock, drawGraph(expectedDaily));
    EXPECT_CALL(viewMock, drawGraph(expectedAverage));
    EXPECT_CALL(viewMock, drawGraph(actualAverage));

    sut.attachView(viewMock);
}

TEST_F(DailyStatisticsSharedDataFetcherFixture,
       updates_legend_with_generic_data)
{
    using sprint_timer::api::DailyStatisticsDTO;
    const LegendData expected{"90", "77.70"};
    const DailyStatisticsDTO dailyStatistics{77.7, 55.5, 90, {20, 0, 12, 22}};
    mocks::given_handler_returns(dailyStatisticsHandler, dailyStatistics);

    EXPECT_CALL(viewMock, updateLegend(expected));

    sut.attachView(viewMock);
}

TEST_F(DailyStatisticsSharedDataFetcherFixture,
       updates_legend_when_shared_data_changed)
{
    using sprint_timer::api::DailyStatisticsDTO;
    const LegendData expected{"90", "77.70"};
    sut.attachView(viewMock);
    const DailyStatisticsDTO dailyStatistics{77.7, 55.5, 90, {20, 0, 12, 22}};
    mocks::given_handler_returns(dailyStatisticsHandler, dailyStatistics);

    EXPECT_CALL(viewMock, updateLegend(expected));

    sut.onSharedDataChanged();
}

TEST_F(DailyStatisticsSharedDataFetcherFixture, updates_graph_with_generic_data)
{
    using namespace sprint_timer::ui::contracts::DailyStatisticGraphContract;
    using sprint_timer::api::DailyStatisticsDTO;
    const GraphOptions expectedDailyOptions{
        constants::penWidthF,
        std::string{constants::dailyGraphColor},
        std::string{constants::pointColor},
        true,
        LineStyle::Solid};
    const GraphData expectedDaily{expectedDailyOptions,
                                  {GraphValue{Value{0}, Value{2}, "1"},
                                   GraphValue{Value{1}, Value{12}, "2"},
                                   GraphValue{Value{2}, Value{15}, "3"},
                                   GraphValue{Value{3}, Value{0}, "4"},
                                   GraphValue{Value{4}, Value{10}, "5"}}};
    const GraphOptions expectedAverageOptions{constants::penWidthF,
                                              std::string{constants::goalColor},
                                              "",
                                              false,
                                              LineStyle::Dash};
    const GraphData expectedAverage{expectedAverageOptions,
                                    {GraphValue{Value{0}, Value{12}, ""},
                                     GraphValue{Value{4}, Value{12}, ""}}};
    const GraphOptions actualAverageOptions{
        constants::penWidthF,
        std::string{constants::averageColor},
        "",
        false,
        LineStyle::Solid};
    const GraphData actualAverage{actualAverageOptions,
                                  {GraphValue{Value{0}, Value{19.5}, ""},
                                   GraphValue{Value{4}, Value{19.5}, ""}}};
    mocks::given_handler_returns(
        dailyStatisticsHandler,
        DailyStatisticsDTO{19.5, 12.0, 39, {2, 12, 15, 0, 10}});

    EXPECT_CALL(viewMock, clearGraphs());
    EXPECT_CALL(viewMock, drawGraph(expectedDaily));
    EXPECT_CALL(viewMock, drawGraph(expectedAverage));
    EXPECT_CALL(viewMock, drawGraph(actualAverage));

    sut.attachView(viewMock);
}

TEST_F(DailyStatisticsSharedDataFetcherFixture,
       updates_graph_when_shared_data_changed)
{
    using namespace sprint_timer::ui::contracts::DailyStatisticGraphContract;
    using sprint_timer::api::DailyStatisticsDTO;
    const GraphOptions expectedDailyOptions{
        constants::penWidthF,
        std::string{constants::dailyGraphColor},
        std::string{constants::pointColor},
        true,
        LineStyle::Solid};
    const GraphData expectedDaily{expectedDailyOptions,
                                  {GraphValue{Value{0}, Value{2}, "1"},
                                   GraphValue{Value{1}, Value{12}, "2"},
                                   GraphValue{Value{2}, Value{15}, "3"},
                                   GraphValue{Value{3}, Value{0}, "4"},
                                   GraphValue{Value{4}, Value{10}, "5"}}};
    const GraphOptions expectedAverageOptions{constants::penWidthF,
                                              std::string{constants::goalColor},
                                              "",
                                              false,
                                              LineStyle::Dash};
    const GraphData expectedAverage{expectedAverageOptions,
                                    {GraphValue{Value{0}, Value{12}, ""},
                                     GraphValue{Value{4}, Value{12}, ""}}};
    const GraphOptions actualAverageOptions{
        constants::penWidthF,
        std::string{constants::averageColor},
        "",
        false,
        LineStyle::Solid};
    const GraphData actualAverage{actualAverageOptions,
                                  {GraphValue{Value{0}, Value{19.5}, ""},
                                   GraphValue{Value{4}, Value{19.5}, ""}}};
    sut.attachView(viewMock);
    mocks::given_handler_returns(
        dailyStatisticsHandler,
        DailyStatisticsDTO{19.5, 12.0, 39, {2, 12, 15, 0, 10}});

    EXPECT_CALL(viewMock, clearGraphs());
    EXPECT_CALL(viewMock, drawGraph(expectedDaily));
    EXPECT_CALL(viewMock, drawGraph(expectedAverage));
    EXPECT_CALL(viewMock, drawGraph(actualAverage));

    sut.onSharedDataChanged();
}
