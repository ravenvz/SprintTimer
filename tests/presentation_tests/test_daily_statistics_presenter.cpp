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
#include "mocks/QueryHandlerMock.h"
#include "mocks/StatisticsColleagueMock.h"
#include "mocks/StatisticsMediatorMock.h"
#include <core/SprintBuilder.h>
#include <qt_gui/presentation/DailyStatisticsGraphPresenter.h>

using sprint_timer::entities::Sprint;
using sprint_timer::ui::contracts::DailyStatisticGraphContract::GraphData;
using sprint_timer::ui::contracts::DailyStatisticGraphContract::LegendData;
using sprint_timer::use_cases::WorkScheduleQuery;
using ::testing::ReturnRef;

namespace constants {

const std::string dailyGraphColor{"#f63c0d"};
const std::string averageColor{"#3949c4"};
const std::string goalColor{"#39c473"};
const std::string pointColor{"#ffffff"};
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

template <typename WrappedType>
bool operator==(const WrappedType& lhs, const WrappedType& rhs)
{
    return lhs.value == rhs.value;
}

template <> bool operator==(const Value& lhs, const Value& rhs)
{
    return std::abs(lhs.value - rhs.value) < 0.001;
}

bool operator==(const GraphValue& lhs, const GraphValue& rhs)
{
    return lhs.xValue == rhs.xValue && rhs.yValue == rhs.yValue &&
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

std::vector<Sprint> buildSprintsFixture()
{
    using namespace std::chrono_literals;
    using namespace dw;
    std::vector<Sprint> sprints;
    auto append_n = [&sprints](const auto& sprint, size_t times) {
        for (size_t i = 0; i < times; ++i)
            sprints.push_back(sprint);
    };
    sprint_timer::SprintBuilder builder;
    builder.withTaskUuid("123");
    DateTime dateTime{Date{Year{2020}, Month{2}, Day{1}}};
    builder.withTimeSpan(DateTimeRange{dateTime, dateTime});
    append_n(builder.build(), 2);
    builder.withTimeSpan(DateTimeRange{dateTime + Days{1}, dateTime + Days{1}});
    append_n(builder.build(), 12);
    builder.withTimeSpan(DateTimeRange{dateTime + Days{2}, dateTime + Days{2}});
    append_n(builder.build(), 15);
    builder.withTimeSpan(DateTimeRange{dateTime + Days{4}, dateTime + Days{4}});
    append_n(builder.build(), 10);
    return sprints;
}

class DailyStatisticsViewMock
    : public sprint_timer::ui::contracts::DailyStatisticGraphContract::View {
public:
    MOCK_METHOD(void, updateLegend, (const LegendData&), (override));
    MOCK_METHOD(void, drawGraph, (const GraphData&), (override));
    MOCK_METHOD(void, clearGraphs, (), (override));
};

class DailyStatisticsPresenterFixture : public ::testing::Test {
public:
    ::testing::NiceMock<
        mocks::QueryHandlerMock<WorkScheduleQuery, sprint_timer::WorkSchedule>>
        workScheduleHandlerMock;
    ::testing::NiceMock<
        mocks::QueryHandlerMock<sprint_timer::use_cases::RequestSprintsQuery,
                                std::vector<Sprint>>>
        requestSprintsHandlerMock;
    ::testing::NiceMock<mocks::StatisticsMediatorMock> mediator_mock;
    sprint_timer::ui::DailyStatisticsGraphPresenter presenter{
        workScheduleHandlerMock, mediator_mock};
    ::testing::NiceMock<mocks::ColleagueMock> fakeColleague;
    ::testing::NiceMock<DailyStatisticsViewMock> viewMock;
};

TEST_F(DailyStatisticsPresenterFixture,
       sets_placeholder_legend_data_for_view_when_not_provided_date_range)
{
    ON_CALL(mediator_mock, range())
        .WillByDefault(::testing::Return(std::nullopt));

    EXPECT_CALL(viewMock, updateLegend(LegendData{"No data", "No data"}));
    EXPECT_CALL(viewMock, drawGraph(::testing::_)).Times(0);

    presenter.attachView(viewMock);
}

TEST_F(DailyStatisticsPresenterFixture, updates_legend_with_generic_data)
{
    using sprint_timer::Distribution;
    using namespace dw;
    using ::testing::_;
    using ::testing::Return;

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
    const auto sprints = buildSprintsFixture();
    const LegendData expected{"39", "19.50"};

    ON_CALL(mediator_mock, range()).WillByDefault(Return(dateRange));
    ON_CALL(mediator_mock, sprints()).WillByDefault(ReturnRef(sprints));
    ON_CALL(workScheduleHandlerMock, handle(_)).WillByDefault(Return(schedule));

    EXPECT_CALL(viewMock, updateLegend(expected)).Times(1);

    presenter.attachView(viewMock);
}

TEST_F(DailyStatisticsPresenterFixture, updates_graph_with_generic_data)
{
    using sprint_timer::Distribution;
    using namespace dw;
    using namespace sprint_timer::ui::contracts::DailyStatisticGraphContract;
    using ::testing::_;
    using ::testing::Return;

    const GraphOptions expectedDailyOptions{constants::dailyGraphColor,
                                            constants::penWidthF,
                                            true,
                                            constants::pointColor,
                                            LineStyle::Solid};
    const GraphData expectedDaily{expectedDailyOptions,
                                  {GraphValue{Value{0}, Value{2}, "1"},
                                   GraphValue{Value{1}, Value{12}, "2"},
                                   GraphValue{Value{2}, Value{15}, "3"},
                                   GraphValue{Value{3}, Value{0}, "4"},
                                   GraphValue{Value{4}, Value{10}, "5"}}};

    const GraphOptions expectedAverageOptions{
        constants::goalColor, constants::penWidthF, false, "", LineStyle::Dash};
    const GraphData expectedAverage{expectedAverageOptions,
                                    {GraphValue{Value{0}, Value{12}, ""},
                                     GraphValue{Value{4}, Value{12}, ""}}};

    const GraphOptions actualAverageOptions{constants::averageColor,
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
    const auto sprints = buildSprintsFixture();

    ON_CALL(mediator_mock, range()).WillByDefault(Return(dateRange));
    ON_CALL(mediator_mock, sprints()).WillByDefault(ReturnRef(sprints));
    ON_CALL(workScheduleHandlerMock, handle(_)).WillByDefault(Return(schedule));

    EXPECT_CALL(viewMock, clearGraphs());
    EXPECT_CALL(viewMock, drawGraph(expectedDaily));
    EXPECT_CALL(viewMock, drawGraph(expectedAverage));
    EXPECT_CALL(viewMock, drawGraph(actualAverage));

    presenter.attachView(viewMock);
}

TEST_F(DailyStatisticsPresenterFixture,
       requeries_handler_when_shared_data_is_changed)
{
    using sprint_timer::Distribution;
    using ::testing::_;
    using ::testing::Return;
    using ::testing::Truly;
    using namespace dw;
    const Distribution<double> distribution{{2.0, 12.0, 15.0, 0.0, 10.0}};
    constexpr DateRange dateRange{Date{Year{2020}, Month{2}, Day{1}},
                                  Date{Year{2020}, Month{2}, Day{5}}};
    const auto newDateRange = std::make_optional<dw::DateRange>(
        dw::current_date(), dw::current_date());
    sprint_timer::WeekSchedule weekSchedule;
    sprint_timer::WorkSchedule schedule;
    schedule.addWeekSchedule(dateRange.start() - dw::Years{1}, weekSchedule);
    presenter.attachView(viewMock);
    const std::vector<Sprint> stubSprints;

    ON_CALL(mediator_mock, range()).WillByDefault(Return(newDateRange));
    ON_CALL(workScheduleHandlerMock, handle(_)).WillByDefault(Return(schedule));

    EXPECT_CALL(mediator_mock, sprints()).WillOnce(ReturnRef(stubSprints));

    presenter.onSharedDataChanged();
}
