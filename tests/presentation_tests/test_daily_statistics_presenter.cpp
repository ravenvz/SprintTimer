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
#include "common_utils/FakeUuidGenerator.h"
#include "core/use_cases/request_sprints/RequestSprintsQuery.h"
#include "mocks/QueryHandlerMock.h"
#include "mocks/StatisticsColleagueMock.h"
#include "qt_gui/presentation/DailyStatisticsGraphPresenter.h"
#include <string_view>

using sprint_timer::ui::DailyStatisticsGraphPresenter;
using sprint_timer::ui::StatisticsContext;
using sprint_timer::ui::contracts::DailyStatisticGraphContract::GraphData;
using sprint_timer::ui::contracts::DailyStatisticGraphContract::LegendData;
using sprint_timer::use_cases::WorkScheduleQuery;
using ::testing::Return;
using ::testing::ReturnRef;

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

std::vector<sprint_timer::use_cases::SprintDTO> buildSomeSprints()
{
    using namespace std::chrono_literals;
    using namespace dw;
    using sprint_timer::use_cases::SprintDTO;
    FakeUuidGenerator generator;
    std::vector<SprintDTO> sprints;
    auto append_n = [&sprints](const auto& sprint, size_t times) {
        for (size_t i = 0; i < times; ++i) {
            sprints.push_back(sprint);
        }
    };
    const DateTime dateTime{Date{Year{2020}, Month{2}, Day{1}}};
    const DateTimeRange dateTimeRange{dateTime, dateTime};
    const std::string taskUuid{"123"};

    append_n(SprintDTO{generator.generateUUID(),
                       taskUuid,
                       "",
                       std::vector<std::string>{},
                       dateTimeRange},
             2);
    append_n(SprintDTO{generator.generateUUID(),
                       taskUuid,
                       "",
                       std::vector<std::string>{},
                       add_offset(dateTimeRange, Days{1})},
             12);
    append_n(SprintDTO{generator.generateUUID(),
                       taskUuid,
                       "",
                       std::vector<std::string>{},
                       add_offset(dateTimeRange, Days{2})},
             15);
    append_n(SprintDTO{generator.generateUUID(),
                       taskUuid,
                       "",
                       std::vector<std::string>{},
                       add_offset(dateTimeRange, Days{4})},
             10);

    return sprints;
}

class DailyStatisticsViewMock
    : public sprint_timer::ui::contracts::DailyStatisticGraphContract::View {
public:
    MOCK_METHOD(void, updateLegend, (const LegendData&), (override));
    MOCK_METHOD(void, drawGraph, (const GraphData&), (override));
    MOCK_METHOD(void, clearGraphs, (), (override));
};

class DailyStatisticsSharedDataFetcherFixture : public ::testing::Test {
public:
    ::testing::NiceMock<mocks::QueryHandlerMock<WorkScheduleQuery>>
        workScheduleHandlerMock;
    sprint_timer::ui::StatisticsMediator mediator;
    ::testing::NiceMock<mocks::ColleagueMock> fakeColleague;
    ::testing::NiceMock<DailyStatisticsViewMock> viewMock;
    size_t numTopTags{6};
};

TEST_F(DailyStatisticsSharedDataFetcherFixture,
       sets_placeholder_legend_data_for_view_when_not_provided_date_range)
{
    const StatisticsContext statisticsContext;
    DailyStatisticsGraphPresenter sut{
        workScheduleHandlerMock, mediator, statisticsContext};

    EXPECT_CALL(viewMock, updateLegend(LegendData{"No data", "No data"}));
    EXPECT_CALL(viewMock, drawGraph(::testing::_)).Times(0);

    sut.attachView(viewMock);
}

TEST_F(DailyStatisticsSharedDataFetcherFixture,
       updates_legend_with_generic_data)
{
    using namespace dw;
    using namespace ::testing;
    constexpr DateRange dateRange{Date{Year{2020}, Month{2}, Day{1}},
                                  Date{Year{2020}, Month{2}, Day{5}}};
    sprint_timer::WeekSchedule weekSchedule;
    weekSchedule.setTargetGoal(dw::Weekday::Monday, 12);
    weekSchedule.setTargetGoal(dw::Weekday::Tuesday, 12);
    weekSchedule.setTargetGoal(dw::Weekday::Wednesday, 0);
    weekSchedule.setTargetGoal(dw::Weekday::Thursday, 10);
    weekSchedule.setTargetGoal(dw::Weekday::Friday, 10);
    weekSchedule.setTargetGoal(dw::Weekday::Saturday, 0);
    weekSchedule.setTargetGoal(dw::Weekday::Sunday, 0);
    sprint_timer::WorkSchedule schedule;
    schedule.addWeekSchedule(dateRange.start() - dw::Years{1}, weekSchedule);
    const LegendData expected{"39", "19.50"};
    const auto sprints = buildSomeSprints();
    sprint_timer::ui::StatisticsContext statisticsContext{
        sprints, dateRange, numTopTags};
    sprint_timer::ui::DailyStatisticsGraphPresenter sut{
        workScheduleHandlerMock, mediator, statisticsContext};
    mocks::given_handler_returns(workScheduleHandlerMock, schedule);

    EXPECT_CALL(viewMock, updateLegend(expected));

    sut.attachView(viewMock);
}

TEST_F(DailyStatisticsSharedDataFetcherFixture, updates_graph_with_generic_data)
{
    using sprint_timer::Distribution;
    using namespace dw;
    using namespace sprint_timer::ui::contracts::DailyStatisticGraphContract;
    using ::testing::_;
    using ::testing::ByMove;
    const GraphOptions expectedDailyOptions{
        std::string{constants::dailyGraphColor},
        constants::penWidthF,
        true,
        std::string{constants::pointColor},
        LineStyle::Solid};
    const GraphData expectedDaily{expectedDailyOptions,
                                  {GraphValue{Value{0}, Value{2}, "1"},
                                   GraphValue{Value{1}, Value{12}, "2"},
                                   GraphValue{Value{2}, Value{15}, "3"},
                                   GraphValue{Value{3}, Value{0}, "4"},
                                   GraphValue{Value{4}, Value{10}, "5"}}};
    const GraphOptions expectedAverageOptions{std::string{constants::goalColor},
                                              constants::penWidthF,
                                              false,
                                              "",
                                              LineStyle::Dash};
    const GraphData expectedAverage{expectedAverageOptions,
                                    {GraphValue{Value{0}, Value{12}, ""},
                                     GraphValue{Value{4}, Value{12}, ""}}};
    const GraphOptions actualAverageOptions{
        std::string{constants::averageColor},
        constants::penWidthF,
        false,
        "",
        LineStyle::Solid};
    const GraphData actualAverage{actualAverageOptions,
                                  {GraphValue{Value{0}, Value{19.5}, ""},
                                   GraphValue{Value{4}, Value{19.5}, ""}}};
    constexpr DateRange dateRange{Date{Year{2020}, Month{2}, Day{1}},
                                  Date{Year{2020}, Month{2}, Day{5}}};
    sprint_timer::WeekSchedule weekSchedule;
    weekSchedule.setTargetGoal(dw::Weekday::Monday, 12);
    weekSchedule.setTargetGoal(dw::Weekday::Tuesday, 12);
    weekSchedule.setTargetGoal(dw::Weekday::Wednesday, 0);
    weekSchedule.setTargetGoal(dw::Weekday::Thursday, 10);
    weekSchedule.setTargetGoal(dw::Weekday::Friday, 10);
    weekSchedule.setTargetGoal(dw::Weekday::Saturday, 0);
    weekSchedule.setTargetGoal(dw::Weekday::Sunday, 0);
    sprint_timer::WorkSchedule schedule;
    schedule.addWeekSchedule(dateRange.start() - dw::Years{1}, weekSchedule);
    const auto sprints = buildSomeSprints();
    sprint_timer::ui::StatisticsContext statisticsContext{
        sprints, dateRange, numTopTags};
    sprint_timer::ui::DailyStatisticsGraphPresenter sut{
        workScheduleHandlerMock, mediator, statisticsContext};
    mocks::given_handler_returns(workScheduleHandlerMock, schedule);

    EXPECT_CALL(viewMock, clearGraphs());
    EXPECT_CALL(viewMock, drawGraph(expectedDaily));
    EXPECT_CALL(viewMock, drawGraph(expectedAverage));
    EXPECT_CALL(viewMock, drawGraph(actualAverage));

    sut.attachView(viewMock);
}

